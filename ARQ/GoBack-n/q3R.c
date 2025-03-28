#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5600
#define WINDOW_SIZE 4

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024];
    int expected_frame = 1; 

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

    // Receive frames
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int valread = recv(new_socket, buffer, sizeof(buffer), 0);
        
        if (valread <= 0) {
            printf("Connection closed by sender.\n");
            break;
        }

        int frame_no = atoi(buffer); // Convert frame number from string to int

        if (frame_no == expected_frame) {
            printf("Received frame %d\n", frame_no);
            expected_frame++; // Expect next frame
        } else {
            printf("Frame %d out of order. Discarding...\n", frame_no);
        }

        // Randomly drop ACKs to simulate packet loss
        if (rand() % 5 == 0) { 
            printf("ACK %d lost!\n", expected_frame - 1);
        } else {
            snprintf(buffer, sizeof(buffer), "%d", expected_frame - 1);
            send(new_socket, buffer, strlen(buffer), 0);
            printf("ACK %d sent\n", expected_frame - 1);
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}

