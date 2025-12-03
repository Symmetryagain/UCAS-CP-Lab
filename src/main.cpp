#include <iostream>

// #include "antlr4-runtime.h"
#include "tree/ErrorNode.h"
#include "tree/ParseTree.h"

#include "Analyzer.h"

using namespace antlr4;
extern SymTree g_symtree;
extern FuncTable g_functable;
extern std::string special_funcname[];
extern Btype special_funcType[];
extern std::vector<FuncParamsType> special_funcParams[];

// 添加树形打印功能
void printTree(antlr4::tree::ParseTree *tree,
               const std::vector<std::string> &ruleNames,
               const std::string &indent = "", bool isLast = true) {
  std::cout << indent;

  if (isLast) {
    std::cout << "└── ";
  } else {
    std::cout << "├── ";
  }

  // 打印节点信息
  if (tree->children.empty()) {
    // 叶子节点（终结符）
    std::cout << tree->toString() << std::endl;
  } else {
    // 内部节点（非终结符）
    auto *ctx = dynamic_cast<antlr4::ParserRuleContext *>(tree);
    if (ctx) {
      std::cout << ruleNames[ctx->getRuleIndex()] << std::endl;
    } else {
      std::cout << "Unknown" << std::endl;
    }
  }

  // 递归打印子节点
  std::string newIndent = indent + (isLast ? "    " : "│   ");
  for (size_t i = 0; i < tree->children.size(); i++) {
    printTree(tree->children[i], ruleNames, newIndent,
              i == tree->children.size() - 1);
  }
}

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
  std::cout << "=== Token Stream ===" << std::endl;
  tokens.fill();
  for (auto token : tokens.getTokens()) {
    if (token->getType() != antlr4::Token::EOF) {
      std::cout << "Line " << token->getLine() << ":"
                << token->getCharPositionInLine() << " "
                << lexer.getVocabulary().getSymbolicName(token->getType())
                << " = '" << token->getText() << "'" << std::endl;
    }
  }

  CACTParser parser(&tokens);

  // 获取解析树
  auto tree = parser.comp_units();

  // 打印解析树
  std::cout << "=== Parse Tree ===" << std::endl;
  std::cout << tree->toStringTree(&parser) << std::endl;

  std::cout << "\n=== Detailed AST ===" << std::endl;
  printTree(tree, parser.getRuleNames());

  if (lexer.getNumberOfSyntaxErrors() > 0) {
    std::cout << "lex error: " << lexer.getNumberOfSyntaxErrors() << std::endl;
    std::cout << "False" << std::endl;
    return 1;
  }

  if (parser.getNumberOfSyntaxErrors() > 0) {
    std::cout << "syntax error: " << parser.getNumberOfSyntaxErrors()
              << std::endl;
    std::cout << "False" << std::endl;
    return 2;
  }

  std::cout << "=== End ===" << std::endl;
  std::cout << "Number of errors: "
            << lexer.getNumberOfSyntaxErrors() +
                   parser.getNumberOfSyntaxErrors()
            << std::endl;

  std::cout << "True" << std::endl;

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