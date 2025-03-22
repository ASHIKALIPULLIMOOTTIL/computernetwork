#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

void main()
{
    int client;
    struct sockaddr_in servAddr;
    char servMsg[2000], cliMsg[2000];
    int server_struct_length = sizeof(servAddr);

    client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client < 0)
    {
        printf("Error while creating socket\n");
        exit(1);
    }

    printf("Socket created successfully\n");
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(2002);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter message to server: ");
    fgets(cliMsg, sizeof(cliMsg), stdin);
    cliMsg[strcspn(cliMsg, "\n")] = 0; // Remove newline



    if (sendto(client, cliMsg, strlen(cliMsg), 0, (struct sockaddr *)&servAddr, server_struct_length) < 0)
    {
        printf("Unable to send message\n");
        exit(1);
    }

    if (recvfrom(client, servMsg, sizeof(servMsg), 0, (struct sockaddr *)&servAddr, &server_struct_length) < 0)
    {
        printf("Error while receiving server's msg\n");
        exit(1);
    }

    close(client);
}

