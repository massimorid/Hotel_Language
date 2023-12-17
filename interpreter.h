// interpreter.h
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h" // Include to get AST node definitions

// Function declarations for executing each type of AST node
void executeProgram(ASTNode *ast);
void executeReservation(ReservationNode *node);
void executeUpdate(UpdateNode *node);
void executeCancel(CancelNode *node);
void executeCheck(CheckNode *node);
void executeCreateGuestProfile(CreateGuestProfileNode *node);
void executeGenerateInvoice(GenerateInvoiceNode *node);
void executeRequestService(RequestServiceNode *node);

#endif 
