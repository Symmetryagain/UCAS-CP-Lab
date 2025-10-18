#include <iostream>

// #include "antlr4-runtime.h"
#include "tree/ErrorNode.h"
#include "tree/ParseTree.h"

#include "CACTLexer.h"
#include "CACTParser.h"
#include "CACTVisitor.h"

using namespace antlr4;

// class Analysis : public HelloVisitor {
// public:
//     std::any visitR(HelloParser::RContext *context) {
//         visitChildren( context );
        
//         std::cout << "enter rule [r]!" << std::endl;
//         std::cout << "the ID is: " << context->ID()->getText().c_str() << std::endl;
//         return nullptr;
//     }

//     std::any visitErrorNode(tree::ErrorNode * node) override {
//         std::cout << "visit error node!" << std::endl;
//         return nullptr;
//     }
// };

int main(int argc, const char* argv[]) {
    if(argc < 2) {
        std::cout << "Error. Please enter source file." << std::endl;
        return 1;
    }

    std::ifstream stream;
    stream.open(argv[1]);

    if (!stream.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return 9;
    }

    ANTLRInputStream input(stream);
    CACTLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    CACTParser parser(&tokens);

    tree::ParseTree *ctx = parser.compilationUnit();

    if (lexer.getNumberOfSyntaxErrors() > 0) {
        std::cout << "lex error: " << lexer.getNumberOfSyntaxErrors() << std::endl;
        std::cout << "False" << std::endl;
        return 1;
    }

    if (parser.getNumberOfSyntaxErrors() > 0) {
        std::cout << "syntax error: " << parser.getNumberOfSyntaxErrors() << std::endl;
        std::cout << "False" << std::endl;
        return 2;
    }

    std::cout << "True" << std::endl;

    return 0;
}