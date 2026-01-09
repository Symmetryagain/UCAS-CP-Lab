#ifndef _UTILS_H
#define _UTILS_H

#include "btype.h"
#include "tree/ParseTree.h"
#include <bits/stdc++.h>
#include "Analyzer.h"
#include "global.h"

Btype str_to_type(const std::string &s);

int parse_int(const std::string &s);

std::string createLabel();

std::string createVar(const Btype &b, bool is_array);

void printTree(antlr4::tree::ParseTree *tree,
               const std::vector<std::string> &ruleNames,
               const std::string &indent, bool isLast);
               
void printTokens(CACTLexer &lexer, antlr4::CommonTokenStream &tokens);

template<typename... Args>
void dbg(const Args&... args) {
    bool first = true;
    ((std::cerr << (first ? first = false, "" : " ") << args), ...);
    std::cerr << std::endl;
}

template<typename... Args>
std::string gen(const Args&... args) {
    std::ostringstream oss;
    bool first = true;
    ((oss << (first ? first = false, "" : " ") << args), ...);
    oss << "\n";
    return oss.str();
}

template<typename... Args>
void out(const Args&... args) {
    bool first = true;
    ((*global_out << (first ? first = false, "" : " ") << args), ...);
    *global_out << std::endl;
}

#endif // _UTILS_H