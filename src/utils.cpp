#include "utils.h"
#include "SymTable.h"
#include "FuncTable.h"
#include "tree/ParseTree.h"
#include "Analyzer.h"
#include "global.h"

extern int g_count;
extern std::string btype_string[];

Btype str_to_type(const std::string &s) {
  if (s == "int") return Int;
  else if (s == "float") return Float;
  else if (s == "double") return Double;
  else if (s == "bool") return Bool;
  else if (s == "void") return Void;
  else {
    // 
    assert(0);
    return Void;
  }
}

int parse_int(const std::string &s) {
  return std::stoi(s, nullptr, 0);
}

std::string createLabel() {
  ++g_count;
  return "l_" + std::to_string(g_count);
}

std::string createVar(const Btype &b, bool is_array) {
  ++g_count;
  if (!is_array) return btype_string[b] + "_" + std::to_string(g_count);
  else return "a" + btype_string[b] + "_" + std::to_string(g_count);
}

void printTree(antlr4::tree::ParseTree *tree,
               const std::vector<std::string> &ruleNames,
               const std::string &indent, bool isLast) {
  std::cerr << indent;

  if (isLast) {
    std::cerr << "└── ";
  } else {
    std::cerr << "├── ";
  }

  if (tree->children.empty()) {
    std::cerr << tree->toString() << std::endl;
  } else {
    auto *ctx = dynamic_cast<antlr4::ParserRuleContext *>(tree);
    if (ctx) {
      std::cerr << ruleNames[ctx->getRuleIndex()] << std::endl;
    } else {
      std::cerr << "Unknown" << std::endl;
    }
  }

  std::string newIndent = indent + (isLast ? "    " : "│   ");
  for (size_t i = 0; i < tree->children.size(); i++) {
    printTree(tree->children[i], ruleNames, newIndent,
              i == tree->children.size() - 1);
  }
}

void printTokens(CACTLexer &lexer, antlr4::CommonTokenStream &tokens) {
  for (auto token : tokens.getTokens()) {
    if (token->getType() != antlr4::Token::EOF) {
      std::cerr << "Line " << token->getLine() << ":"
                << token->getCharPositionInLine() << " "
                << lexer.getVocabulary().getSymbolicName(token->getType())
                << " = '" << token->getText() << "'" << std::endl;
    }
  }
}


// template<typename... Args>
// void dbg(Args&&... args) {
//     [](auto&&... params) {
//         std::size_t n = 0;
//         ((std::cerr << params << (++n != sizeof...(params) ? " " : "")), ...);
//     }(std::forward<Args>(args)...);
    
//     std::cerr << std::endl;
// }

// template<typename... Args>
// void dbg(const Args&... args) {
//     bool first = true;
//     ((std::cerr << (first ? first = false, "" : " ") << args), ...);
//     std::cerr << std::endl;
// }

// template<typename... Args>
// std::string gen(const Args&... args) {
//     std::ostringstream oss;
//     bool first = true;
//     ((oss << (first ? first = false, "" : " ") << args), ...);
//     return oss.str();
// }

// template<typename... Args>
// void out(const Args&... args) {
//     bool first = true;
//     ((*global_out << (first ? first = false, "" : " ") << args), ...);
//     *global_out << std::endl;
// }