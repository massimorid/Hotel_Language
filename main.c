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
ASTNode* parseReserveRoom(Lexer* lexer) {
    ASTNode* reserveNode = createASTNode(NODE_TYPE_RESERVE_ROOM);

    // Assume all fields (room type, check-in date, etc.) are stored as children
    reserveNode->children = (ASTNode**)malloc(sizeof(ASTNode*) * 4); // Four fields for reserve room
    reserveNode->children_count = 4;

    // Consume 'reserve' and 'room' keywords (no need to create AST nodes for these)
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'reserve'");
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'room'");

    // Consume and process room type
    Token* token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected room type");
    ASTNode* roomTypeNode = createASTNode(NODE_TYPE_RESERVE_ROOM);
    roomTypeNode->value = strdup(token->value);
    reserveNode->children[0] = roomTypeNode;
    free(token->value);
    free(token);

    // Consume 'from' keyword
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'from'");

    // Consume and process check-in date
    token = consume(lexer, TOKEN_TYPE_LITERAL, "Expected check-in date");
    ASTNode* checkInDateNode = createASTNode(NODE_TYPE_RESERVE_ROOM);
    checkInDateNode->value = strdup(token->value);
    reserveNode->children[1] = checkInDateNode;
    free(token->value);
    free(token);

    // Consume 'to' keyword
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'to'");

    // Consume and process check-out date
    token = consume(lexer, TOKEN_TYPE_LITERAL, "Expected check-out date");
    ASTNode* checkOutDateNode = createASTNode(NODE_TYPE_RESERVE_ROOM);
    checkOutDateNode->value = strdup(token->value);
    reserveNode->children[2] = checkOutDateNode;
    free(token->value);
    free(token);

    // Consume 'for' keyword
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'for'");

    // Consume and process guest name
    token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected guest name");
    ASTNode* guestNameNode = createASTNode(NODE_TYPE_RESERVE_ROOM);
    guestNameNode->value = strdup(token->value);
    reserveNode->children[3] = guestNameNode;
    free(token->value);
    free(token);

    return reserveNode;
}


}
ASTNode* parseUpdateReservation(Lexer* lexer) {
    ASTNode* updateNode = createASTNode(NODE_TYPE_UPDATE_RESERVATION);

    // Assume all fields (reservation ID, attribute, new value) are stored as children
    updateNode->children = (ASTNode**)malloc(sizeof(ASTNode*) * 3); // Three fields for update reservation
    updateNode->children_count = 3;

    // Consume 'update' and 'reservation' keywords (no need to create AST nodes for these)
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'update'");
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'reservation'");

    // Consume and process reservation ID
    Token* token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected reservation ID");
    ASTNode* reservationIDNode = createASTNode(NODE_TYPE_UPDATE_RESERVATION);
    reservationIDNode->value = strdup(token->value);
    updateNode->children[0] = reservationIDNode;
    free(token->value);
    free(token);

    // Consume 'with' keyword
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'with'");

    // Consume and process attribute to update
    token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected attribute to update");
    ASTNode* attributeNode = createASTNode(NODE_TYPE_UPDATE_RESERVATION);
    attributeNode->value = strdup(token->value);
    updateNode->children[1] = attributeNode;
    free(token->value);
    free(token);

    // Consume and process new value for the attribute
    token = consume(lexer, TOKEN_TYPE_LITERAL, "Expected new value for attribute");
    ASTNode* newValueNode = createASTNode(NODE_TYPE_UPDATE_RESERVATION);
    newValueNode->value = strdup(token->value);
    updateNode->children[2] = newValueNode;
    free(token->value);
    free(token);

    return updateNode;
}

ASTNode* parseCancelReservation(Lexer* lexer) {
    ASTNode* cancelNode = createASTNode(NODE_TYPE_CANCEL_RESERVATION);

    // There is only one field (reservation ID) for cancel reservation
    cancelNode->children = (ASTNode**)malloc(sizeof(ASTNode*)); // One field for reservation ID
    cancelNode->children_count = 1;

    // Consume 'cancel' and 'reservation' keywords (no need to create AST nodes for these)
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'cancel'");
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'reservation'");

    // Consume and process reservation ID
    Token* token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected reservation ID");
    ASTNode* reservationIDNode = createASTNode(NODE_TYPE_CANCEL_RESERVATION);
    reservationIDNode->value = strdup(token->value);
    cancelNode->children[0] = reservationIDNode;
    free(token->value);
    free(token);

    return cancelNode;
}

ASTNode* parseCheckRoomAvailability(Lexer* lexer) {
    ASTNode* checkRoomAvailabilityNode = createASTNode(NODE_TYPE_CHECK_ROOM_AVAILABILITY);

    // There are three fields (room type, check-in date, check-out date)
    checkRoomAvailabilityNode->children = (ASTNode**)malloc(sizeof(ASTNode*) * 3);
    checkRoomAvailabilityNode->children_count = 3;

    // Consume 'check', 'room', and 'availability' keywords
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'check'");
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'room'");
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'availability'");

    // Consume and process room type
    Token* token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected room type");
    ASTNode* roomTypeNode = createASTNode(NODE_TYPE_CHECK_ROOM_AVAILABILITY);
    roomTypeNode->value = strdup(token->value);
    checkRoomAvailabilityNode->children[0] = roomTypeNode;
    free(token->value);
    free(token);

    // Consume 'from' keyword
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'from'");

    // Consume and process check-in date
    token = consume(lexer, TOKEN_TYPE_LITERAL, "Expected check-in date");
    ASTNode* checkInDateNode = createASTNode(NODE_TYPE_CHECK_ROOM_AVAILABILITY);
    checkInDateNode->value = strdup(token->value);
    checkRoomAvailabilityNode->children[1] = checkInDateNode;
    free(token->value);
    free(token);

    // Consume 'to' keyword
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'to'");

    // Consume and process check-out date
    token = consume(lexer, TOKEN_TYPE_LITERAL, "Expected check-out date");
    ASTNode* checkOutDateNode = createASTNode(NODE_TYPE_CHECK_ROOM_AVAILABILITY);
    checkOutDateNode->value = strdup(token->value);
    checkRoomAvailabilityNode->children[2] = checkOutDateNode;
    free(token->value);
    free(token);

    return checkRoomAvailabilityNode;
}


ASTNode* parseCreateGuestProfile(Lexer* lexer) {
    ASTNode* createGuestProfileNode = createASTNode(NODE_TYPE_CREATE_GUEST_PROFILE);

    // Assume two fields (guest name, guest contact) are stored as children
    createGuestProfileNode->children = (ASTNode**)malloc(sizeof(ASTNode*) * 2); // Two fields: name and contact
    createGuestProfileNode->children_count = 2;

    // Consume 'create', 'guest', 'profile', 'with', 'name', 'and', 'contact' keywords
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'create'");
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'guest'");
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'profile'");
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'with'");
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'name'");
    
    // Consume and process guest name
    Token* token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected guest name");
    ASTNode* guestNameNode = createASTNode(NODE_TYPE_CREATE_GUEST_PROFILE);
    guestNameNode->value = strdup(token->value);
    createGuestProfileNode->children[0] = guestNameNode;
    free(token->value);
    free(token);

    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'and'");
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'contact'");

    // Consume and process guest contact
    token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected guest contact");
    ASTNode* guestContactNode = createASTNode(NODE_TYPE_CREATE_GUEST_PROFILE);
    guestContactNode->value = strdup(token->value);
    createGuestProfileNode->children[1] = guestContactNode;
    free(token->value);
    free(token);

    return createGuestProfileNode;
}
ASTNode* parseGenerateInvoice(Lexer* lexer) {
    ASTNode* generateInvoiceNode = createASTNode(NODE_TYPE_GENERATE_INVOICE);

    // There is only one field (reservation ID) for generate invoice
    generateInvoiceNode->children = (ASTNode**)malloc(sizeof(ASTNode*)); // One field for reservation ID
    generateInvoiceNode->children_count = 1;

    // Consume 'generate', 'invoice', 'for', and 'reservation' keywords
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'generate'");
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'invoice'");
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'for'");
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'reservation'");

    // Consume and process reservation ID
    Token* token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected reservation ID");
    ASTNode* reservationIDNode = createASTNode(NODE_TYPE_GENERATE_INVOICE);
    reservationIDNode->value = strdup(token->value);
    generateInvoiceNode->children[0] = reservationIDNode;
    free(token->value);
    free(token);

    return generateInvoiceNode;
}

ASTNode* parseRequestService(Lexer* lexer) {
    ASTNode* requestServiceNode = createASTNode(NODE_TYPE_REQUEST_SERVICE);

    // There are two fields (service name, reservation ID) for request service
    requestServiceNode->children = (ASTNode**)malloc(sizeof(ASTNode*) * 2); // Two fields: service name and reservation ID
    requestServiceNode->children_count = 2;

    // Consume 'request' keyword (no need to create an AST node for this)
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'request'");

    // Consume and process service name
    Token* token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected service name");
    ASTNode* serviceNameNode = createASTNode(NODE_TYPE_REQUEST_SERVICE);
    serviceNameNode->value = strdup(token->value);
    requestServiceNode->children[0] = serviceNameNode;
    free(token->value);
    free(token);

    // Consume 'for' and 'reservation' keywords
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'for'");
    consume(lexer, TOKEN_TYPE_KEYWORD, "Expected 'reservation'");

    // Consume and process reservation ID
    token = consume(lexer, TOKEN_TYPE_IDENTIFIER, "Expected reservation ID");
    ASTNode* reservationIDNode = createASTNode(NODE_TYPE_REQUEST_SERVICE);
    reservationIDNode->value = strdup(token->value);
    requestServiceNode->children[1] = reservationIDNode;
    free(token->value);
    free(token);

    return requestServiceNode;
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


// semantic analysis functions

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
        case NODE_TYPE_CANCEL_RESERVATION:
            checkCancelReservation(node, table);
            break;
        case NODE_TYPE_CHECK_ROOM_AVAILABILITY:
            checkCheckRoomAvailability(node, table);
            break;
        case NODE_TYPE_CREATE_GUEST_PROFILE:
            checkCreateGuestProfile(node, table);
            break;
        case NODE_TYPE_GENERATE_INVOICE:
            checkGenerateInvoice(node, table);
            break;
        case NODE_TYPE_REQUEST_SERVICE:
            checkRequestService(node, table);
            break;
    

    // Recursively call performSemanticAnalysis for all children of the current node
    for (int i = 0; i < node->children_count; i++) {
        performSemanticAnalysis(node->children[i], table);
    }
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
        case NODE_TYPE_CHECK_ROOM_AVAILABILITY:
            interpretCheckRoomAvailability(node);
            break;
        case NODE_TYPE_CREATE_GUEST_PROFILE:
            interpretCreateGuestProfile(node);
            break;
        case NODE_TYPE_GENERATE_INVOICE:
            interpretGenerateInvoice(node);
            break;
        case NODE_TYPE_REQUEST_SERVICE:
            interpretRequestService(node);
            break;



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
