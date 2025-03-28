#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_SIZE 100

int main() {
    int sock, nums[MAX_SIZE], n;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Enter number of elements: ");
    scanf("%d", &n);

    printf("Enter elements: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &nums[i]);
    }

    write(sock, &n, sizeof(n));
    write(sock, nums, n * sizeof(int));

    int max, min;
    float avg;
    read(sock, &max, sizeof(max));
    read(sock, &min, sizeof(min));
    read(sock, &avg, sizeof(avg));

    printf("Max: %d, Min: %d, Avg: %.2f\n", max, min, avg);

    close(sock);
    return 0;
}

