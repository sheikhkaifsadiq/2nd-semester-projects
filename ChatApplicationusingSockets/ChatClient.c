/*
 * Chat Client in C (TCP/IP)
 * * Features:
 * - Connects to the server using connect().
 * - Sends input to server and waits for a reply.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE];

    printf("========================================\n");
    printf("           Chat Client                  \n");
    printf("========================================\n");

    // 1. Create Socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    // 127.0.0.1 is "localhost" (your own computer)
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // 2. Connect to the Server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed. Is the server running?\n");
        return -1;
    }

    printf("Connected to server! Type 'exit' to quit.\n\n");

    // Chat Loop
    while (1) {
        // 3. Send Message
        printf("You: ");
        fgets(message, BUFFER_SIZE, stdin);
        send(sock, message, strlen(message), 0);

        if (strncmp(message, "exit", 4) == 0) {
            printf("Exiting chat...\n");
            break;
        }

        // 4. Receive Reply
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            printf("Server disconnected.\n");
            break;
        }
        printf("Server: %s", buffer);
        
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Server ended the chat.\n");
            break;
        }
    }

    close(sock);
    return 0;
}
