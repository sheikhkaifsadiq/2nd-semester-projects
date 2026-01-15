/*
 * Basic Arithmetic Interpreter in C
 * * Features:
 * - Implements a Recursive Descent Parser.
 * - Supports integer arithmetic (+, -, *, /).
 * - Handles Operator Precedence (multiplication before addition).
 * - Handles Parentheses for grouping.
 * - Ignores whitespace for flexible input.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Global pointer to track where we are in the string
const char *expression;

// Function Prototypes for the Parser
int parse_expression();
int parse_term();
int parse_factor();
void skip_whitespace();

int main() {
    char input[256];

    printf("========================================\n");
    printf("     Basic Arithmetic Interpreter       \n");
    printf("========================================\n");
    printf("Enter mathematical expressions (e.g., 3 + 4 * (2 - 1)).\n");
    printf("Type 'exit' to quit.\n\n");

    while (1) {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        // Remove newline character
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) break;
        if (strlen(input) == 0) continue;

        // Point the global pointer to the start of the user's input
        expression = input;
        
        // Start the recursive parsing chain
        int result = parse_expression();
        
        // Only print result if we consumed the whole string (simple error check)
        skip_whitespace();
        if (*expression == '\0') {
            printf("Result: %d\n", result);
        } else {
            printf("Error: Unexpected character '%c' at end of expression.\n", *expression);
        }
    }

    return 0;
}

// Helper: Skips spaces to allow input like "3 + 4"
void skip_whitespace() {
    while (*expression && isspace(*expression)) {
        expression++;
    }
}

// Level 1: Handles Addition and Subtraction
// Grammar: Expression = Term + Term - Term ...
int parse_expression() {
    int result = parse_term(); // Get the first number (or product)

    while (1) {
        skip_whitespace();
        if (*expression == '+') {
            expression++;
            result += parse_term();
        } else if (*expression == '-') {
            expression++;
            result -= parse_term();
        } else {
            break; // No more + or -, return what we have
        }
    }
    return result;
}

// Level 2: Handles Multiplication and Division (Higher Precedence)
// Grammar: Term = Factor * Factor / Factor ...
int parse_term() {
    int result = parse_factor(); // Get the number (or parenthesized group)

    while (1) {
        skip_whitespace();
        if (*expression == '*') {
            expression++;
            result *= parse_factor();
        } else if (*expression == '/') {
            expression++;
            int divisor = parse_factor();
            if (divisor == 0) {
                printf("Error: Division by zero!\n");
                return 0;
            }
            result /= divisor;
        } else {
            break;
        }
    }
    return result;
}

// Level 3: Handles Numbers and Parentheses (Highest Precedence)
// Grammar: Factor = Number | (Expression)
int parse_factor() {
    skip_whitespace();
    int result = 0;

    if (*expression == '(') {
        expression++; // Eat the '('
        result = parse_expression(); // Recursively parse what's inside
        skip_whitespace();
        if (*expression == ')') {
            expression++; // Eat the ')'
        } else {
            printf("Error: Missing closing parenthesis.\n");
        }
    } else if (isdigit(*expression)) {
        // Parse the integer digit by digit
        while (isdigit(*expression)) {
            result = result * 10 + (*expression - '0');
            expression++;
        }
    } else {
        // If it's not a number or '(', it's garbage
        printf("Error: Expected number or '(', found '%c'\n", *expression);
        if (*expression) expression++; // Skip to avoid infinite loop
    }
    
    return result;
}
