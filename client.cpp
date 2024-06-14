#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9034
#define BUF_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char message[BUF_SIZE];

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server. Type 'exit' to quit.\n");

    // Send messages to server
    while (1) {
        printf("Enter message: ");
        fgets(message, BUF_SIZE, stdin);
        // Remove newline character
        message[strcspn(message, "\n")] = '\0';

        // Send message to server
        send(sockfd, message, strlen(message), 0);

        // Check for exit command
        if (strcmp(message, "exit") == 0) {
            break;
        }

        // Clear message buffer
        memset(message, 0, sizeof(message));

        // Receive response from server
        recv(sockfd, message, BUF_SIZE, 0);
        printf("Server response: %s\n", message);
    }

    // Close socket
    close(sockfd);
    printf("Connection closed.\n");

    return 0;
}
