/*
 * Library Management System in C
 * * Features:
 * - Uses 'struct' to store Book details.
 * - Uses FILE handling (fwrite/fread) to save data permanently.
 * - Allows Adding, Viewing, and Searching for books.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book {
    int id;
    char title[100];
    char author[100];
    // 1 = Available, 0 = Checked Out
    int isAvailable; 
};

// File name to store data
const char *FILENAME = "library_data.dat";

// Function Prototypes
void addBook();
void viewLibrary();
void searchBook();
void clearInputBuffer();

int main() {
    int choice;

    while (1) {
        printf("\n=== Library Management System ===\n");
        printf("1. Add New Book\n");
        printf("2. View All Books\n");
        printf("3. Search Book by ID\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer(); // Consume newline

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                viewLibrary();
                break;
            case 3:
                searchBook();
                break;
            case 4:
                printf("Exiting Library System. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

void addBook() {
    struct Book b;
    FILE *fp;

    // Open file in 'append binary' mode
    fp = fopen(FILENAME, "ab"); 
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d", &b.id);
    clearInputBuffer();

    printf("Enter Book Title: ");
    fgets(b.title, 100, stdin);
    b.title[strcspn(b.title, "\n")] = 0; // Remove newline

    printf("Enter Author Name: ");
    fgets(b.author, 100, stdin);
    b.author[strcspn(b.author, "\n")] = 0;

    b.isAvailable = 1; // Default to available

    // Write the struct to file
    fwrite(&b, sizeof(struct Book), 1, fp);
    
    printf("Book added successfully!\n");
    fclose(fp);
}

void viewLibrary() {
    struct Book b;
    FILE *fp;
    int count = 0;

    // Open file in 'read binary' mode
    fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("No books found (File does not exist yet).\n");
        return;
    }

    printf("\n%-5s | %-30s | %-20s | %s\n", "ID", "Title", "Author", "Status");
    printf("----------------------------------------------------------------------\n");

    // Read through the file one struct at a time
    while (fread(&b, sizeof(struct Book), 1, fp)) {
        printf("%-5d | %-30s | %-20s | %s\n", 
               b.id, 
               b.title, 
               b.author, 
               b.isAvailable ? "Available" : "Checked Out");
        count++;
    }

    if (count == 0) {
        printf("Library is empty.\n");
    }
    
    fclose(fp);
}

void searchBook() {
    struct Book b;
    FILE *fp;
    int searchId, found = 0;

    fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("No books found.\n");
        return;
    }

    printf("Enter Book ID to search: ");
    scanf("%d", &searchId);
    clearInputBuffer();

    while (fread(&b, sizeof(struct Book), 1, fp)) {
        if (b.id == searchId) {
            printf("\nBook Found!\n");
            printf("Title: %s\n", b.title);
            printf("Author: %s\n", b.author);
            printf("Status: %s\n", b.isAvailable ? "Available" : "Checked Out");
            found = 1;
            break; // Stop searching once found
        }
    }

    if (!found) {
        printf("Book with ID %d not found.\n", searchId);
    }

    fclose(fp);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
