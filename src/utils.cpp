#include "utils.h"
#include "SymTable.h"
#include "FuncTable.h"
#include "tree/ParseTree.h"
#include "Analyzer.h"

extern int g_count;
extern std::string btype_string[];

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