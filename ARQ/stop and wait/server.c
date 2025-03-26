#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

void main() {
    int server, newSock, k = 5, m = 1, p;
    char buffer[1024];
    struct sockaddr_in servAddr;
    struct sockaddr_storage store;
    socklen_t addrSize;

    // Socket creation
    server = socket(PF_INET, SOCK_STREAM, 0);
    
    // Address setup
    servAddr.sin_family = AF_INET;  // Address family
    servAddr.sin_port = htons(5600); // Port in network byte order
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Internet address
    memset(servAddr.sin_zero, '\0', sizeof servAddr.sin_zero); // Clear memory

    // Binding
    bind(server, (struct sockaddr *)&servAddr, sizeof(servAddr));

    // Listening
    if (listen(server, 5) == 0)
        printf("Listening\n");
    else {
        printf("Error\n");
        exit(1);
    }

    // Accept connection
    newSock = accept(server, (struct sockaddr *)&store, &addrSize);
    if (newSock == -1) {
        printf("Error in creating socket\n");
        exit(1);
    }

    // Loop to receive and acknowledge frames
    while (k != 0) {
        int y = recv(newSock, buffer, 1024, 0); // Receive function

        if (y == -1) {
            printf("Error in receiving\n");
            exit(1);
        }

        // Compare received frame
        if (strncmp(buffer, "frame", 5) == 0) {
            printf("Received %d successfully\n", m);
        } else {
            printf("Frame %d not received\n", m);
        }

        // Acknowledge frame
        if (m % 2 == 0) { // Successful ACK for even packets
            strcpy(buffer, "ack");
        } else {
            strcpy(buffer, "kca"); // Error for odd packets
            printf("Ack lost\n");
            
            for (p = 1; p <= 3; p++) {
                printf("Waiting for %d seconds\n", p); // Delay for retransmission
            }

            printf("Retransmitting ack...\n");
            strcpy(buffer, "ack");
            sleep(3);
        }

        printf("Sending ack %d...\n", m);
        int z = send(newSock, buffer, 19, 0); // Send function

        if (z == -1) {
            printf("Error in sending\n");
            exit(1);
        }

        k--;
        m++;
    }

    // Close socket
    close(newSock);
}

