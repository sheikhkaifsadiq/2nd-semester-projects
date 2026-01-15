/*
 * Palindrome and Prime Number Checker
 * * Features:
 * - Checks if a number is a Palindrome (reads the same forwards and backwards).
 * - Checks if a number is Prime (divisible only by 1 and itself).
 * - Uses a menu to let the user switch between modes.
 */

#include <stdio.h>
#include <stdbool.h>

// Function Prototypes
void checkPalindrome();
void checkPrime();
int getIntegerInput();

int main() {
    int choice;
    bool running = true;

    while (running) {
        printf("\n=== Number Checker ===\n");
        printf("1. Check Palindrome\n");
        printf("2. Check Prime Number\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            // Clear buffer
            while(getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                checkPalindrome();
                break;
            case 2:
                checkPrime();
                break;
            case 3:
                printf("Goodbye!\n");
                running = false;
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

void checkPalindrome() {
    int n, reversed = 0, original, remainder;
    
    printf("Enter an integer to check if it's a palindrome: ");
    n = getIntegerInput();
    
    // Store the original number because 'n' will be modified
    original = n;

    // Logic to reverse the number
    // Example: n = 123
    // 1. rem = 3, rev = 3, n = 12
    // 2. rem = 2, rev = 32, n = 1
    // 3. rem = 1, rev = 321, n = 0
    while (n != 0) {
        remainder = n % 10;
        reversed = reversed * 10 + remainder;
        n /= 10;
    }

    if (original == reversed)
        printf("%d is a palindrome.\n", original);
    else
        printf("%d is not a palindrome.\n", original);
}

void checkPrime() {
    int n, i;
    bool isPrime = true;

    printf("Enter a positive integer to check if it's prime: ");
    n = getIntegerInput();

    // 0 and 1 are not prime numbers
    if (n == 0 || n == 1) {
        isPrime = false;
    } else {
        // We only need to check up to n/2.
        // If a number isn't divisible by anything up to half its value,
        // it won't be divisible by anything larger (except itself).
        for (i = 2; i <= n / 2; ++i) {
            if (n % i == 0) {
                isPrime = false;
                break;
            }
        }
    }

    if (isPrime)
        printf("%d is a prime number.\n", n);
    else
        printf("%d is not a prime number.\n", n);
}

// Helper function to handle input validation
int getIntegerInput() {
    int num;
    while (scanf("%d", &num) != 1) {
        printf("Invalid input. Please enter a valid integer: ");
        while(getchar() != '\n'); // Clear buffer
    }
    return num;
}
