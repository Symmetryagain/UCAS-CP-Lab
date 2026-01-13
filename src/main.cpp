#include <iostream>

#include "Analyzer.h"
#include "global.h"
#include "tree/ParseTree.h"
#include "utils.h"

int merge_constant = 0;

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0]
              << " input_file_name [--print-tokens] [--print-parser-tree] "
                 "[--print-ast] [--syntax] [--emit-IR] [-O1]"
              << std::endl;
    return 9;
  }

  std::string input_path = argv[1];
  bool opt_print_tokens = false;
  bool opt_print_parser_tree = false;
  bool opt_print_ast = false;
  bool opt_syntax_only = false;

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
      merge_constant = 1;
    else {
      std::cerr << "Unknown option: " << a << std::endl;
      return 9;
    }
  }

  std::ifstream stream(input_path);
  if (!stream.is_open()) {
    std::cerr << "Failed to open file: " << input_path << std::endl;
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

  visitor.visit(tree);

  if (!g_functable.check(special_funcname[7])) { // main
    return 3;
  }
  g_symtree.leaveScope();

  return 0;
}