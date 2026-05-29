#ifndef CACT_UTILS_DEBUG_H
#define CACT_UTILS_DEBUG_H

#include <string>
#include <vector>

#include "tree/ParseTree.h"

// Forward declarations
class CACTLexer;
namespace antlr4 { class CommonTokenStream; }

void printTree(
    antlr4::tree::ParseTree *tree,
    const std::vector<std::string> &ruleNames,
    const std::string &indent, bool isLast
);

void printTokens(CACTLexer &lexer, antlr4::CommonTokenStream &tokens);

#endif // CACT_UTILS_DEBUG_H
