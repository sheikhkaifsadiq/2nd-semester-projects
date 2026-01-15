/*
 * Simple To-Do List in C
 * * Features:
 * - Add tasks (stored in a string array)
 * - View all current tasks
 * - Delete a task by its number
 * - Uses functions to separate logic
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TASKS 10      // Maximum number of tasks allowed
#define MAX_LENGTH 100    // Maximum length of a single task description

// Global variables to store tasks and the current count
char tasks[MAX_TASKS][MAX_LENGTH];
int taskCount = 0;

// Function Prototypes
void addTask();
void viewTasks();
void deleteTask();
void clearInputBuffer();

int main() {
    int choice;
    bool running = true;

    while (running) {
        printf("\n=== To-Do List Menu ===\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Delete Task\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }

        switch (choice) {
            case 1:
                addTask();
                break;
            case 2:
                viewTasks();
                break;
            case 3:
                deleteTask();
                break;
            case 4:
                printf("Exiting... Have a productive day!\n");
                running = false;
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

void addTask() {
    if (taskCount >= MAX_TASKS) {
        printf("Error: The list is full! Delete a task first.\n");
        return;
    }

    printf("Enter task description: ");
    clearInputBuffer(); // Clear newline left by scanf

    // fgets reads a line of text including spaces
    fgets(tasks[taskCount], MAX_LENGTH, stdin);

    // Remove the newline character that fgets automatically adds
    tasks[taskCount][strcspn(tasks[taskCount], "\n")] = 0;

    taskCount++;
    printf("Task added successfully.\n");
}

void viewTasks() {
    if (taskCount == 0) {
        printf("Your To-Do list is empty.\n");
        return;
    }

    printf("\n--- Your Tasks ---\n");
    for (int i = 0; i < taskCount; i++) {
        printf("%d. %s\n", i + 1, tasks[i]);
    }
}

void deleteTask() {
    if (taskCount == 0) {
        printf("List is empty. Nothing to delete.\n");
        return;
    }

    viewTasks(); // Show list so user knows what number to pick
    printf("Enter the number of the task to delete: ");
    
    int index;
    if (scanf("%d", &index) != 1) {
        printf("Invalid input.\n");
        clearInputBuffer();
        return;
    }

    // Adjust for 0-based indexing (User enters 1, we need index 0)
    if (index < 1 || index > taskCount) {
        printf("Invalid task number.\n");
        return;
    }

    // To delete, we shift all subsequent tasks one position to the left
    // Example: [A, B, C, D] -> delete B (index 1) -> [A, C, D]
    for (int i = index - 1; i < taskCount - 1; i++) {
        strcpy(tasks[i], tasks[i + 1]);
    }

    taskCount--;
    printf("Task deleted successfully.\n");
}

// Helper function to clear the "enter" key from the input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
