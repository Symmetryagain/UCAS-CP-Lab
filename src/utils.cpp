#include "utils.h"
#include "utils_debug.h"

#include "context.h"
#include "tree/ParseTree.h"
#include "analyzer.h"

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