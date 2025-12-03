#ifndef _UTILS_H
#define _UTILS_H

#include "btype.h"
#include <bits/stdc++.h>

Btype str_to_type(const std::string &s);
int parse_int(const std::string &s);
std::string createLabel();
std::string createVar(const Btype &b, bool is_array);

#endif 