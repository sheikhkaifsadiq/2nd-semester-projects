/*
 * Contact Management System in C
 * * Features:
 * - Uses a Linked List to store contacts (Dynamic Memory).
 * - Saves and Loads contacts to/from a file.
 * - Allows Adding and Listing contacts.
 * - Demonstrates malloc() and pointers.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the Contact structure (Node for the Linked List)
struct Contact {
    char name[50];
    char phone[20];
    char email[50];
    struct Contact *next; // Pointer to the next contact in the list
};

// Global pointer to the head (start) of the list
struct Contact *head = NULL;
const char *FILENAME = "contacts.dat";

// Function Prototypes
void addContact();
void displayContacts();
void saveContacts();
void loadContacts();
void clearInputBuffer();
void freeList();

int main() {
    int choice;
    
    // Load existing contacts from file at startup
    loadContacts();

    while (1) {
        printf("\n=== Contact Management System ===\n");
        printf("1. Add New Contact\n");
        printf("2. Display All Contacts\n");
        printf("3. Save and Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                addContact();
                break;
            case 2:
                displayContacts();
                break;
            case 3:
                saveContacts();
                freeList(); // Clean up memory before exiting
                printf("Contacts saved. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}

void addContact() {
    // 1. Allocate memory for a new node
    struct Contact *newContact = (struct Contact*)malloc(sizeof(struct Contact));
    if (newContact == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    // 2. Get Input
    printf("Enter Name: ");
    fgets(newContact->name, 50, stdin);
    newContact->name[strcspn(newContact->name, "\n")] = 0;

    printf("Enter Phone: ");
    fgets(newContact->phone, 20, stdin);
    newContact->phone[strcspn(newContact->phone, "\n")] = 0;

    printf("Enter Email: ");
    fgets(newContact->email, 50, stdin);
    newContact->email[strcspn(newContact->email, "\n")] = 0;

    // 3. Link the new node to the list
    // We will insert at the BEGINNING (Head) for simplicity (O(1) operation)
    newContact->next = head;
    head = newContact;

    printf("Contact added successfully.\n");
}

void displayContacts() {
    if (head == NULL) {
        printf("No contacts found.\n");
        return;
    }

    struct Contact *current = head;
    printf("\n%-20s | %-15s | %-20s\n", "Name", "Phone", "Email");
    printf("-----------------------------------------------------------\n");
    
    // Traverse the list
    while (current != NULL) {
        printf("%-20s | %-15s | %-20s\n", current->name, current->phone, current->email);
        current = current->next; // Move to the next node
    }
}

void saveContacts() {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    struct Contact *current = head;
    while (current != NULL) {
        // We only write the data fields, not the 'next' pointer
        // The pointer is only valid in the current program execution
        fwrite(current->name, sizeof(current->name), 1, fp);
        fwrite(current->phone, sizeof(current->phone), 1, fp);
        fwrite(current->email, sizeof(current->email), 1, fp);
        current = current->next;
    }

    fclose(fp);
}

void loadContacts() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) return; // File doesn't exist yet, which is fine

    while (1) {
        // Create a temporary holder to read data
        struct Contact temp;
        
        // Try reading one contact's worth of data
        // If we can't read a full name, we stop
        if (fread(temp.name, sizeof(temp.name), 1, fp) < 1) break;
        fread(temp.phone, sizeof(temp.phone), 1, fp);
        fread(temp.email, sizeof(temp.email), 1, fp);

        // Allocate memory for the actual list node
        struct Contact *newNode = (struct Contact*)malloc(sizeof(struct Contact));
        strcpy(newNode->name, temp.name);
        strcpy(newNode->phone, temp.phone);
        strcpy(newNode->email, temp.email);

        // Add to head of list
        newNode->next = head;
        head = newNode;
    }

    fclose(fp);
}

// Clean up allocated memory
void freeList() {
    struct Contact *current = head;
    while (current != NULL) {
        struct Contact *temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
