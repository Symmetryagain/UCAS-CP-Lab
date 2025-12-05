#include <iostream>

// #include "antlr4-runtime.h"
// #include "tree/ErrorNode.h"
#include "tree/ParseTree.h"
#include "utils.h"
#include "Analyzer.h"

using namespace antlr4;
extern SymTree g_symtree;
extern FuncTable g_functable;
extern std::string special_funcname[];
extern Btype special_funcType[];
extern std::vector<FuncParamsType> special_funcParams[];

int main(int argc, const char *argv[]) {
  std::ifstream stream;
  if (argc > 1) {
    stream.open(argv[1]);
  } else {
    std::cerr << "No input file specified" << std::endl;
    return 9;
  }

  if (!stream.is_open()) {
    std::cerr << "Failed to open file" << std::endl;
    return 9;
  }

  ANTLRInputStream input(stream);
  CACTLexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  // 打印所有Token
  std::cerr << "=== Token Stream ===" << std::endl;
  tokens.fill();
  for (auto token : tokens.getTokens()) {
    if (token->getType() != antlr4::Token::EOF) {
      std::cerr << "Line " << token->getLine() << ":"
                << token->getCharPositionInLine() << " "
                << lexer.getVocabulary().getSymbolicName(token->getType())
                << " = '" << token->getText() << "'" << std::endl;
    }
  }

  CACTParser parser(&tokens);

  // 获取解析树
  auto tree = parser.comp_units();

  // Print parser tree
  // std::cerr << "=== Parse Tree ===" << std::endl;
  // std::cerr << tree->toStringTree(&parser) << std::endl;

  // std::cerr << "\n=== Detailed AST ===" << std::endl;
  // printTree(tree, parser.getRuleNames(), "", true);

  if (lexer.getNumberOfSyntaxErrors() > 0) {
    std::cerr << "lex error: " << lexer.getNumberOfSyntaxErrors() << std::endl;
    std::cerr << "False" << std::endl;
    return 1;
  }

  if (parser.getNumberOfSyntaxErrors() > 0) {
    std::cerr << "syntax error: " << parser.getNumberOfSyntaxErrors()
              << std::endl;
    std::cerr << "False" << std::endl;
    return 2;
  }

  std::cerr << "=== End ===" << std::endl;
  std::cerr << "Number of errors: "
            << lexer.getNumberOfSyntaxErrors() +
                   parser.getNumberOfSyntaxErrors()
            << std::endl;

  std::cerr << "True" << std::endl;

  Analyzer visitor;
  g_symtree.enterScope();
  for (int i = 0; i < 7; ++i) {
    if (!g_functable.define(
      special_funcType[i],
      special_funcname[i],
      special_funcParams[i]
    )) {
      // should not reach here
    }
  }
  visitor.visit(tree);
  if (!g_functable.check(special_funcname[7])) { // main
    //
    exit(1);  
    assert(0);
  }
  g_symtree.leaveScope();
  return parser.getNumberOfSyntaxErrors() + lexer.getNumberOfSyntaxErrors();
}