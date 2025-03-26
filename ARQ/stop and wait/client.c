#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

void main() {
    int client, y, x, k = 5, m = 1, p;
    char buffer[1024];
    struct sockaddr_in servAddr;
    socklen_t addrSize;

    // Socket creation
    client = socket(PF_INET, SOCK_STREAM, 0);

    // Address setup
    servAddr.sin_family = AF_INET;  // Address family
    servAddr.sin_port = htons(5600); // Port in network byte order
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Internet address
    memset(servAddr.sin_zero, '\0', sizeof servAddr.sin_zero); // Clear memory

    // Connecting to the server
    y = connect(client, (struct sockaddr *)&servAddr, sizeof(servAddr));
    if (y == -1) {
        printf("Error in connection\n");
        exit(1);
    }

    // Loop to send frames
    while (k != 0) {
        if (m <= 5) {
            printf("Sending %d\n", m);
        }

        if (m % 2 == 0) { // Successful transmission for even packets
            strcpy(buffer, "frame");
        } else {
            strcpy(buffer, "error"); // Simulating packet loss for odd packets
            printf("Packet loss\n");

            for (p = 1; p <= 3; p++) {
                printf("Waiting for %d seconds\n", p); // Delay for retransmission
            }

            printf("Retransmitting...\n");
            strcpy(buffer, "frame");
            sleep(3);
        }

        // Send the frame
        int x = send(client, buffer, 19, 0);
        if (x == -1) {
            printf("Error in sending\n");
            exit(1);
        } else {
            printf("Sent %d\n", m);
        }

        // Receive acknowledgment
        int z = recv(client, buffer, 1024, 0);
        if (z == -1) {
            printf("Error in receiving data\n");
            exit(1);
        }

        // Compare received frame
        if (strncmp(buffer, "ack", 3) == 0) {
            printf("Acknowledgement received for %d...\n", m);
        } else {
            printf("Ack not received\n");
        }

        k--;
        m++;
    }

    // Close socket
    close(client);
}

