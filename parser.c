#include "parser.h"



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

