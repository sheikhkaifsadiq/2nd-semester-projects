/*
 * Student Grading System in C
 * * Features:
 * - Uses a 'struct' to group student data (Name, ID, Grades).
 * - Accepts input for multiple students.
 * - Calculates average scores and assigns letter grades.
 * - Displays a formatted report.
 */

#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 5
#define NUM_SUBJECTS 3

// Define a structure to hold student information
struct Student {
    char name[50];
    int id;
    float scores[NUM_SUBJECTS];
    float average;
    char letterGrade;
};

// Function Prototypes
void calculateGrade(struct Student *s);
void printReport(struct Student s);
void clearInputBuffer();

int main() {
    struct Student students[MAX_STUDENTS];
    int count = 0;
    char continueInput;

    printf("========================================\n");
    printf("        Student Grading System          \n");
    printf("========================================\n");

    // Input Loop
    while (count < MAX_STUDENTS) {
        printf("\n--- Enter details for Student %d ---\n", count + 1);
        
        printf("Name: ");
        fgets(students[count].name, 50, stdin);
        // Remove newline character if present
        students[count].name[strcspn(students[count].name, "\n")] = 0;

        printf("Student ID: ");
        while (scanf("%d", &students[count].id) != 1) {
            printf("Invalid ID. Please enter a number: ");
            clearInputBuffer();
        }
        clearInputBuffer(); // Clear buffer after reading ID

        // Input grades for subjects
        float sum = 0;
        printf("Enter scores for %d subjects:\n", NUM_SUBJECTS);
        for (int i = 0; i < NUM_SUBJECTS; i++) {
            printf("  Subject %d: ", i + 1);
            while (scanf("%f", &students[count].scores[i]) != 1) {
                printf("  Invalid input. Enter a number: ");
                clearInputBuffer();
            }
            sum += students[count].scores[i];
        }
        clearInputBuffer(); // Clear buffer after reading scores

        // Calculate Average
        students[count].average = sum / NUM_SUBJECTS;

        // Assign Letter Grade based on struct pointer
        calculateGrade(&students[count]);

        count++;

        // Check if user wants to add more
        if (count < MAX_STUDENTS) {
            printf("Add another student? (y/n): ");
            scanf("%c", &continueInput);
            clearInputBuffer();
            if (continueInput == 'n' || continueInput == 'N') {
                break;
            }
        }
    }

    // Output Report
    printf("\n\n========================================\n");
    printf("           Class Performance Report       \n");
    printf("========================================\n");
    for (int i = 0; i < count; i++) {
        printReport(students[i]);
    }

    return 0;
}

// Function to calculate letter grade based on average
// We pass a pointer (*s) so we can modify the actual student variable
void calculateGrade(struct Student *s) {
    if (s->average >= 90) s->letterGrade = 'A';
    else if (s->average >= 80) s->letterGrade = 'B';
    else if (s->average >= 70) s->letterGrade = 'C';
    else if (s->average >= 60) s->letterGrade = 'D';
    else s->letterGrade = 'F';
}

// Function to print a single student's row
void printReport(struct Student s) {
    printf("ID: %-5d | Name: %-20s | Avg: %-6.2f | Grade: %c\n", 
           s.id, s.name, s.average, s.letterGrade);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
