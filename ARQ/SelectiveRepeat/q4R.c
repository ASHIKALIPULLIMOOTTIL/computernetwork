#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 5600
#define WINDOW_SIZE 4
#define MAX_SEQ 10 // Maximum number of frames

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024];
    int expected_frame = 1;
    int received[MAX_SEQ] = {0}; // Buffer for storing out-of-order frames

    srand(time(NULL)); // Random seed for simulating lost ACKs

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for connection...\n");

    // Accept connection
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to sender\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int valread = recv(new_socket, buffer, sizeof(buffer), 0);
        
        if (valread <= 0) {
            printf("Connection closed by sender.\n");
            break;
        }

        int frame_no = atoi(buffer); // Convert frame number from string to int

        printf("Received frame %d\n", frame_no);

        // Store the received frame
        received[frame_no] = 1;

        // Send ACKs only for received frames
        if (rand() % 5 == 0) { // Simulating ACK loss
            printf("ACK %d lost!\n", frame_no);
        } else {
            snprintf(buffer, sizeof(buffer), "%d", frame_no);
            send(new_socket, buffer, strlen(buffer), 0);
            printf("ACK %d sent\n", frame_no);
        }

        // Deliver stored frames in order
        while (received[expected_frame]) {
            printf("Delivering frame %d\n", expected_frame);
            received[expected_frame] = 0; // Mark as delivered
            expected_frame++;
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}

