/*
 * Simple Command Line Shell in C
 * * Features:
 * - Runs a Read-Eval-Print Loop (REPL).
 * - Parses user input into commands and arguments.
 * - Uses fork() to create child processes.
 * - Uses execvp() to execute system commands (ls, pwd, echo, etc.).
 * - Implements built-in commands: 'cd' and 'exit'.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

// Function Prototypes
void read_command(char *cmd);
void parse_command(char *cmd, char **args);
void execute_command(char **args);

int main() {
    char command[MAX_CMD_LEN];
    char *args[MAX_ARGS];

    printf("========================================\n");
    printf("        Custom C Shell (myshell)        \n");
    printf("========================================\n");

    while (1) {
        printf("myshell> "); // Print prompt
        
        // 1. READ
        read_command(command);

        // Handle empty input (user just pressed Enter)
        if (strcmp(command, "") == 0) {
            continue;
        }

        // 2. PARSE
        parse_command(command, args);

        // 3. EXECUTE
        // Check for built-in command 'exit'
        if (strcmp(args[0], "exit") == 0) {
            printf("Exiting shell...\n");
            break;
        }
        
        // Check for built-in command 'cd' (Change Directory)
        // We handle this manually because 'cd' changes the state of the parent process.
        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "myshell: expected argument to \"cd\"\n");
            } else {
                if (chdir(args[1]) != 0) {
                    perror("myshell"); // Print error if directory doesn't exist
                }
            }
            continue; // Skip the fork/exec step
        }

        // Execute external commands
        execute_command(args);
    }

    return 0;
}

void read_command(char *cmd) {
    if (fgets(cmd, MAX_CMD_LEN, stdin) == NULL) {
        // Handle Ctrl+D (End of File)
        printf("\n");
        exit(0);
    }
    // Remove the newline character at the end
    cmd[strcspn(cmd, "\n")] = 0;
}

void parse_command(char *cmd, char **args) {
    int i = 0;
    // strtok splits the string by spaces
    char *token = strtok(cmd, " ");
    
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL; // Arguments list must be NULL terminated
}

void execute_command(char **args) {
    // 1. FORK: Create a new process
    pid_t pid = fork();

    if (pid == 0) {
        // --- CHILD PROCESS ---
        // This code runs inside the new process.
        // execvp replaces the current process image with the new program.
        if (execvp(args[0], args) == -1) {
            perror("myshell"); // Only prints if execvp fails
        }
        exit(EXIT_FAILURE); // Kill child if command failed
    } else if (pid < 0) {
        // Error forking
        perror("myshell");
    } else {
        // --- PARENT PROCESS ---
        // We wait for the child to finish before showing the prompt again.
        wait(NULL);
    }
}
