/*
 * Simple OS Kernel: Process Scheduler Simulation
 * * Features:
 * - Implements Round Robin Scheduling Algorithm.
 * - Simulates "Context Switching" between processes.
 * - Manages a Ready Queue of processes.
 * - Calculates Waiting Time and Turnaround Time metrics.
 */

#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10

// Represents a program running in our OS
struct Process {
    int id;             // Process ID (PID)
    int burstTime;      // Total CPU time needed to finish
    int remainingTime;  // Time left to execute
    int waitingTime;    // Time spent waiting in queue
    int turnaroundTime; // Total time from arrival to completion
};

void calculateTimes(struct Process proc[], int n, int quantum);
void printSystemState(struct Process proc[], int n);

int main() {
    struct Process proc[MAX_PROCESSES];
    int n, quantum;

    printf("========================================\n");
    printf("    OS Kernel: Round Robin Scheduler    \n");
    printf("========================================\n");

    printf("Enter number of processes (max %d): ", MAX_PROCESSES);
    if (scanf("%d", &n) != 1 || n > MAX_PROCESSES || n <= 0) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    // Initialize Processes
    for (int i = 0; i < n; i++) {
        proc[i].id = i + 1;
        printf("Enter CPU Burst Time for Process P%d: ", i + 1);
        scanf("%d", &proc[i].burstTime);
        proc[i].remainingTime = proc[i].burstTime;
        proc[i].waitingTime = 0;
        proc[i].turnaroundTime = 0;
    }

    printf("Enter Time Quantum (max time a process runs at once): ");
    scanf("%d", &quantum);

    printf("\n--- Starting Scheduler Simulation ---\n");
    calculateTimes(proc, n, quantum);

    printf("\n--- Final Performance Metrics ---\n");
    printSystemState(proc, n);

    return 0;
}

// Core Scheduling Logic
void calculateTimes(struct Process proc[], int n, int quantum) {
    int currentTime = 0;
    int completed = 0;
    
    // Loop until all processes are completed
    while (completed < n) {
        bool idle = true; // Flag to check if CPU was idle this cycle

        for (int i = 0; i < n; i++) {
            // Check if process still needs CPU time
            if (proc[i].remainingTime > 0) {
                idle = false; // CPU is doing work
                
                printf("[Time %d] Context Switch -> Process P%d\n", currentTime, proc[i].id);

                if (proc[i].remainingTime > quantum) {
                    // Process runs for the full time quantum
                    currentTime += quantum;
                    proc[i].remainingTime -= quantum;
                    printf("        ... P%d ran for %dms (Remaining: %dms)\n", 
                           proc[i].id, quantum, proc[i].remainingTime);
                } else {
                    // Process finishes in this turn
                    currentTime += proc[i].remainingTime;
                    printf("        ... P%d ran for %dms and FINISHED.\n", 
                           proc[i].id, proc[i].remainingTime);
                    
                    // Calculation:
                    // Turnaround Time = Completion Time - Arrival Time (Assuming Arrival is 0 for all)
                    proc[i].turnaroundTime = currentTime;
                    
                    // Waiting Time = Turnaround Time - Burst Time
                    proc[i].waitingTime = proc[i].turnaroundTime - proc[i].burstTime;
                    
                    proc[i].remainingTime = 0;
                    completed++;
                }
            }
        }

        // Safety check if no process is ready (shouldn't happen in this simple model)
        if (idle && completed < n) {
            currentTime++;
        }
    }
}

void printSystemState(struct Process proc[], int n) {
    float avgWait = 0, avgTurnaround = 0;

    printf("------------------------------------------------------------\n");
    printf("PID\tBurst Time\tWaiting Time\tTurnaround Time\n");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("P%d\t%dms\t\t%dms\t\t%dms\n", 
            proc[i].id, proc[i].burstTime, proc[i].waitingTime, proc[i].turnaroundTime);
        
        avgWait += proc[i].waitingTime;
        avgTurnaround += proc[i].turnaroundTime;
    }

    printf("------------------------------------------------------------\n");
    printf("Average Waiting Time: %.2fms\n", avgWait / n);
    printf("Average Turnaround Time: %.2fms\n", avgTurnaround / n);
}
