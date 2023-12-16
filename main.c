#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    NODE_TYPE_RESERVE_ROOM,
    NODE_TYPE_UPDATE_RESERVATION,
    NODE_TYPE_CANCEL_RESERVATION,
    NODE_TYPE_CHECK_ROOM_AVAILABILITY,
    NODE_TYPE_CREATE_GUEST_PROFILE,
    NODE_TYPE_GENERATE_INVOICE,
    NODE_TYPE_REQUEST_SERVICE,
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char* value;
    struct ASTNode** children;
    int children_count;
} ASTNode;

ASTNode* createASTNode(NodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->value = NULL;
    node->children = NULL;
    node->children_count = 0;
    return node;
}

void freeASTNode(ASTNode* node) {
    if (node != NULL) {
        free(node->value);
        for (int i = 0; i < node->children_count; ++i) {
            freeASTNode(node->children[i]);
        }
        free(node->children);
        free(node);
    }
}

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
    free(token->value);
    free(token);

    // Consume 'room' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'room'");
    free(token->value);
    free(token);

    // Consume room type
    token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected room type");
    char* roomType = strdup(token->value);
    free(token->value);
    free(token);

    // Consume 'from' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'from'");
    free(token->value);
    free(token);

    // Consume check-in date
    token = consume(lexer, TOKEN_TYPE_LITERAL, "Expected check-in date");
    char* checkInDate = strdup(token->value);
    free(token->value);
    free(token);

    // Consume 'to' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'to'");
    free(token->value);
    free(token);

    // Consume check-out date
    token = consume(lexer, TOKEN_TYPE_LITERAL, "Expected check-out date");
    char* checkOutDate = strdup(token->value);
    free(token->value);
    free(token);

    // Consume 'for' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'for'");
    free(token->value);
    free(token);

    // Consume guest name
    token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected guest name");
    char* guestName = strdup(token->value);
    free(token->value);
    free(token);

    // Here you would typically construct an AST node for this statement
    // For simplicity, we'll just print it
    printf("Parsed a reserve room statement for room type %s from %s to %s for guest %s\n", 
           roomType, checkInDate, checkOutDate, guestName);

    // Free the duplicated strings after constructing the AST node
    free(roomType);
    free(checkInDate);
    free(checkOutDate);
    free(guestName);
}


void parseUpdateReservation(Lexer* lexer) {
    // Consume 'update' keyword
    Token* token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'update'");
    free(token->value);
    free(token);

    // Consume 'reservation' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'reservation'");
    free(token->value);
    free(token);

    // Consume reservation ID
    token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected reservation ID");
    char* reservationID = strdup(token->value);
    free(token->value);
    free(token);

    // Consume 'with' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'with'");
    free(token->value);
    free(token);

    // Consume attribute to update
    token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected attribute to update");
    char* attribute = strdup(token->value);
    free(token->value);
    free(token);

    // Consume new value for the attribute
    token = consume(lexer, TOKEN_TYPE_LITERAL, "Expected new value for attribute");
    char* newValue = strdup(token->value);
    free(token->value);
    free(token);

    // Here you would typically construct an AST node for this statement
    // For simplicity, we'll just print it
    printf("Parsed an update reservation statement for reservation ID %s, updating %s to %s\n", reservationID, attribute, newValue);

    // Free the duplicated strings after constructing the AST node
    free(reservationID);
    free(attribute);
    free(newValue);
}

void parseCancelReservation(Lexer* lexer) {
    // Consume 'cancel' keyword
    Token* token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'cancel'");
    free(token->value);
    free(token);

    // Consume 'reservation' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'reservation'");
    free(token->value);
    free(token);

    // Consume reservation ID
    token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected reservation ID");
    char* reservationID = strdup(token->value);
    free(token->value);
    free(token);

    // Here you would typically construct an AST node for this statement
    // For simplicity, we'll just print it
    printf("Parsed a cancel reservation statement for reservation ID %s\n", reservationID);

    // Free the duplicated string after constructing the AST node
    free(reservationID);
}

void parseCheckRoomAvailability(Lexer* lexer) {
    // Consume 'check' keyword
    Token* token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'check'");
    free(token->value);
    free(token);

    // Consume 'room' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'room'");
    free(token->value);
    free(token);

    // Consume 'availability' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'availability'");
    free(token->value);
    free(token);

    // Consume room type
    token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected room type");
    char* roomType = strdup(token->value);
    free(token->value);
    free(token);

    // Consume 'from' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'from'");
    free(token->value);
    free(token);

    // Consume check-in date
    token = consume(lexer, TOKEN_TYPE_LITERAL, "Expected check-in date");
    char* checkInDate = strdup(token->value);
    free(token->value);
    free(token);

    // Consume 'to' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'to'");
    free(token->value);
    free(token);

    // Consume check-out date
    token = consume(lexer, TOKEN_TYPE_LITERAL, "Expected check-out date");
    char* checkOutDate = strdup(token->value);
    free(token->value);
    free(token);

    // Here you would typically construct an AST node for this statement
    // For simplicity, we'll just print it
    printf("Parsed a check room availability statement for room type %s from %s to %s\n", roomType, checkInDate, checkOutDate);

    // Free the duplicated strings after constructing the AST node
    free(roomType);
    free(checkInDate);
    free(checkOutDate);
}

void parseCreateGuestProfile(Lexer* lexer) {
    // Consume 'create' keyword
    Token* token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'create'");
    free(token->value);
    free(token);

    // Consume 'guest' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'guest'");
    free(token->value);
    free(token);

    // Consume 'profile' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'profile'");
    free(token->value);
    free(token);

    // Consume 'with' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'with'");
    free(token->value);
    free(token);

    // Consume 'name' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'name'");
    free(token->value);
    free(token);

    // Consume guest name
    token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected guest name");
    char* guestName = strdup(token->value);
    free(token->value);
    free(token);

    // Consume 'and' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'and'");
    free(token->value);
    free(token);

    // Consume 'contact' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'contact'");
    free(token->value);
    free(token);

    // Consume guest contact
    token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected guest contact");
    char* guestContact = strdup(token->value);
    free(token->value);
    free(token);

    // Here you would typically construct an AST node for this statement
    // For simplicity, we'll just print it
    printf("Parsed a create guest profile statement for guest name %s with contact %s\n", guestName, guestContact);

    // Free the duplicated strings after constructing the AST node
    free(guestName);
    free(guestContact);
}

void parseGenerateInvoice(Lexer* lexer) {
    // Consume 'generate' keyword
    Token* token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'generate'");
    free(token->value);
    free(token);

    // Consume 'invoice' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'invoice'");
    free(token->value);
    free(token);

    // Consume 'for' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'for'");
    free(token->value);
    free(token);

    // Consume 'reservation' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'reservation'");
    free(token->value);
    free(token);

    // Consume reservation ID
    token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected reservation ID");
    char* reservationID = strdup(token->value);
    free(token->value);
    free(token);

    // Here you would typically construct an AST node for this statement
    // For simplicity, we'll just print it
    printf("Parsed a generate invoice statement for reservation ID %s\n", reservationID);

    // Free the duplicated string after constructing the AST node
    free(reservationID);
}

void parseRequestService(Lexer* lexer) {
    // Consume 'request' keyword
    Token* token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'request'");
    free(token->value);
    free(token);

    // Consume service name
    token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected service name");
    char* serviceName = strdup(token->value);
    free(token->value);
    free(token);

    // Consume 'for' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'for'");
    free(token->value);
    free(token);

    // Consume 'reservation' keyword
    token = consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'reservation'");
    free(token->value);
    free(token);

    // Consume reservation ID
    token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected reservation ID");
    char* reservationID = strdup(token->value);
    free(token->value);
    free(token);

    // Here you would typically construct an AST node for this statement
    // For simplicity, we'll just print it
    printf("Parsed a request service statement for service '%s' on reservation ID %s\n", serviceName, reservationID);

    // Free the duplicated strings after constructing the AST node
    free(serviceName);
    free(reservationID);
}

void parseStatement(Lexer* lexer) {
    Token* token = nextToken(lexer);

    if (matchToken(token, "reserve")) {
        parseReserveRoom(lexer);
    } else if (matchToken(token, "update")) {
        parseUpdateReservation(lexer);
    } else if (matchToken(token, "cancel")) {
        parseCancelReservation(lexer);
    } else if (matchToken(token, "check")) {
        parseCheckRoomAvailability(lexer);
    } else if (matchToken(token, "create")) {
        parseCreateGuestProfile(lexer);
    } else if (matchToken(token, "generate")) {
        parseGenerateInvoice(lexer);
    } else if (matchToken(token, "request")) {
        parseRequestService(lexer);
    } else {
        fprintf(stderr, "Error: Unexpected statement '%s' at line %d\n", token->value, lexer->line);
        free(token->value);
        free(token);
        exit(1);
    }

    free(token->value);
    free(token);
}


// Main function to demonstrate the parser
int main() {
    const char* sourceCode = "reserve room deluxe from 2022-01-01 to 2022-01-05 for John Doe";
    
    Lexer lexer;
    initLexer(&lexer, sourceCode);

    parseStatement(&lexer);

    return 0;
}




// semantic analysis functions:

typedef struct SymbolTable {
    char** identifiers;
    int count;
    int capacity;
} SymbolTable;

SymbolTable* createSymbolTable() {
    SymbolTable* table = (SymbolTable*)malloc(sizeof(SymbolTable));
    table->identifiers = NULL;
    table->count = 0;
    table->capacity = 0;
    return table;
}

void freeSymbolTable(SymbolTable* table) {
    for (int i = 0; i < table->count; i++) {
        free(table->identifiers[i]);
    }
    free(table->identifiers);
    free(table);
}

int symbolTableContains(SymbolTable* table, const char* identifier) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->identifiers[i], identifier) == 0) {
            return 1;
        }
    }
    return 0;
}

void symbolTableAdd(SymbolTable* table, const char* identifier) {
    if (symbolTableContains(table, identifier)) {
        fprintf(stderr, "Semantic Error: Duplicate identifier '%s'\n", identifier);
        exit(1);
    }
    if (table->count == table->capacity) {
        table->capacity = table->capacity < 8 ? 8 : table->capacity * 2;
        table->identifiers = (char**)realloc(table->identifiers, table->capacity * sizeof(char*));
    }
    table->identifiers[table->count++] = strdup(identifier);
}

int isValidRoomType(const char* roomType) {
    // Assuming there's a function that checks if the room type is valid.
    // For demonstration, let's say "deluxe" and "standard" are valid room types.
    return strcmp(roomType, "deluxe") == 0 || strcmp(roomType, "standard") == 0;
}

int isValidDate(const char* date) {
    // Simple check for date format YYYY-MM-DD
    return strlen(date) == 10 && date[4] == '-' && date[7] == '-';
}

int isAfter(const char* checkInDate, const char* checkOutDate) {
    // This function would compare the dates to ensure check-out is after check-in.
    // For simplicity, let's assume a function that correctly compares the date strings.
    // ...

    return 1; // Assume it returns true for demonstration purposes.
}

void checkReserveRoom(ASTNode* node, SymbolTable* table) {
    if (!isValidRoomType(node->children[0]->value)) {
        fprintf(stderr, "Semantic Error: Invalid room type '%s'\n", node->children[0]->value);
        exit(1);
    }

    if (!isValidDate(node->children[1]->value) || !isValidDate(node->children[2]->value)) {
        fprintf(stderr, "Semantic Error: Invalid date format\n");
        exit(1);
    }

    if (!isAfter(node->children[1]->value, node->children[2]->value)) {
        fprintf(stderr, "Semantic Error: Check-out date must be after check-in date\n");
        exit(1);
    }

    if (strlen(node->children[3]->value) == 0) {
        fprintf(stderr, "Semantic Error: Guest name cannot be empty\n");
        exit(1);
    }
}

void checkUpdateReservation(ASTNode* node, SymbolTable* table) {
    if (!symbolTableContains(table, node->children[0]->value)) {
        fprintf(stderr, "Semantic Error: Reservation ID '%s' not found\n", node->children[0]->value);
        exit(1);
    }

    // Let's say the only updatable attribute is "roomType"
    if (strcmp(node->children[1]->value, "roomType") != 0) {
        fprintf(stderr, "Semantic Error: Can only update 'roomType'\n");
        exit(1);
    }

    if (!isValidRoomType(node->children[2]->value)) {
        fprintf(stderr, "Semantic Error: Invalid new room type '%s'\n", node->children[2]->value);
        exit(1);
    }
}

void performSemanticAnalysis(ASTNode* node, SymbolTable* table) {
    switch (node->type) {
        case NODE_TYPE_RESERVE_ROOM:
            checkReserveRoom(node, table);
            break;
        case NODE_TYPE_UPDATE_RESERVATION:
            checkUpdateReservation(node, table);
            break;
        // ... handle other cases
    }

    // Recursively call performSemanticAnalysis for all children of the current node
    for (int i = 0; i < node->children_count; i++) {
        performSemanticAnalysis(node->children[i], table);
    }
}


int main() {
    // ... Lexer and parser setup ...

    // After constructing the AST
    ASTNode* ast = NULL; // Constructed from parser
    SymbolTable* table = createSymbolTable();

    // Perform semantic analysis
    performSemanticAnalysis(ast, table);

    // Proceed with code generation or interpretation...

    freeASTNode(ast);
    freeSymbolTable(table);
    return 0;
}


// code generator/interpreter

void interpretAST(ASTNode* node) {
    if (node == NULL) return;

    switch (node->type) {
        case NODE_TYPE_RESERVE_ROOM:
            interpretReserveRoom(node);
            break;
        case NODE_TYPE_UPDATE_RESERVATION:
            interpretUpdateReservation(node);
            break;
        case NODE_TYPE_CANCEL_RESERVATION:
            interpretCancelReservation(node);
            break;
        // ... other cases ...
        default:
            fprintf(stderr, "Unknown node type in interpreter\n");
            exit(1);
    }

    for (int i = 0; i < node->children_count; i++) {
        interpretAST(node->children[i]);
    }
}

// function speficific nodes

void interpretReserveRoom(ASTNode* node) {
    char* roomType = node->children[0]->value;
    char* checkInDate = node->children[1]->value;
    char* checkOutDate = node->children[2]->value;
    char* guestName = node->children[3]->value;

    // Here, you would have the logic to reserve a room in your system
    printf("Reserving a %s room from %s to %s for %s\n", roomType, checkInDate, checkOutDate, guestName);
}

void interpretUpdateReservation(ASTNode* node) {
    char* reservationID = node->children[0]->value;
    char* attribute = node->children[1]->value;
    char* newValue = node->children[2]->value;

    // Logic to update a reservation
    printf("Updating reservation %s: %s to %s\n", reservationID, attribute, newValue);
}

void interpretCancelReservation(ASTNode* node) {
    char* reservationID = node->children[0]->value;

    // Logic to cancel a reservation
    printf("Cancelling reservation with ID %s\n", reservationID);
}

void interpretCheckRoomAvailability(ASTNode* node) {
    char* roomType = node->children[0]->value;
    char* checkInDate = node->children[1]->value;
    char* checkOutDate = node->children[2]->value;

    // Here, you would check the availability of the room in your system
    printf("Checking availability for a %s room from %s to %s\n", roomType, checkInDate, checkOutDate);

    // In a real system, you would query the database and return the result
}

void interpretCreateGuestProfile(ASTNode* node) {
    char* guestName = node->children[0]->value;
    char* guestContact = node->children[1]->value;

    // Logic to create a guest profile
    printf("Creating guest profile for %s with contact %s\n", guestName, guestContact);

    // In a real system, you would add this information to your database
}


void interpretGenerateInvoice(ASTNode* node) {
    char* reservationID = node->children[0]->value;

    // Logic to generate an invoice
    printf("Generating invoice for reservation ID %s\n", reservationID);

    // In a real system, this would involve calculating costs and creating an invoice record
}

void interpretRequestService(ASTNode* node) {
    char* serviceName = node->children[0]->value;
    char* reservationID = node->children[1]->value;

    // Logic to request a service
    printf("Requesting service '%s' for reservation ID %s\n", serviceName, reservationID);

    // In a real system, this would involve adding a service request to your system linked to the reservation
}



