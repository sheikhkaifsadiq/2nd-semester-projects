/*
 * Tic-Tac-Toe Game in C
 * * Features:
 * - Two-player mode (Player X and Player O).
 * - Uses a 3x3 2D array for the board.
 * - logic checks for rows, columns, and diagonals to find a winner.
 * - Handles invalid moves and detects draws.
 */

#include <stdio.h>
#include <stdbool.h>

char board[3][3];
char currentPlayer;

// Function Prototypes
void initializeBoard();
void printBoard();
int checkWin();
int checkDraw();
void switchPlayer();

int main() {
    int row, col;
    bool gameRunning = true;
    
    initializeBoard();
    currentPlayer = 'X'; // X starts first

    printf("========================================\n");
    printf("           Tic-Tac-Toe Game             \n");
    printf("========================================\n");

    while (gameRunning) {
        printBoard();
        
        printf("\nPlayer %c's turn.\n", currentPlayer);
        printf("Enter row (1-3) and column (1-3) separated by space: ");
        
        // Input validation
        if (scanf("%d %d", &row, &col) != 2) {
            printf("Invalid input! Please enter two numbers.\n");
            // Clear buffer
            while(getchar() != '\n');
            continue;
        }

        // Adjust for 0-based indexing
        row--; 
        col--;

        // Check if move is valid
        if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ') {
            board[row][col] = currentPlayer;
            
            // Check game status after move
            if (checkWin()) {
                printBoard();
                printf("\nCongratulations! Player %c wins!\n", currentPlayer);
                gameRunning = false;
            } else if (checkDraw()) {
                printBoard();
                printf("\nIt's a draw!\n");
                gameRunning = false;
            } else {
                switchPlayer();
            }
        } else {
            printf("Invalid move! The cell is either occupied or out of bounds. Try again.\n");
        }
    }

    return 0;
}

void initializeBoard() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}

void printBoard() {
    printf("\n");
    printf(" %c | %c | %c \n", board[0][0], board[0][1], board[0][2]);
    printf("---|---|---\n");
    printf(" %c | %c | %c \n", board[1][0], board[1][1], board[1][2]);
    printf("---|---|---\n");
    printf(" %c | %c | %c \n", board[2][0], board[2][1], board[2][2]);
    printf("\n");
}

int checkWin() {
    // Check Rows
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == currentPlayer && board[i][1] == currentPlayer && board[i][2] == currentPlayer)
            return 1;
    }
    // Check Columns
    for (int i = 0; i < 3; i++) {
        if (board[0][i] == currentPlayer && board[1][i] == currentPlayer && board[2][i] == currentPlayer)
            return 1;
    }
    // Check Diagonals
    if (board[0][0] == currentPlayer && board[1][1] == currentPlayer && board[2][2] == currentPlayer)
        return 1;
    if (board[0][2] == currentPlayer && board[1][1] == currentPlayer && board[2][0] == currentPlayer)
        return 1;

    return 0;
}

int checkDraw() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ')
                return 0; // Found an empty spot, game continues
        }
    }
    return 1; // No empty spots, it's a draw
}

void switchPlayer() {
    if (currentPlayer == 'X') {
        currentPlayer = 'O';
    } else {
        currentPlayer = 'X';
    }
}
