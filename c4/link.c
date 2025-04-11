#include <stdio.h>
#include <limits.h>

#define MAX 10
#define INF 9999

void dijkstra(int graph[MAX][MAX], int n, int start) {
    int dist[MAX], visited[MAX], prev[MAX];
    int i, count, minDist, nextNode;

    // Initialize distances and visited
    for (i = 0; i < n; i++) {
        dist[i] = graph[start][i];
        visited[i] = 0;
        prev[i] = (graph[start][i] != INF && i != start) ? start : -1;
    }

    dist[start] = 0;
    visited[start] = 1;
    count = 1;

    while (count < n - 1) {
        minDist = INF;
        for (i = 0; i < n; i++) {
            if (dist[i] < minDist && !visited[i]) {
                minDist = dist[i];
                nextNode = i;
            }
        }

        visited[nextNode] = 1;
        for (i = 0; i < n; i++) {
            if (!visited[i]) {
                if (minDist + graph[nextNode][i] < dist[i]) {
                    dist[i] = minDist + graph[nextNode][i];
                    prev[i] = nextNode;
                }
            }
        }
        count++;
    }

    // Print routing table for this node
    printf("\nRouting table for Node %d:\n", start);
    printf("Destination\tCost\tPath\n");
    for (i = 0; i < n; i++) {
        if (i != start) {
            printf("%d\t\t%d\t", i, dist[i]);
            int path[MAX], j = 0, temp = i;
            while (temp != -1) {
                path[j++] = temp;
                temp = prev[temp];
            }
            for (j--; j >= 0; j--) {
                printf("%d ", path[j]);
            }
            printf("\n");
        }
    }
}

int main() {
    int graph[MAX][MAX], n, i, j;

    printf("Enter number of nodes: ");
    scanf("%d", &n);

    printf("Enter adjacency matrix (use %d for INF/no link):\n", INF);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            scanf("%d", &graph[i][j]);

    // Compute routing tables for all nodes
    for (i = 0; i < n; i++) {
        dijkstra(graph, n, i);
    }

    return 0;
}

