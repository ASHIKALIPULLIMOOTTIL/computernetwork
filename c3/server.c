#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_SIZE 100

int main() {
    int server_fd, new_socket, nums[MAX_SIZE], n;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    new_socket = accept(server_fd, (struct sockaddr*)&address, &addr_len);
    if (new_socket < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    read(new_socket, &n, sizeof(n));
    read(new_socket, nums, n * sizeof(int));

    int max = nums[0], min = nums[0], sum = nums[0];
    for (int i = 1; i < n; i++) {
        if (nums[i] > max) max = nums[i];
        if (nums[i] < min) min = nums[i];
        sum += nums[i];
    }
    float avg = (float)sum / n;

    write(new_socket, &max, sizeof(max));
    write(new_socket, &min, sizeof(min));
    write(new_socket, &avg, sizeof(avg));

    printf("Results sent to client.\n");

    close(new_socket);
    close(server_fd);
    return 0;
}

