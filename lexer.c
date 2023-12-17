#include "lexer.h"





const char *keywords[] = {
    "reserve", "update", "cancel", "check", "create", "generate", "request", NULL
};

int isKeyword(const char *str) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

Token getNextToken(const char **input) {
    const char *reader = *input;
    Token token;
    token.type = TOKEN_UNKNOWN;
    strcpy(token.value, "");

    while (isspace(*reader)) reader++;

    if (*reader == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }

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
            else if (isdigit(*reader) || *reader == '\"' || *reader == '\'') {
                int length = 0;
                char endChar = *reader;
                reader++;  

                if (endChar == '\"' || endChar == '\'') {
                    while (reader[length] && reader[length] != endChar) length++;
                } else {
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
