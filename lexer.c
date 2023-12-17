#include "lexer.h"


// Keywords in the language



const char *keywords[] = {
    "reserve", "update", "cancel", "check", "create", "generate", "request", NULL
};

// Check if a string is a keyword
int isKeyword(const char *str) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

// Lexer function to get the next token
Token getNextToken(const char **input) {
    const char *reader = *input;
    Token token;
    token.type = TOKEN_UNKNOWN;
    strcpy(token.value, "");

    // Skip whitespaces
    while (isspace(*reader)) reader++;

    // Check for end of file/input
    if (*reader == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }

    // Code to check for symbols
    switch (*reader) {
        case ',':
        case ':':
        case ';':
        case '(':
        case ')':
        case '+':
        case '-':
        case '*':
        case '/':
            token.type = TOKEN_SYMBOL;
            snprintf(token.value, sizeof(token.value), "%c", *reader);
            reader++;
            break;
        default:
            // Check for keywords and identifiers
            if (isalpha(*reader)) {
                int length = 0;
                while (isalpha(reader[length])) length++;
                strncpy(token.value, reader, length);
                token.value[length] = '\0';
                reader += length;

                if (isKeyword(token.value)) {
                    token.type = TOKEN_KEYWORD;
                } else {
                    token.type = TOKEN_IDENTIFIER;
                }
            }
            // Check for literals (this part is simplistic and needs to be expanded)
            else if (isdigit(*reader) || *reader == '\"' || *reader == '\'') {
                int length = 0;
                char endChar = *reader;
                reader++;  // Skip the beginning quote or digit

                if (endChar == '\"' || endChar == '\'') {
                    // String literal
                    while (reader[length] && reader[length] != endChar) length++;
                } else {
                    // Numeric literal
                    while (isdigit(reader[length])) length++;
                }

                strncpy(token.value, reader, length);
                token.value[length] = '\0';
                reader += length + (endChar == '\"' || endChar == '\'' ? 1 : 0); // Skip the ending quote

                token.type = TOKEN_LITERAL;
            }
            break;
    }

    *input = reader;
    return token;
}
