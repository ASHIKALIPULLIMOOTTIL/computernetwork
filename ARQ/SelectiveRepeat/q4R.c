#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define TOTAL_FRAMES 10

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char frame[2], ack[2];
    int received[TOTAL_FRAMES] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    listen(server_fd, 3);
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Receiver connected.\n");

    while (1) {
        recv(new_socket, frame, sizeof(frame), 0);
        int frame_no = atoi(frame);
        printf("Received Frame: %d\n", frame_no);

        if (!received[frame_no]) {
            received[frame_no] = 1;
            sprintf(ack, "%d", frame_no);
            send(new_socket, ack, strlen(ack), 0);
            printf("Sent ACK: %s\n", ack);
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}

