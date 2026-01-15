/*
 * Chat Server in C (TCP/IP)
 * * Features:
 * - Uses socket(), bind(), listen(), and accept() to set up a connection.
 * - Supports 1-on-1 chat with a client.
 * - Runs a "Ping-Pong" communication loop (Wait for message -> Reply).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE];

    printf("========================================\n");
    printf("        Chat Server (Listening)         \n");
    printf("========================================\n");

    // 1. Create Socket File Descriptor
    // AF_INET = IPv4, SOCK_STREAM = TCP
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Attach socket to the port 8080 (Optional: prevents "Address already in use" errors)
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Listen on any IP address of this machine
    address.sin_port = htons(PORT);       // Host to Network Short (endianness conversion)

    // 3. Bind the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 4. Start Listening (Queue up to 3 connections)
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server started on port %d. Waiting for connections...\n", PORT);

    // 5. Accept an incoming connection
    // This blocks (waits) until a client tries to connect
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Connection established!\n\n");

    // Chat Loop
    while (1) {
        // Clean buffer
        memset(buffer, 0, BUFFER_SIZE);

        // Receive message from client
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        printf("Client: %s", buffer);

        // Check for exit command
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Ending chat.\n");
            break;
        }

        // Send reply
        printf("Server: ");
        fgets(message, BUFFER_SIZE, stdin);
        send(new_socket, message, strlen(message), 0);
        
        if (strncmp(message, "exit", 4) == 0) {
            break;
        }
    }

    // Close socket
    close(new_socket);
    close(server_fd);
    return 0;
}
