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

    servAddr.sin_family = AF_INET;  // Address family
    servAddr.sin_port = htons(5600); // Port in network byte order
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Internet address
    memset(servAddr.sin_zero, '\0', sizeof(servAddr.sin_zero)); // Clear memory blocks

    // Connecting to server
    y = connect(client, (struct sockaddr *) &servAddr, sizeof(servAddr));
    if (y == -1) {
        printf("Error in connection\n");
        exit(1);
    }

    // Sending and receiving packets
    while (k != 0) { // Demo for 5 packets
        if (m <= 5)
            printf("Sending %d\n", m);

        if (m % 2 == 0) { // Successful transmission for even packets
            strcpy(buffer, "frame");
        } else { // Error for odd packets
            strcpy(buffer, "error");
            printf("Packet loss\n");

            for (p = 1; p <= 3; p++) {
                printf("Waiting for %d seconds\n", p); // Delay of 3 sec for retransmission
            }
        }

        printf("Retransmitting...\n");
        strcpy(buffer, "frame");
        sleep(3); // Sleep for 3 sec

        // Sending the frame
        int x = send(client, buffer, 19, 0);
        if (x == -1) {
            printf("Error in sending\n");
            exit(1);
        } else {
            printf("Sent %d\n", m);
        }

        // Receiving acknowledgment
        int z = recv(client, buffer, 1024, 0);
        if (z == -1) {
            printf("Error in receiving data\n");
            exit(1);
        }

        // Checking acknowledgment
        if (strncmp(buffer, "ack", 3) == 0) {
            printf("Acknowledgment received for %d...\n", m - 1);
        } else {
            printf("Ack not received\n");
        }

        k--; 
        m++;
    }

    close(client); // Closing socket
}

