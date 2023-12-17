// interpreter.c
#include "interpreter.h"

// Execute the entire program by traversing the AST and executing each node
void executeProgram(ASTNode *ast) {
    while (ast != NULL) {
        switch (ast->type) {
            case RESERVATION_NODE:
                executeReservation((ReservationNode *)ast);
                break;
            case UPDATE_NODE:
                executeUpdate((UpdateNode *)ast);
                break;
            case CANCEL_NODE:
                executeCancel((CancelNode *)ast);
                break;
            case CHECK_NODE:
                executeCheck((CheckNode *)ast);
                break;
            case CREATE_GUEST_PROFILE_NODE:
                executeCreateGuestProfile((CreateGuestProfileNode *)ast);
                break;
            case GENERATE_INVOICE_NODE:
                executeGenerateInvoice((GenerateInvoiceNode *)ast);
                break;
            case REQUEST_SERVICE_NODE:
                executeRequestService((RequestServiceNode *)ast);
                break;
            default:
                printf("Unknown node type\n");
                break;
        }
        ast = ast->next; // Move to the next node in the list
    }
}


void executeReservation(ReservationNode *node) {
    if (node == NULL) {
        printf("Error: Null node passed to executeReservation.\n");
        return;
    }

    // Example logic for handling a reservation
    printf("Processing Reservation:\n");
    printf(" - Room Type: %s\n", node->roomType);
    printf(" - Check-in Date: %s\n", node->checkInDate);
    printf(" - Check-out Date: %s\n", node->checkOutDate);
    printf(" - Guest Name: %s\n", node->guestName);

}


void executeUpdate(UpdateNode *node) {
    if (node == NULL) {
        printf("Error: Null node passed to executeUpdate.\n");
        return;
    }

    // Simulated logic for updating a reservation
    printf("Simulating update operation:\n");
    printf(" - Reservation ID: %s\n", node->reservationId);
    printf(" - Attribute to Update: %s\n", node->attribute);
    printf(" - New Value: %s\n", node->value);
}

void executeCancel(CancelNode *node) {
    if (node == NULL) {
        printf("Error: Null node passed to executeCancel.\n");
        return;
    }

    // Simulated logic for handling a cancellation
    printf("Simulating cancellation operation:\n");
    printf(" - Reservation ID to Cancel: %s\n", node->reservationId);
}


void executeCancel(CancelNode *node) {
    if (node == NULL) {
        printf("Error: Null node passed to executeCancel.\n");
        return;
    }

    // Simulated logic for handling a cancellation
    printf("Simulating cancellation operation:\n");
    printf(" - Reservation ID to Cancel: %s\n", node->reservationId);
}


void executeCreateGuestProfile(CreateGuestProfileNode *node){
    if (node == NULL) {
        printf("Error: Null node passed to executeCreateGuestProfile.\n");
        return;
    }

    // Simulated logic for creating a guest profile
    printf("Simulating creation of a guest profile:\n");
    printf(" - Guest Name: %s\n", node->guestName);
    printf(" - Guest Contact: %s\n", node->guestContact);
}



void executeGenerateInvoice(GenerateInvoiceNode *node){
    if (node == NULL) {
        printf("Error: Null node passed to executeGenerateInvoice.\n");
        return;
    }

    printf("Simulating invoice generation:\n");
    printf(" - Generating Invoice for Reservation ID: %s\n", node->reservationId);
}




void executeRequestService(RequestServiceNode *node){
    if (node == NULL) {
        printf("Error: Null node passed to executeRequestService.\n");
        return;
    }

    printf("Simulating service request:\n");
    printf(" - Requesting Service: %s\n", node->serviceName);
    printf(" - For Reservation ID: %s\n", node->reservationId);
}


