#ifndef CACT_IR_DCE_H
#define CACT_IR_DCE_H

#include <string>
#include <vector>

// Dead code elimination on IR text.
// Phase 1: removes unreachable @func...@endfunc blocks.
void ir_dce(std::vector<std::string> &ir_lines);

#endif // CACT_IR_DCE_H
