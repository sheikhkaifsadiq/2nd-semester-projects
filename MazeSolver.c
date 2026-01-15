/*
 * Maze Solver in C using Backtracking
 * * Features:
 * - Uses a recursive function to explore paths.
 * - Solves a fixed 6x6 grid (1 = Wall, 0 = Path).
 * - Visualizes the final solution path.
 */

#include <stdio.h>
#include <stdbool.h>

// Define Maze Size
#define N 6

// The Maze Grid: 1 represents a wall, 0 represents an open path
// We want to get from (0,0) to (5,5)
int maze[N][N] = {
    {0, 1, 0, 0, 0, 0},
    {0, 1, 0, 1, 1, 0},
    {0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0}
};

// This array will store the solution path
int sol[N][N];

// Function Prototypes
void printSolution();
bool solveMaze(int x, int y);
bool isSafe(int x, int y);

int main() {
    // Initialize solution matrix to 0
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sol[i][j] = 0;
        }
    }

    printf("========================================\n");
    printf("           Maze Solver Logic            \n");
    printf("========================================\n");

    if (solveMaze(0, 0)) {
        printf("Path found!\n\n");
        printSolution();
    } else {
        printf("No solution exists for this maze.\n");
    }

    return 0;
}

// A utility function to print the solution matrix
void printSolution() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (sol[i][j] == 1)
                printf(" * "); // * represents the path
            else
                printf(" . "); // . represents empty space/walls
        }
        printf("\n");
    }
}

// Utility to check if x, y is valid index for N*N maze
bool isSafe(int x, int y) {
    // Check bounds AND check if it's not a wall (1)
    if (x >= 0 && x < N && y >= 0 && y < N && maze[x][y] == 0) {
        return true;
    }
    return false;
}

/* * Recursive function to solve the maze
 * x, y: current coordinates
 */
bool solveMaze(int x, int y) {
    // Base Case: If x, y is the destination (bottom-right corner)
    if (x == N - 1 && y == N - 1 && maze[x][y] == 0) {
        sol[x][y] = 1;
        return true;
    }

    // Check if maze[x][y] is valid
    if (isSafe(x, y) == true) {
        // Check if this cell is already part of the solution path
        // (This prevents infinite loops going back and forth)
        if (sol[x][y] == 1) {
            return false;
        }

        // Mark x, y as part of the solution path
        sol[x][y] = 1;

        /* Move forward in 'x' direction (Down) */
        if (solveMaze(x + 1, y) == true)
            return true;

        /* Move forward in 'y' direction (Right) */
        if (solveMaze(x, y + 1) == true)
            return true;

        /* Optional: Move Up */
        if (solveMaze(x - 1, y) == true)
             return true;

        /* Optional: Move Left */
        if (solveMaze(x, y - 1) == true)
             return true;

        // BACKTRACK: If none of the above movements work, unmark this cell
        // This step is the essence of backtracking. We "undo" the move.
        sol[x][y] = 0;
        return false;
    }

    return false;
}
