#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"



typedef struct ASTNode {
    enum { RESERVATION_NODE, UPDATE_NODE, CANCEL_NODE, CHECK_NODE, CREATE_GUEST_PROFILE_NODE, GENERATE_INVOICE_NODE, REQUEST_SERVICE_NODE } type;
    struct ASTNode *next; // Next node in the list
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

#endif 
