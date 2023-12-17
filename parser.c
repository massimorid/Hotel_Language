#include "parser.h"



ReservationNode* parseReservation(const char **input) {
    ReservationNode *node = malloc(sizeof(ReservationNode));
    if (!node) {
        return NULL;
    }
    node->base.type = RESERVATION_NODE;
    node->roomType = NULL;
    node->checkInDate = NULL;
    node->checkOutDate = NULL;
    node->guestName = NULL;
    Token currentToken;
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "room") != 0) goto error;
    currentToken = getNextToken(input); 
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->roomType = strdup(currentToken.value);
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "from") != 0) goto error;
    currentToken = getNextToken(input); 
    if (currentToken.type != TOKEN_LITERAL) goto error;
    node->checkInDate = strdup(currentToken.value);
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "to") != 0) goto error;
    currentToken = getNextToken(input);
    if (currentToken.type != TOKEN_LITERAL) goto error;
    node->checkOutDate = strdup(currentToken.value);
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "for") != 0) goto error;
    currentToken = getNextToken(input); 
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->guestName = strdup(currentToken.value);
    return node; 

    

error:
    free(node); 
    return NULL;
}



UpdateNode* parseUpdate(const char **input) {
    UpdateNode *node = malloc(sizeof(UpdateNode));
    if (!node) {
        return NULL;
    }
    node->base.type = UPDATE_NODE;
    node->reservationId = NULL;
    node->attribute = NULL;
    node->value = NULL;
    Token currentToken;
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "reservation") != 0) goto error;
    currentToken = getNextToken(input); 
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->reservationId = strdup(currentToken.value);
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "with") != 0) goto error;
    currentToken = getNextToken(input); 
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->attribute = strdup(currentToken.value);
    currentToken = getNextToken(input); 
    if (currentToken.type != TOKEN_LITERAL && currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->value = strdup(currentToken.value);
    return node; 

error:
    if (node->reservationId) free(node->reservationId);
    if (node->attribute) free(node->attribute);
    if (node->value) free(node->value);
    free(node);
    return NULL;
}


CancelNode* parseCancel(const char **input) {
    CancelNode *node = malloc(sizeof(CancelNode));
    if (!node) {
        return NULL;
    }
    node->base.type = CANCEL_NODE;
    node->reservationId = NULL;
    Token currentToken;
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "reservation") != 0) goto error;
    currentToken = getNextToken(input); 
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->reservationId = strdup(currentToken.value);
    return node; 
error:
    if (node->reservationId) free(node->reservationId);
    free(node); 
    return NULL;
}

CheckNode* parseCheck(const char **input) {
    CheckNode *node = malloc(sizeof(CheckNode));
    if (!node) {
        return NULL;
    }
    node->base.type = CHECK_NODE;
    node->roomType = NULL;
    node->checkInDate = NULL;
    node->checkOutDate = NULL;
    Token currentToken;
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "room") != 0) goto error;
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "availability") != 0) goto error;
    currentToken = getNextToken(input); 
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->roomType = strdup(currentToken.value);
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "from") != 0) goto error;
    currentToken = getNextToken(input); 
    if (currentToken.type != TOKEN_LITERAL) goto error;
    node->checkInDate = strdup(currentToken.value);
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "to") != 0) goto error;
    currentToken = getNextToken(input);
    if (currentToken.type != TOKEN_LITERAL) goto error;
    node->checkOutDate = strdup(currentToken.value);
    return node; 

error:
    if (node->roomType) free(node->roomType);
    if (node->checkInDate) free(node->checkInDate);
    if (node->checkOutDate) free(node->checkOutDate);
    free(node); 
    return NULL;
}


CreateGuestProfileNode* parseCreateGuestProfile(const char **input) {
    CreateGuestProfileNode *node = malloc(sizeof(CreateGuestProfileNode));
    if (!node) {
        return NULL;
    }
    node->base.type = CREATE_GUEST_PROFILE_NODE;
    node->guestName = NULL;
    node->guestContact = NULL;

    Token currentToken;

    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "guest") != 0) goto error;
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "profile") != 0) goto error;
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "with") != 0) goto error;
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "name") != 0) goto error;
    currentToken = getNextToken(input); 
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->guestName = strdup(currentToken.value);
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "and") != 0) goto error;
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "contact") != 0) goto error;
    currentToken = getNextToken(input); 
    if (currentToken.type != TOKEN_LITERAL) goto error;
    node->guestContact = strdup(currentToken.value);
    return node; 
error:
    if (node->guestName) free(node->guestName);
    if (node->guestContact) free(node->guestContact);
    free(node); 
    return NULL;
}


GenerateInvoiceNode* parseGenerateInvoice(const char **input) {
    GenerateInvoiceNode *node = malloc(sizeof(GenerateInvoiceNode));
    if (!node) {
        return NULL;
    }
    node->base.type = GENERATE_INVOICE_NODE;
    node->reservationId = NULL;
    Token currentToken;
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "invoice") != 0) goto error;
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "for") != 0) goto error;
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "reservation") != 0) goto error;
    currentToken = getNextToken(input); 
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->reservationId = strdup(currentToken.value);
    return node; 
error:
    if (node->reservationId) free(node->reservationId);
    free(node); 
    return NULL;
}


RequestServiceNode* parseRequestService(const char **input) {
    RequestServiceNode *node = malloc(sizeof(RequestServiceNode));
    if (!node) {
        return NULL;
    }
    node->base.type = REQUEST_SERVICE_NODE;
    node->serviceName = NULL;
    node->reservationId = NULL;
    Token currentToken;
    currentToken = getNextToken(input); 
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->serviceName = strdup(currentToken.value);
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "for") != 0) goto error;
    currentToken = getNextToken(input); 
    if (strcmp(currentToken.value, "reservation") != 0) goto error;
    currentToken = getNextToken(input); 
    if (currentToken.type != TOKEN_IDENTIFIER) goto error;
    node->reservationId = strdup(currentToken.value);
    return node; 

error:
    if (node->serviceName) free(node->serviceName);
    if (node->reservationId) free(node->reservationId);
    free(node); 
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
        printf("Syntax error or unknown command: '%s'\n", currentToken.value);
        return NULL; 
    }
}

