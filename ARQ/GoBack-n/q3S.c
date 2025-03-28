#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5600
#define WINDOW_SIZE 4

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    int base = 1, next_frame = 1;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert address
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to receiver\n");

    while (1) {
        while (next_frame < base + WINDOW_SIZE) {
            snprintf(buffer, sizeof(buffer), "%d", next_frame);
            send(sock, buffer, strlen(buffer), 0);
            printf("Sent frame %d\n", next_frame);
            next_frame++;
        }

        // Wait for acknowledgment
        memset(buffer, 0, sizeof(buffer));
        int valread = recv(sock, buffer, sizeof(buffer), 0);
        
        if (valread <= 0) {
            printf("Connection closed by receiver.\n");
            break;
        }

        int ack_no = atoi(buffer);
        printf("Received ACK %d\n", ack_no);

        if (ack_no >= base) {
            base = ack_no + 1;
        } else {
            printf("ACK error: Expected %d but got %d. Retransmitting from %d...\n", base, ack_no, base);
            next_frame = base; // Retransmit from the last unacknowledged frame
        }

        if (base >= 10) {
            printf("All frames sent successfully.\n");
            break;
        }
    }

    close(sock);
    return 0;
}

