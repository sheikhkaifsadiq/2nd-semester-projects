/*
 * Simple Bank System in C
 * * Features:
 * - Create new accounts with a secure 4-digit PIN.
 * - Perform Deposits and Withdrawals.
 * - PIN authentication required for withdrawals and balance checks.
 * - Validates input to prevent negative deposits or overdrafts.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ACCOUNTS 100

struct Account {
    int accountNumber;
    char name[50];
    int pin; // Simple integer PIN for security
    float balance;
};

// Global "Database" of accounts
struct Account bank[MAX_ACCOUNTS];
int accountCount = 0;

// Function Prototypes
void createAccount();
void depositMoney();
void withdrawMoney();
void checkBalance();
int findAccount(int accNum);
void clearInputBuffer();

int main() {
    int choice;
    
    while (1) {
        printf("\n=== Simple Bank System ===\n");
        printf("1. Create New Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Check Balance\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                depositMoney();
                break;
            case 3:
                withdrawMoney();
                break;
            case 4:
                checkBalance();
                break;
            case 5:
                printf("Thank you for using Simple Bank. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}

void createAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Error: Bank database is full.\n");
        return;
    }

    struct Account newAcc;

    // Auto-generate account number (starting from 1001)
    newAcc.accountNumber = 1001 + accountCount;

    printf("\n--- Create Account ---\n");
    printf("Enter Account Holder Name: ");
    fgets(newAcc.name, 50, stdin);
    newAcc.name[strcspn(newAcc.name, "\n")] = 0; // Remove newline

    printf("Set a 4-digit PIN: ");
    while (scanf("%d", &newAcc.pin) != 1 || newAcc.pin < 1000 || newAcc.pin > 9999) {
        printf("Invalid PIN. Please enter a 4-digit number: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    newAcc.balance = 0.0f; // Initial balance is zero

    // Save to global array
    bank[accountCount] = newAcc;
    accountCount++;

    printf("Account created successfully!\n");
    printf("Your Account Number is: %d\n", newAcc.accountNumber);
    printf("PLEASE REMEMBER YOUR ACCOUNT NUMBER AND PIN.\n");
}

void depositMoney() {
    int accNum, index;
    float amount;

    printf("\n--- Deposit Money ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &accNum);
    clearInputBuffer();

    index = findAccount(accNum);
    if (index == -1) {
        printf("Account not found.\n");
        return;
    }

    printf("Enter amount to deposit: $");
    if (scanf("%f", &amount) != 1 || amount <= 0) {
        printf("Invalid amount. Must be positive.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    bank[index].balance += amount;
    printf("Success! New Balance: $%.2f\n", bank[index].balance);
}

void withdrawMoney() {
    int accNum, index, pinInput;
    float amount;

    printf("\n--- Withdraw Money ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &accNum);
    clearInputBuffer();

    index = findAccount(accNum);
    if (index == -1) {
        printf("Account not found.\n");
        return;
    }

    // Security Check
    printf("Enter PIN: ");
    scanf("%d", &pinInput);
    clearInputBuffer();

    if (pinInput != bank[index].pin) {
        printf("Authentication Failed! Wrong PIN.\n");
        return;
    }

    printf("Enter amount to withdraw: $");
    if (scanf("%f", &amount) != 1 || amount <= 0) {
        printf("Invalid amount.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (amount > bank[index].balance) {
        printf("Transaction Declined: Insufficient Funds.\n");
        printf("Current Balance: $%.2f\n", bank[index].balance);
    } else {
        bank[index].balance -= amount;
        printf("Withdrawal Successful. Please take your cash.\n");
        printf("Remaining Balance: $%.2f\n", bank[index].balance);
    }
}

void checkBalance() {
    int accNum, index, pinInput;

    printf("\n--- Check Balance ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &accNum);
    clearInputBuffer();

    index = findAccount(accNum);
    if (index == -1) {
        printf("Account not found.\n");
        return;
    }

    // Security Check
    printf("Enter PIN: ");
    scanf("%d", &pinInput);
    clearInputBuffer();

    if (pinInput != bank[index].pin) {
        printf("Authentication Failed! Wrong PIN.\n");
        return;
    }

    printf("\nAccount Holder: %s\n", bank[index].name);
    printf("Current Balance: $%.2f\n", bank[index].balance);
}

// Helper to find the array index of an account number
int findAccount(int accNum) {
    for (int i = 0; i < accountCount; i++) {
        if (bank[i].accountNumber == accNum) {
            return i;
        }
    }
    return -1; // Not found
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
