#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Enumeration for token types
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_LITERAL,
    TOKEN_SYMBOL,
    TOKEN_UNKNOWN,
    TOKEN_EOF
} TokenType;

// Structure for a token
typedef struct {
    TokenType type;
    char value[100];
} Token;

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

    // Check for symbols
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

typedef struct ASTNode {
enum { RESERVATION_NODE, UPDATE_NODE, CANCEL_NODE, CHECK_NODE, CREATE_GUEST_PROFILE_NODE, GENERATE_INVOICE_NODE, REQUEST_SERVICE_NODE } type;
} ASTNode;

typedef struct {
    ASTNode base;
    char *roomType;
    char *checkInDate;
    char *checkOutDate;
    char *guestName;
} ReservationNode;

typedef struct {
    ASTNode base;
    char *reservationId;
    char *attribute;
    char *value;
} UpdateNode;

typedef struct {
    ASTNode base;
    char *reservationId;
} CancelNode;

typedef struct {
    ASTNode base;
    char *roomType;
    char *checkInDate;
    char *checkOutDate;
} CheckNode;

typedef struct {
    ASTNode base;
    char *guestName;
    char *guestContact;
} CreateGuestProfileNode;

typedef struct {
    ASTNode base;
    char *reservationId;
} GenerateInvoiceNode;

typedef struct {
    ASTNode base;
    char *serviceName;
    char *reservationId;
} RequestServiceNode;



ReservationNode* parseReservation(const char **input);
UpdateNode* parseUpdate(const char **input);
CancelNode* parseCancel(const char **input);
CheckNode* parseCheck(const char **input);
CreateGuestProfileNode* parseCreateGuestProfile(const char **input);
GenerateInvoiceNode* parseGenerateInvoice(const char **input);
RequestServiceNode* parseRequestService(const char **input);
ASTNode* parseStatement(const char **input); // This will return a generic ASTNode



ReservationNode* parseReservation(const char **input) {
    // Initialize node
    ReservationNode *node = malloc(sizeof(ReservationNode));
    if (!node) {
        // Handle memory allocation failure
        return NULL;
    }
    node->base.type = RESERVATION_NODE;
    node->roomType = NULL;
    node->checkInDate = NULL;
    node->checkOutDate = NULL;
    node->guestName = NULL;
    Token currentToken;
    // 'reserve' keyword is already matched if we are in this function
    currentToken = getNextToken(input); // Should be 'room'
    if (strcmp(currentToken.value, "room") != 0) goto error;
    currentToken = getNextToken(input); // Should be <room_type>
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->roomType = strdup(currentToken.value);
    currentToken = getNextToken(input); // Should be 'from'
    if (strcmp(currentToken.value, "from") != 0) goto error;
    currentToken = getNextToken(input); // Should be <check-in_date>
    if (currentToken.type != TOKEN_LITERAL) goto error;
    node->checkInDate = strdup(currentToken.value);
    currentToken = getNextToken(input); // Should be 'to'
    if (strcmp(currentToken.value, "to") != 0) goto error;
    currentToken = getNextToken(input); // Should be <check-out_date>
    if (currentToken.type != TOKEN_LITERAL) goto error;
    node->checkOutDate = strdup(currentToken.value);
    currentToken = getNextToken(input); // Should be 'for'
    if (strcmp(currentToken.value, "for") != 0) goto error;
    currentToken = getNextToken(input); // Should be <guest_name>
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->guestName = strdup(currentToken.value);
    return node; // Successful parsing

    

error:
    free(node); // Clean up in case of error
    return NULL;
}



UpdateNode* parseUpdate(const char **input) {
    // Initialize node
    UpdateNode *node = malloc(sizeof(UpdateNode));
    if (!node) {
        // Handle memory allocation failure
        return NULL;
    }
    node->base.type = UPDATE_NODE;
    node->reservationId = NULL;
    node->attribute = NULL;
    node->value = NULL;
    Token currentToken;
    // 'update' keyword is already matched if we are in this function
    currentToken = getNextToken(input); // Should be 'reservation'
    if (strcmp(currentToken.value, "reservation") != 0) goto error;
    currentToken = getNextToken(input); // Should be <reservation_id>
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->reservationId = strdup(currentToken.value);
    currentToken = getNextToken(input); // Should be 'with'
    if (strcmp(currentToken.value, "with") != 0) goto error;
    currentToken = getNextToken(input); // Should be <attribute>
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->attribute = strdup(currentToken.value);
    currentToken = getNextToken(input); // Should be <value>
    // <value> can be a string literal, date literal, or numeric literal based on the attribute
    if (currentToken.type != TOKEN_LITERAL && currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->value = strdup(currentToken.value);
    return node; // Successful parsing

error:
    // Free allocated strings if any
    if (node->reservationId) free(node->reservationId);
    if (node->attribute) free(node->attribute);
    if (node->value) free(node->value);
    // Free the node itself
    free(node);
    return NULL;
}


CancelNode* parseCancel(const char **input) {
    // Initialize node
    CancelNode *node = malloc(sizeof(CancelNode));
    if (!node) {
        // Handle memory allocation failure
        return NULL;
    }
    node->base.type = CANCEL_NODE;
    node->reservationId = NULL;
    Token currentToken;
    // 'cancel' keyword is already matched if we are in this function
    currentToken = getNextToken(input); // Should be 'reservation'
    if (strcmp(currentToken.value, "reservation") != 0) goto error;
    currentToken = getNextToken(input); // Should be <reservation_id>
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->reservationId = strdup(currentToken.value);
    return node; // Successful parsing
error:
    if (node->reservationId) free(node->reservationId);
    free(node); // Clean up in case of error
    return NULL;
}

CheckNode* parseCheck(const char **input) {
    // Initialize node
    CheckNode *node = malloc(sizeof(CheckNode));
    if (!node) {
        // Handle memory allocation failure
        return NULL;
    }
    node->base.type = CHECK_NODE;
    node->roomType = NULL;
    node->checkInDate = NULL;
    node->checkOutDate = NULL;
    Token currentToken;
    // 'check' keyword is already matched if we are in this function
    currentToken = getNextToken(input); // Should be 'room'
    if (strcmp(currentToken.value, "room") != 0) goto error;
    currentToken = getNextToken(input); // Should be 'availability'
    if (strcmp(currentToken.value, "availability") != 0) goto error;
    currentToken = getNextToken(input); // Should be <room_type>
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->roomType = strdup(currentToken.value);
    currentToken = getNextToken(input); // Should be 'from'
    if (strcmp(currentToken.value, "from") != 0) goto error;
    currentToken = getNextToken(input); // Should be <check-in_date>
    if (currentToken.type != TOKEN_LITERAL) goto error;
    node->checkInDate = strdup(currentToken.value);
    currentToken = getNextToken(input); // Should be 'to'
    if (strcmp(currentToken.value, "to") != 0) goto error;
    currentToken = getNextToken(input); // Should be <check-out_date>
    if (currentToken.type != TOKEN_LITERAL) goto error;
    node->checkOutDate = strdup(currentToken.value);
    return node; // Successful parsing

error:
    if (node->roomType) free(node->roomType);
    if (node->checkInDate) free(node->checkInDate);
    if (node->checkOutDate) free(node->checkOutDate);
    free(node); // Clean up in case of error
    return NULL;
}


CreateGuestProfileNode* parseCreateGuestProfile(const char **input) {
    // Initialize node
    CreateGuestProfileNode *node = malloc(sizeof(CreateGuestProfileNode));
    if (!node) {
        // Handle memory allocation failure
        return NULL;
    }
    node->base.type = CREATE_GUEST_PROFILE_NODE;
    node->guestName = NULL;
    node->guestContact = NULL;

    Token currentToken;

    // 'create' keyword is already matched if we are in this function
    currentToken = getNextToken(input); // Should be 'guest'
    if (strcmp(currentToken.value, "guest") != 0) goto error;
    currentToken = getNextToken(input); // Should be 'profile'
    if (strcmp(currentToken.value, "profile") != 0) goto error;
    currentToken = getNextToken(input); // Should be 'with'
    if (strcmp(currentToken.value, "with") != 0) goto error;
    currentToken = getNextToken(input); // Should be 'name'
    if (strcmp(currentToken.value, "name") != 0) goto error;
    currentToken = getNextToken(input); // Should be <guest_name>
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->guestName = strdup(currentToken.value);
    currentToken = getNextToken(input); // Should be 'and'
    if (strcmp(currentToken.value, "and") != 0) goto error;
    currentToken = getNextToken(input); // Should be 'contact'
    if (strcmp(currentToken.value, "contact") != 0) goto error;
    currentToken = getNextToken(input); // Should be <guest_contact>
    // Assuming <guest_contact> is a string literal in this context
    if (currentToken.type != TOKEN_LITERAL) goto error;
    node->guestContact = strdup(currentToken.value);
    return node; // Successful parsing
error:
    if (node->guestName) free(node->guestName);
    if (node->guestContact) free(node->guestContact);
    free(node); // Clean up in case of error
    return NULL;
}


GenerateInvoiceNode* parseGenerateInvoice(const char **input) {
    // Initialize node
    GenerateInvoiceNode *node = malloc(sizeof(GenerateInvoiceNode));
    if (!node) {
        // Handle memory allocation failure
        return NULL;
    }
    node->base.type = GENERATE_INVOICE_NODE;
    node->reservationId = NULL;
    Token currentToken;
    // 'generate' keyword is already matched if we are in this function
    currentToken = getNextToken(input); // Should be 'invoice'
    if (strcmp(currentToken.value, "invoice") != 0) goto error;
    currentToken = getNextToken(input); // Should be 'for'
    if (strcmp(currentToken.value, "for") != 0) goto error;
    currentToken = getNextToken(input); // Should be 'reservation'
    if (strcmp(currentToken.value, "reservation") != 0) goto error;
    currentToken = getNextToken(input); // Should be <reservation_id>
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->reservationId = strdup(currentToken.value);
    return node; // Successful parsing
error:
    if (node->reservationId) free(node->reservationId);
    free(node); // Clean up in case of error
    return NULL;
}


RequestServiceNode* parseRequestService(const char **input) {
    // Initialize node
    RequestServiceNode *node = malloc(sizeof(RequestServiceNode));
    if (!node) {
        // Handle memory allocation failure
        return NULL;
    }
    node->base.type = REQUEST_SERVICE_NODE;
    node->serviceName = NULL;
    node->reservationId = NULL;
    Token currentToken;
    // 'request' keyword is already matched if we are in this function
    currentToken = getNextToken(input); // Should be <service_name>
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->serviceName = strdup(currentToken.value);
    currentToken = getNextToken(input); // Should be 'for'
    if (strcmp(currentToken.value, "for") != 0) goto error;
    currentToken = getNextToken(input); // Should be 'reservation'
    if (strcmp(currentToken.value, "reservation") != 0) goto error;
    currentToken = getNextToken(input); // Should be <reservation_id>
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->reservationId = strdup(currentToken.value);
    return node; // Successful parsing

error:
    if (node->serviceName) free(node->serviceName);
    if (node->reservationId) free(node->reservationId);
    free(node); // Clean up in case of error
    return NULL;
}


ASTNode* parseStatement(const char **input) {
    Token currentToken = getNextToken(input);

    if (strcmp(currentToken.value, "reserve") == 0) {
        return (ASTNode*)parseReservation(input);
    } else if (strcmp(currentToken.value, "update") == 0) {
        return (ASTNode*)parseUpdate(input);
    } else if (strcmp(currentToken.value, "cancel") == 0) {
        return (ASTNode*)parseCancel(input);
    } else if (strcmp(currentToken.value, "check") == 0) {
        return (ASTNode*)parseCheck(input);
    } else if (strcmp(currentToken.value, "create") == 0) {
        return (ASTNode*)parseCreateGuestProfile(input);
    } else if (strcmp(currentToken.value, "generate") == 0) {
        return (ASTNode*)parseGenerateInvoice(input);
    } else if (strcmp(currentToken.value, "request") == 0) {
        return (ASTNode*)parseRequestService(input);
    } else {
        // Handle syntax error or unknown command
        printf("Syntax error or unknown command: '%s'\n", currentToken.value);
        return NULL; // Fail, return NULL for ASTNode* type
    }
}


int main() {
    // Example input string representing a command in your language
    const char *input = "reserve room deluxe from 2023-01-01 to 2023-01-05 for John Doe";

    // Call the parser with the input
    ASTNode *parsedTree = parseStatement(&input);

    if (parsedTree != NULL) {
        printf("Parsing successful!\n");

        // Here, you would normally handle the parsed AST.
        // For now, we'll just print a simple message.
        // You can extend this part to traverse the AST and perform specific actions
        // based on the types of nodes and their contents.
        printf("Command parsed: %s\n", input);

        // TODO: Handle and free the AST
    } else {
        printf("Parsing failed.\n");
    }

    return 0;
}
