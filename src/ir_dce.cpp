#include "ir_dce.h"
#include "utils.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// --- helpers ---

static vector<string> split(const string &str) {
    istringstream iss(str);
    vector<string> tokens;
    string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}

static bool is_var(const string &s) {
    return s.size() >= 2 && s[0] == '%';
}

// Extract base variable name (%ai_5[3] → %ai_5, %ai_5[%i_2] → %ai_5)
static string var_base(const string &s) {
    size_t br = s.find('[');
    return (br != string::npos) ? s.substr(0, br) : s;
}

// Extract all %-variables from a token, including those inside brackets.
// E.g., "%ai_5[%i_2]" → {"%ai_5", "%i_2"}
static vector<string> extract_vars(const string &token) {
    vector<string> vars;
    if (token.size() < 2 || token[0] != '%') return vars;

    // Find all %-prefixed substrings in this token
    for (size_t i = 0; i < token.size(); ++i) {
        if (token[i] == '%') {
            size_t j = i + 1;
            while (j < token.size() && (isalnum(token[j]) || token[j] == '_'))
                ++j;
            string v = token.substr(i, j - i);
            vars.push_back(v);
            i = j - 1;
        }
    }
    return vars;
}

// --- Phase 1: function-level DCE ---

static void dce_functions(vector<string> &lines) {
    map<string, int> func_start, func_end;
    set<string> all_funcs;

    for (size_t i = 0; i < lines.size(); ++i) {
        auto tokens = split(lines[i]);
        if (tokens.empty()) continue;
        if (tokens[0] == "@func") {
            func_start[tokens[1]] = i;
            all_funcs.insert(tokens[1]);
        }
        if (tokens[0] == "@endfunc") {
            for (auto it = func_start.rbegin(); it != func_start.rend(); ++it)
                if (func_end.count(it->first) == 0) { func_end[it->first] = i; break; }
        }
    }

    if (all_funcs.count("%main") == 0) return;

    // call graph
    map<string, set<string>> call_graph;
    for (const auto &[fname, start] : func_start) {
        int end = func_end.count(fname) ? func_end[fname] : (int)lines.size();
        for (int i = start + 1; i < end; ++i) {
            auto tokens = split(lines[i]);
            if (tokens.empty() || tokens[0] != "call") continue;
            string callee = (tokens[1] == "void") ? tokens[2] : tokens[2];
            if (all_funcs.count(callee)) call_graph[fname].insert(callee);
        }
    }

    // BFS
    set<string> reachable;
    queue<string> q;
    q.push("%main");
    reachable.insert("%main");
    while (!q.empty()) {
        string f = q.front(); q.pop();
        for (const auto &c : call_graph[f])
            if (reachable.count(c) == 0) { reachable.insert(c); q.push(c); }
    }

    // filter
    vector<string> result;
    bool skipping = false;
    for (size_t i = 0; i < lines.size(); ++i) {
        auto tokens = split(lines[i]);
        if (tokens.empty()) { result.push_back(lines[i]); continue; }
        if (tokens[0] == "@func") {
            string name = tokens[1];
            if (reachable.count(name)) { skipping = false; result.push_back(lines[i]); }
            else { skipping = true; }
            continue;
        }
        if (skipping) { if (tokens[0] == "@endfunc") skipping = false; continue; }
        result.push_back(lines[i]);
    }
    lines = result;
}

// --- Phase 2: instruction-level DCE within functions ---

struct BasicBlock {
    int start;              // index into func_lines (inclusive)
    int end;                // index into func_lines (inclusive)
    set<int> successors;    // block indices
    set<string> def;        // vars defined before any use in this block
    set<string> use;        // vars used before any def in this block
    set<string> in, out;    // liveness
};

// Get destination variable of an instruction (if any)
static string get_dest(const vector<string> &tokens) {
    // Instruction formats:
    // assign DEST SRC
    // op [type] DEST SRC1 SRC2
    // op [type] DEST SRC
    if (tokens[0] == "assign") return (tokens.size() >= 2) ? tokens[1] : "";
    if (tokens[0] == "call")   return (tokens.size() >= 2 && tokens[1] != "void") ? tokens[1] : "";

    // Neg i|f|d DEST SRC, Not DEST SRC
    if (tokens[0] == "Neg" || tokens[0] == "Not")
        return (tokens.size() >= 3 && tokens[1].size() == 1) ? tokens[2] : tokens[1];

    // Add i|f|d DEST SRC1 SRC2, DAnd DEST SRC1 SRC2, etc.
    static const set<string> bin_ops = {"Add","Sub","Mul","Div","Rem","LT","LE","GT","GE","EQ","NE","DAnd","DOr","Addr"};
    if (bin_ops.count(tokens[0])) {
        size_t dest_idx = (tokens.size() >= 3 && tokens[1].size() == 1) ? 2 : 1;
        return (dest_idx < tokens.size()) ? tokens[dest_idx] : "";
    }
    return "";
}

// Collect %-variables used as operands in an instruction
static set<string> collect_uses(const vector<string> &tokens, bool has_dest) {
    set<string> uses;
    size_t start = 1;
    if (has_dest) {
        // The dest itself may contain array indices that are reads (e.g., %ai[%idx])
        string dest = get_dest(tokens);
        if (!dest.empty()) {
            auto vars = extract_vars(dest);
            // All vars except the base array name are "uses" (index reads)
            for (size_t vi = 1; vi < vars.size(); ++vi) uses.insert(vars[vi]);
        }
        start = tokens[1].size() == 1 ? 2 : 1;
        if (start < tokens.size()) start++;
    }
    for (size_t i = start; i < tokens.size(); ++i) {
        auto vars = extract_vars(tokens[i]);
        for (const auto &v : vars) uses.insert(v);
    }
    return uses;
}

// Check if an instruction is removable (no side effects)
static bool is_removable(const vector<string> &tokens) {
    if (tokens.empty()) return false;
    const string &op = tokens[0];
    if (op == "assign") {
        // Removable only if neither dest nor src involves array access
        string dest = get_dest(tokens);
        if (dest.find('[') != string::npos) return false; // array write
        // Check if any operand involves array access (load from memory)
        for (size_t i = 1; i < tokens.size(); ++i)
            if (tokens[i].find('[') != string::npos) return false;
        return true;
    }
    if (op == "@var" || op == "@array") return false;
    if (op == "call" || op == "branch" || op == "return" ||
        op == "label" || op == "retire") return false;
    return true;
}

static vector<int> build_blocks(const vector<string> &func_lines,
                                 vector<BasicBlock> &blocks) {
    blocks.clear();
    int n = (int)func_lines.size();
    if (n == 0) return {};

    // Find leaders: first instruction, labels, and targets of branches
    set<int> leaders;
    leaders.insert(0);

    for (int i = 0; i < n; ++i) {
        auto tokens = split(func_lines[i]);
        if (tokens.empty()) continue;
        if (tokens[0] == "label") {
            leaders.insert(i);
            if (i + 1 < n) leaders.insert(i + 1);
        }
        if (tokens[0] == "branch") {
            if (i + 1 < n) leaders.insert(i + 1);
        }
    }

    // Build ordered leader list
    vector<int> leader_list;
    for (int lidx : leaders) {
        auto tokens = split(func_lines[lidx]);
        // Only include leaders that have non-empty content
        leader_list.push_back(lidx);
    }
    sort(leader_list.begin(), leader_list.end());
    if (leader_list.empty()) return {};

    // Create blocks: each block from leader to next leader-1 or n-1
    for (size_t j = 0; j < leader_list.size(); ++j) {
        BasicBlock bb;
        bb.start = leader_list[j];
        bb.end = (j + 1 < leader_list.size()) ? leader_list[j + 1] - 1 : n - 1;
        if (bb.end < bb.start) bb.end = bb.start;
        blocks.push_back(bb);
    }

    return leader_list;
}

// Find label name in a line (if any)
static string get_label(const string &line) {
    auto tokens = split(line);
    if (!tokens.empty() && tokens[0] == "label") return tokens[1];
    return "";
}

// Map label name → block index
static map<string, int> build_label_map(const vector<string> &func_lines,
                                         const vector<BasicBlock> &blocks) {
    map<string, int> lm;
    for (int i = 0; i < (int)blocks.size(); ++i) {
        string lab = get_label(func_lines[blocks[i].start]);
        if (!lab.empty()) lm[lab] = i;
    }
    return lm;
}

static void build_cfg(const vector<string> &func_lines,
                       vector<BasicBlock> &blocks,
                       const map<string, int> &label_map) {
    int nb = (int)blocks.size();
    for (int i = 0; i < nb; ++i) {
        // Last instruction of this block
        auto tokens = split(func_lines[blocks[i].end]);
        if (tokens.empty()) { if (i + 1 < nb) blocks[i].successors.insert(i + 1); continue; }

        const string &op = tokens[0];
        if (op == "branch") {
            // branch label cond → two successors: label target and fall-through
            string target = tokens[1];
            if (label_map.count(target)) blocks[i].successors.insert(label_map.at(target));
            if (i + 1 < nb) blocks[i].successors.insert(i + 1);
        } else if (op == "return") {
            // no successors
        } else {
            // fall-through
            if (i + 1 < nb) blocks[i].successors.insert(i + 1);
        }
    }
}

static void compute_def_use(const vector<string> &func_lines,
                             vector<BasicBlock> &blocks) {
    for (auto &bb : blocks) {
        set<string> killed; // vars killed so far in this block (going forward)

        for (int i = bb.start; i <= bb.end; ++i) {
            auto tokens = split(func_lines[i]);
            if (tokens.empty()) continue;

            // Skip declarations and labels for def/use purposes
            if (tokens[0] == "@var" || tokens[0] == "@array" ||
                tokens[0] == "label" || tokens[0] == "retire") continue;

            bool has_dest = !get_dest(tokens).empty();

            // Collect uses from this instruction
            set<string> inst_uses = collect_uses(tokens, has_dest);

            // Any use that hasn't been killed yet in this block → goes to block's use set
            for (const auto &u : inst_uses) {
                if (killed.count(u) == 0) bb.use.insert(u);
            }

            // Track definitions
            string dest = get_dest(tokens);
            if (has_dest && !dest.empty()) {
                string dbase = var_base(dest);
                if (!killed.count(dbase)) bb.def.insert(dbase);
                killed.insert(dbase);
            }

            // branch and return are "uses" of their operands but they cannot be removed
            // They're already handled by collect_uses
        }
    }
}

static void liveness_analysis(vector<BasicBlock> &blocks) {
    int nb = (int)blocks.size();
    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = nb - 1; i >= 0; --i) {
            // Out[B] = Union of In[S] for all successors
            set<string> new_out;
            for (int s : blocks[i].successors)
                for (const auto &v : blocks[s].in) new_out.insert(v);
            if (new_out != blocks[i].out) { blocks[i].out = new_out; changed = true; }

            // In[B] = Use[B] ∪ (Out[B] - Def[B])
            set<string> new_in = blocks[i].use;
            for (const auto &v : blocks[i].out)
                if (blocks[i].def.count(v) == 0) new_in.insert(v);
            if (new_in != blocks[i].in) { blocks[i].in = new_in; changed = true; }
        }
    }
}

// Returns number of instructions removed
static int eliminate_dead(const vector<string> &func_lines,
                           const vector<BasicBlock> &blocks,
                           vector<string> &new_lines) {
    // Build per-instruction "live after this instruction" state
    // For each block, work backwards through its instructions,
    // tracking liveness, and mark dead instructions.

    // First, for each block, compute live_set at block end (= out[B])
    // Then walk backwards through the block, updating live_set.

    int removed = 0;
    set<int> dead_indices; // line indices in func_lines that are dead

    for (const auto &bb : blocks) {
        set<string> live = bb.out; // live at block exit

        for (int i = bb.end; i >= bb.start; --i) {
            string line = func_lines[i];
            auto tokens = split(line);
            if (tokens.empty()) continue;

            // Control flow instructions: always live
            if (tokens[0] == "branch" || tokens[0] == "return" ||
                tokens[0] == "label" || tokens[0] == "call") {
                // These are "uses" of their operands but are NOT removable
                bool has_dest = !get_dest(tokens).empty();
                // For branch: cond is a use (already handled below via collect_uses)
                // For call: if not void, the return value is defined here
                // But since call is not removable, we don't check liveness for removal

                // Update liveness: remove defs, add uses
                string dest = get_dest(tokens);
                if (has_dest && !dest.empty()) {
                    string dbase = var_base(dest);
                    live.erase(dbase);
                }
                set<string> uses = collect_uses(tokens, has_dest);
                for (const auto &u : uses) live.insert(u);
                continue;
            }

            // @var, @array declarations: always keep (needed by interpreter/asmgen for state init)
            if (tokens[0] == "@var" || tokens[0] == "@array") {
                live.erase(var_base(tokens[1]));
                continue;
            }

            // retire: just skip
            if (tokens[0] == "retire") continue;

            // Removable instructions
            string dest = get_dest(tokens);
            bool has_dest = !dest.empty();

            // Check if destination is dead
            bool is_dead = has_dest && live.count(var_base(dest)) == 0;

            if (is_dead && is_removable(tokens)) {
                dbg("ir_dce: removing dead:", line);
                dead_indices.insert(i);
                removed++;
                continue;
            }

            // Instruction is live: update liveness
            if (has_dest && !dest.empty()) {
                string dbase = var_base(dest);
                live.erase(dbase);
            }
            set<string> uses = collect_uses(tokens, has_dest);
            for (const auto &u : uses) live.insert(u);
        }
    }

    // Build new function body
    new_lines.clear();
    for (int i = 0; i < (int)func_lines.size(); ++i) {
        string line = func_lines[i];
        if (line.empty()) { new_lines.push_back(line); continue; }
        if (dead_indices.count(i)) continue;
        new_lines.push_back(line);
    }

    return removed;
}

static pair<int,int> find_func_range(const vector<string> &lines, int func_idx) {
    // func_idx is the index of a @func line; return (start, end) inclusive
    int start = func_idx;
    int end = -1;
    for (size_t i = start + 1; i < lines.size(); ++i) {
        auto tokens = split(lines[i]);
        if (!tokens.empty() && tokens[0] == "@endfunc") { end = i; break; }
    }
    return {start, end};
}

static void dce_instructions(vector<string> &lines) {
    bool changed = true;
    while (changed) {
        changed = false;
        // Find all function starts
        vector<int> func_starts;
        for (size_t i = 0; i < lines.size(); ++i) {
            auto tokens = split(lines[i]);
            if (!tokens.empty() && tokens[0] == "@func")
                func_starts.push_back((int)i);
        }

        // Process in reverse so replacements don't invalidate earlier indices
        for (int fi = (int)func_starts.size() - 1; fi >= 0; --fi) {
            auto [start, end] = find_func_range(lines, func_starts[fi]);
            if (start < 0 || end < 0 || end <= start + 1) continue;

            // Extract body
            vector<string> body;
            for (int i = start + 1; i < end; ++i)
                body.push_back(lines[i]);

            vector<BasicBlock> blocks;
            build_blocks(body, blocks);
            if (blocks.empty()) continue;

            auto label_map = build_label_map(body, blocks);
            build_cfg(body, blocks, label_map);
            compute_def_use(body, blocks);
            liveness_analysis(blocks);

            vector<string> new_body;
            int removed = eliminate_dead(body, blocks, new_body);
            if (removed > 0) {
                dbg("ir_dce: iteration removed", removed, "instructions from function");
                lines.erase(lines.begin() + start + 1, lines.begin() + end);
                lines.insert(lines.begin() + start + 1, new_body.begin(), new_body.end());
                changed = true;
            }
        }
    }
}

void ir_dce(vector<string> &ir_lines)
{
    dce_functions(ir_lines);
    dce_instructions(ir_lines);
}
