/*
 * Simple Calculator in C
 * * This program performs basic arithmetic operations (addition, subtraction, 
 * multiplication, division) based on user input.
 * * Features:
 * - Uses a while loop to allow multiple calculations.
 * - Uses a switch statement to handle different operators.
 * - Includes a check for division by zero.
 */

#include <stdio.h>
#include <stdbool.h>

int main() {
    char operator;
    double num1, num2, result;
    bool running = true;

    printf("========================================\n");
    printf("      Simple Command-Line Calculator    \n");
    printf("========================================\n");

    while (running) {
        printf("\nEnter an operator (+, -, *, /) or 'q' to quit: ");
        // Notice the space before %c. This consumes any leftover newline characters
        // from previous inputs, preventing the program from skipping input.
        scanf(" %c", &operator);

        // Check if the user wants to quit
        if (operator == 'q' || operator == 'Q') {
            printf("Exiting calculator. Goodbye!\n");
            running = false;
            break; 
        }

        // Validate operator before asking for numbers
        if (operator != '+' && operator != '-' && operator != '*' && operator != '/') {
            printf("Error: Invalid operator. Please try again.\n");
            continue; // Skip the rest of the loop and start over
        }

        printf("Enter two numbers: ");
        // checking the return value of scanf ensures we actually got numbers
        if (scanf("%lf %lf", &num1, &num2) != 2) {
            printf("Error: Invalid input. Please enter numbers only.\n");
            // Clear the input buffer to prevent infinite loops on bad input
            while(getchar() != '\n'); 
            continue;
        }

        switch (operator) {
            case '+':
                result = num1 + num2;
                printf("Result: %.2lf + %.2lf = %.2lf\n", num1, num2, result);
                break;

            case '-':
                result = num1 - num2;
                printf("Result: %.2lf - %.2lf = %.2lf\n", num1, num2, result);
                break;

            case '*':
                result = num1 * num2;
                printf("Result: %.2lf * %.2lf = %.2lf\n", num1, num2, result);
                break;

            case '/':
                if (num2 != 0) {
                    result = num1 / num2;
                    printf("Result: %.2lf / %.2lf = %.2lf\n", num1, num2, result);
                } else {
                    printf("Error: Division by zero is not allowed.\n");
                }
                break;
                
            // The default case is technically covered by our `if` check above,
            // but it's good practice to have it in switch statements.
            default:
                printf("Error: Something went wrong.\n");
        }
    }

    return 0;
}
