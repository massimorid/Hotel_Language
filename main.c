#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
typedef enum {
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_LITERAL,
    TOKEN_TYPE_SYMBOL,
    TOKEN_TYPE_EOF,
    TOKEN_TYPE_UNKNOWN
} TokenType;

// Token definition
typedef struct {
    TokenType type;
    char* value;
} Token;

// List of keywords
const char* keywords[] = {
    "reserve", "update", "cancel", "check", "create", "generate", "request"
};

// Check if a string is a keyword
int isKeyword(const char* str) {
    for (int i = 0; i < sizeof(keywords) / sizeof(char*); i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

// Lexer state
typedef struct {
    const char* start;
    const char* current;
    int line;
} Lexer;

// Initialize the lexer
void initLexer(Lexer* lexer, const char* source) {
    lexer->start = source;
    lexer->current = source;
    lexer->line = 1;
}

// Check if we've reached the end of the source code
int isAtEnd(Lexer* lexer) {
    return *lexer->current == '\0';
}

// Advance the lexer and return the next character
char advance(Lexer* lexer) {
    lexer->current++;
    return lexer->current[-1];
}

// Peek at the current character without consuming it
char peek(Lexer* lexer) {
    return *lexer->current;
}

// Create a new token
Token* newToken(TokenType type, const char* start, int length) {
    Token* token = (Token*)malloc(sizeof(Token));
    token->type = type;
    token->value = (char*)malloc(length + 1);
    memcpy(token->value, start, length);
    token->value[length] = '\0';
    return token;
}

// Lexical analysis: get the next token
Token* nextToken(Lexer* lexer) {
    lexer->start = lexer->current;

    if (isAtEnd(lexer)) {
        return newToken(TOKEN_TYPE_EOF, lexer->start, 0);
    }

    char c = advance(lexer);

    if (isalpha(c)) {
        while (isalnum(peek(lexer))) advance(lexer);
        int length = lexer->current - lexer->start;
        if (isKeyword(lexer->start)) {
            return newToken(TOKEN_TYPE_KEYWORD, lexer->start, length);
        }
        return newToken(TOKEN_TYPE_IDENTIFIER, lexer->start, length);
    }

    if (isdigit(c)) {
        while (isdigit(peek(lexer))) advance(lexer);
        return newToken(TOKEN_TYPE_LITERAL, lexer->start, lexer->current - lexer->start);
    }

    switch (c) {
        case '(': return newToken(TOKEN_TYPE_SYMBOL, lexer->start, 1);
        // Add other symbols as necessary
    }

    return newToken(TOKEN_TYPE_UNKNOWN, lexer->start, 1);
}

// Main function to demonstrate the lexer
int main() {
    const char* sourceCode = "reserve room deluxe from 2022-01-01 to 2022-01-05 for John Doe";
    
    Lexer lexer;
    initLexer(&lexer, sourceCode);
    
    Token* token;
    do {
        token = nextToken(&lexer);
        printf("Token: %s\n", token->value);
        free(token->value);
        free(token);
    } while (token->type != TOKEN_TYPE_EOF);

    return 0;
}



// Function signatures for parsing different statements
void parseReserveRoom(Lexer* lexer);
void parseUpdateReservation(Lexer* lexer);
void parseCancelReservation(Lexer* lexer);
void parseCheckRoomAvailability(Lexer* lexer);
void parseCreateGuestProfile(Lexer* lexer);
void parseGenerateInvoice(Lexer* lexer);
void parseRequestService(Lexer* lexer);
void parseStatement(Lexer* lexer);

// Utility function to compare token value with expected string
int matchToken(Token* token, const char* expected) {
    if (token->type == TOKEN_TYPE_KEYWORD && strcmp(token->value, expected) == 0) {
        return 1;
    }
    return 0;
}

// Utility function to check if token type matches expected type
int checkTokenType(Token* token, TokenType expectedType) {
    return token->type == expectedType;
}

// Consume a token and return it if it matches the expected type
Token* consume(Lexer* lexer, TokenType type, const char* message) {
    Token* token = nextToken(lexer);
    if (token->type != type) {
        fprintf(stderr, "Error: %s\n", message);
        exit(1);
    }
    return token;
}

// The entry point to parse a reservation statement
void parseReserveRoom(Lexer* lexer) {
    // Consume 'reserve' keyword
    Token* token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'reserve'");

    // Consume 'room' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'room'");

    // Consume room type
    token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected room type");

    // Consume 'from' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'from'");

    // Consume check-in date
    token = consume(lexer, TOKEN_TYPE_LITERAL, "Expected check-in date");

    // Consume 'to' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'to'");

    // Consume check-out date
    token = consume(lexer, TOKEN_TYPE_LITERAL, "Expected check-out date");

    // Consume 'for' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'for'");

    // Consume guest name
    token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected guest name");

    // Here you would typically construct an AST node for this statement
    // For simplicity, we'll just print it
    printf("Parsed a reservation statement\n");

    // Normally, you would also free the tokens after using them
}

// Similarly implement other parse functions...
// ...

// Parsing a statement by looking at the current token
void parseStatement(Lexer* lexer) {
    Token* token = nextToken(lexer);

    if (matchToken(token, "reserve")) {
        parseReserveRoom(lexer);
    }
    // Add else if branches for other statement types
    // ...
    else {
        fprintf(stderr, "Error: Unexpected statement\n");
        exit(1);
    }
}

// Main function to demonstrate the parser
int main() {
    const char* sourceCode = "reserve room deluxe from 2022-01-01 to 2022-01-05 for John Doe";
    
    Lexer lexer;
    initLexer(&lexer, sourceCode);

    parseStatement(&lexer);

    return 0;
}
