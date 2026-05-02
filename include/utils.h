#ifndef CACT_UTILS_H
#define CACT_UTILS_H

#include <iostream>
#include <sstream>
#include <string>

#include "btype.h"

Btype str_to_type(const std::string &s);

int parse_int(const std::string &s);

std::string createLabel();

std::string createVar(const Btype &b, bool is_array);

extern std::ostream* global_out;

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

#endif // CACT_UTILS_H
