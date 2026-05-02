#include <fstream>
#include <iostream>
#include <string>

#include "analyzer.h"
#include "asm_gen.h"
#include "context.h"
#include "tree/ParseTree.h"
#include "utils.h"
#include "utils_debug.h"

using namespace std;

int            merge_constant = 0;
int            optimize_level = 0;
vector<string> ir_lines(0);
vector<string> asm_lines = {
    ".option nopic",
    ".attribute  arch, \"rv64i2p0_m2p0_a2p0_f2p0_d2p0_c2p0\"",
    ".attribute  unaligned_access, 0",
    ".attribute  stack_align, 16"
};

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0]
              << " input_file_name [--print-tokens] [--print-parser-tree] "
                 "[--print-ast] [--syntax] [--emit-IR] [-O1] output_file_name"
              << std::endl;
    return 9;
  }

  std::string input_path = argv[1];
  bool opt_print_tokens = false;
  bool opt_print_parser_tree = false;
  bool opt_print_ast = false;
  bool opt_syntax_only = false;
  bool opt_emit_IR = false;

  std::string raw_path = input_path;
  size_t pos = raw_path.rfind(".cact");
  if (pos != std::string::npos) {
    raw_path = raw_path.substr(0, pos);
  }

  std::string ir_path, output_path;
  bool opt_out_file_provided = false;

  for (int i = 2; i < argc; ++i) {
    std::string a = argv[i];
    if (a == "--print-tokens")
      opt_print_tokens = true;
    else if (a == "--print-parser-tree")
      opt_print_parser_tree = true;
    else if (a == "--print-ast")
      opt_print_ast = true;
    else if (a == "--syntax")
      opt_syntax_only = true;
    else if (a == "-O1")
      merge_constant = optimize_level = 1;
    else if (a == "--emit-IR")
      opt_emit_IR = true;
    else { // output file
      opt_out_file_provided = true;
      ir_path = output_path = a;
    }
  }
  if (!opt_out_file_provided || !opt_emit_IR) {
    ir_path = raw_path + ".ir";
  }

  std::ifstream stream(input_path);
  if (!stream.is_open()) {
    std::cerr << "Failed to open file: " << input_path << std::endl;
    return 9;
  }

  std::ofstream outfile(ir_path);
  if (!outfile.is_open()) {
    std::cerr << "Failed to open output file: " << ir_path << std::endl;
    return 9;
  }

  antlr4::ANTLRInputStream input(stream);
  CACTLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);

  // Force lexing
  tokens.fill();

  if (lexer.getNumberOfSyntaxErrors() > 0) {
    std::cerr << "lex error: " << lexer.getNumberOfSyntaxErrors() << std::endl;
    std::cerr << "False" << std::endl;
    return 1;
  }

  if (opt_print_tokens) {
    std::cerr << "=== Token Stream ===" << std::endl;
    printTokens(lexer, tokens);
  }

  CACTParser parser(&tokens);

  // parse
  antlr4::tree::ParseTree *tree = nullptr;
  try {
    tree = parser.comp_units();
  } catch (...) {
    std::cerr << "Parse failed with an exception" << std::endl;
    return 2;
  }

  if (parser.getNumberOfSyntaxErrors() > 0) {
    std::cerr << "syntax error: " << parser.getNumberOfSyntaxErrors()
              << std::endl;
    std::cerr << "False" << std::endl;
    return 2;
  }

  if (opt_print_parser_tree) {
    std::cerr << "=== Parse Tree ===" << std::endl;
    std::cerr << tree->toStringTree(&parser) << std::endl;
  }

  if (opt_print_ast) {
    std::cerr << "\n=== Detailed AST ===" << std::endl;
    printTree(tree, parser.getRuleNames(), "", true);
  }

  std::cerr << "=== Lex and Parse End ===" << std::endl;

  if (opt_syntax_only)
    return 0;

  // semantic analysis / visit
  Analyzer visitor;
  g_symtree.enterScope();
  for (int i = 0; i < 7; ++i) {
    if (!g_functable.define(special_funcType[i], special_funcname[i],
                            special_funcParams[i])) {
      // should not reach here
    }
  }

  global_out = &outfile;

  visitor.visit(tree);

  if (!g_functable.check(special_funcname[7])) { // main
    return 3;
  }
  g_symtree.leaveScope();

  if (opt_emit_IR) 
    return 0;

  stream.close();
  outfile.close();

  if (!opt_out_file_provided) {
    output_path = raw_path + ".s";
  }
  // open ir file as input
  std::ifstream ir_file(ir_path);
  if (!ir_file.is_open()) {
    std::cerr << "Failed to open IR file: " << ir_path << std::endl;
    return 9;
  }

  // open asm file as output
  std::ofstream asm_file(output_path);
  if (!asm_file.is_open()) {
    std::cerr << "Failed to open output file: " << output_path << std::endl;
    return 9;
  }

  // input
	std::cerr << "Reading IR file..." << std::endl;

	string ir_str;
	while(getline(ir_file, ir_str))
	{
		ir_lines.push_back(ir_str);
	}
	ir_file.close();

  // delete ir file
  system(("rm " + ir_path).c_str());

	std::cerr << "IR file read finished." << std::endl;
	std::cerr << "Number of lines: " << ir_lines.size() << std::endl;

	// asmgen
	std::cerr << "Starting ASM generate..." << std::endl;
	asmgen();
	std::cerr << "ASM generated finished." << std::endl;

	// output
	std::cerr << "Writing ASM file..." << std::endl;
	for(auto line : asm_lines)
	{
		asm_file << line << std::endl;
	}
	asm_file.close();
	std::cerr << "ASM file written finished." << std::endl;
  return 0;
}