#include "parser.h"
#include "interpreter.h"

int main() {

    const char *input = "reserve room deluxe from 2023-01-01 to 2023-01-05 for John Doe";
    ASTNode *parsedTree = parseStatement(&input);

    if (parsedTree != NULL) {
        printf("Parsing successful!\n");
        executeProgram(parsedTree);

    } else {
        printf("Parsing failed.\n");
    }

    return 0;
}
