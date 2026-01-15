/*
 * Command-Line Text Editor in C
 * * Features:
 * - Uses a Doubly Linked List to store text lines dynamically.
 * - Supports commands: LIST, ADD, DEL, SAVE, OPEN, HELP, QUIT.
 * - Tracks 'unsaved changes' to prevent accidental data loss.
 * - robust string parsing for commands.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024

// Structure to represent a single line of text
struct Line {
    char text[MAX_LINE_LEN];
    struct Line *prev;
    struct Line *next;
};

// Global State
struct Line *head = NULL;
struct Line *tail = NULL;
char currentFilename[100] = "";
int isDirty = 0; // Flag: 0 = Saved, 1 = Unsaved changes

// Function Prototypes
void appendLine(const char *text);
void deleteLine(int lineNum);
void listLines();
void saveFile(const char *filename);
void loadFile(const char *filename);
void freeBuffer();
void printHelp();

int main() {
    char input[MAX_LINE_LEN + 10]; // Buffer for user command
    char command[10];
    char argument[MAX_LINE_LEN];

    printf("========================================\n");
    printf("        C Text Editor (Line Based)      \n");
    printf("========================================\n");
    printHelp();

    while (1) {
        printf("\nEDITOR> ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;

        // Remove newline
        input[strcspn(input, "\n")] = 0;
        
        // Parse input into Command and Argument
        // Example: "open file.txt" -> command="open", argument="file.txt"
        argument[0] = '\0'; // clear arg
        int parsed = sscanf(input, "%s %[^\n]", command, argument);

        if (parsed < 1) continue; // Empty input

        if (strcmp(command, "list") == 0) {
            listLines();
        } 
        else if (strcmp(command, "add") == 0) {
            if (strlen(argument) > 0) {
                appendLine(argument);
                printf("Line added.\n");
            } else {
                printf("Error: usage 'add <text>'\n");
            }
        }
        else if (strcmp(command, "del") == 0) {
            int lineNum = atoi(argument);
            if (lineNum > 0) {
                deleteLine(lineNum);
            } else {
                printf("Error: usage 'del <line_number>'\n");
            }
        }
        else if (strcmp(command, "save") == 0) {
            if (strlen(argument) > 0) {
                saveFile(argument);
            } else if (strlen(currentFilename) > 0) {
                saveFile(currentFilename);
            } else {
                printf("Error: No filename specified. Usage 'save <filename>'\n");
            }
        }
        else if (strcmp(command, "open") == 0) {
            if (isDirty) {
                char choice;
                printf("Warning: You have unsaved changes. Discard them? (y/n): ");
                scanf(" %c", &choice);
                while(getchar() != '\n'); // Clear buffer
                if (choice != 'y' && choice != 'Y') continue;
            }
            if (strlen(argument) > 0) {
                loadFile(argument);
            } else {
                printf("Error: usage 'open <filename>'\n");
            }
        }
        else if (strcmp(command, "help") == 0) {
            printHelp();
        }
        else if (strcmp(command, "quit") == 0) {
            if (isDirty) {
                char choice;
                printf("Warning: You have unsaved changes. Quit anyway? (y/n): ");
                scanf(" %c", &choice);
                if (choice != 'y' && choice != 'Y') continue;
            }
            break;
        }
        else {
            printf("Unknown command. Type 'help' for list.\n");
        }
    }

    freeBuffer();
    printf("Exiting Editor. Goodbye!\n");
    return 0;
}

// --- Implementation ---

void appendLine(const char *text) {
    struct Line *newNode = (struct Line*)malloc(sizeof(struct Line));
    if (!newNode) {
        printf("Memory allocation error!\n");
        return;
    }
    
    strcpy(newNode->text, text);
    newNode->next = NULL;

    if (head == NULL) {
        // First line
        newNode->prev = NULL;
        head = tail = newNode;
    } else {
        // Append to end
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    isDirty = 1;
}

void deleteLine(int lineNum) {
    struct Line *current = head;
    int count = 1;

    // Traverse to the line
    while (current != NULL && count < lineNum) {
        current = current->next;
        count++;
    }

    if (current == NULL) {
        printf("Error: Line %d does not exist.\n", lineNum);
        return;
    }

    // Unlink the node
    if (current->prev) {
        current->prev->next = current->next;
    } else {
        head = current->next; // We deleted the head
    }

    if (current->next) {
        current->next->prev = current->prev;
    } else {
        tail = current->prev; // We deleted the tail
    }

    free(current);
    printf("Line %d deleted.\n", lineNum);
    isDirty = 1;
}

void listLines() {
    if (head == NULL) {
        printf("[Empty Buffer]\n");
        return;
    }

    struct Line *current = head;
    int count = 1;
    printf("\n--- File Contents ---\n");
    while (current != NULL) {
        printf("%3d: %s\n", count++, current->text);
        current = current->next;
    }
    printf("---------------------\n");
}

void saveFile(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: Could not write to file '%s'\n", filename);
        return;
    }

    struct Line *current = head;
    while (current != NULL) {
        fprintf(fp, "%s\n", current->text);
        current = current->next;
    }

    fclose(fp);
    strcpy(currentFilename, filename);
    isDirty = 0;
    printf("File saved to '%s'.\n", filename);
}

void loadFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Could not open file '%s'\n", filename);
        return;
    }

    // Clear current buffer
    freeBuffer();

    char buffer[MAX_LINE_LEN];
    while (fgets(buffer, sizeof(buffer), fp)) {
        // Remove newline if present
        buffer[strcspn(buffer, "\n")] = 0;
        appendLine(buffer);
    }

    fclose(fp);
    strcpy(currentFilename, filename);
    isDirty = 0; // Just loaded, so clean
    printf("Loaded '%s'.\n", filename);
    listLines();
}

void freeBuffer() {
    struct Line *current = head;
    while (current != NULL) {
        struct Line *temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
    tail = NULL;
}

void printHelp() {
    printf("\nCommands:\n");
    printf("  list             : Show all lines\n");
    printf("  add <text>       : Add a new line at the end\n");
    printf("  del <line num>   : Delete a specific line\n");
    printf("  save <filename>  : Save buffer to file\n");
    printf("  open <filename>  : Load file into buffer\n");
    printf("  quit             : Exit editor\n");
}
