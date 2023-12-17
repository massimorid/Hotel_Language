#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h" 

void executeProgram(ASTNode *ast);
void executeReservation(ReservationNode *node);
void executeUpdate(UpdateNode *node);
void executeCancel(CancelNode *node);
void executeCheck(CheckNode *node);
void executeCreateGuestProfile(CreateGuestProfileNode *node);
void executeGenerateInvoice(GenerateInvoiceNode *node);
void executeRequestService(RequestServiceNode *node);

#endif 
