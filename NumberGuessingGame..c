/*
 * Number Guessing Game in C
 * * Features:
 * - Generates a random number between 1 and 100.
 * - Provides feedback (Too High/Too Low) for every guess.
 * - Tracks the number of attempts.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int secretNumber, guess, attempts = 0;

    // Initialize random number generator using current time
    // If we don't do this, the sequence of numbers will be the same every time we run the program.
    srand(time(0));

    // Generate random number between 1 and 100
    // rand() returns a large integer. % 100 limits it to 0-99. + 1 makes it 1-100.
    secretNumber = (rand() % 100) + 1;

    printf("========================================\n");
    printf("        Number Guessing Game            \n");
    printf("========================================\n");
    printf("I have picked a number between 1 and 100.\n");
    printf("Can you guess what it is?\n\n");

    // Loop until the user guesses the correct number
    do {
        printf("Enter your guess: ");
        
        // Input validation: check if the user actually entered a number
        if (scanf("%d", &guess) != 1) {
            printf("Invalid input. Please enter a number.\n");
            // Clear input buffer to prevent infinite loops
            while(getchar() != '\n');
            continue;
        }

        attempts++;

        if (guess > secretNumber) {
            printf("Too high! Try again.\n");
        } else if (guess < secretNumber) {
            printf("Too low! Try again.\n");
        } else {
            printf("\nCongratulations! You found the number %d in %d attempts.\n", secretNumber, attempts);
        }

    } while (guess != secretNumber);

    return 0;
}
