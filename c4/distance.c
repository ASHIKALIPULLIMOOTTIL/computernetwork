#include <stdio.h>
#include <limits.h>

#define N 5   
#define INF 9999

int main() {
    int dist[N][N], i, j, k;
    
   
    int cost[N][N] = {
        {0,     2,     INF,   1,     8},
        {2,     0,     3,     2,     INF},
        {INF,   3,     0,     4,     INF},
        {1,     2,     4,     0,     3},
        {8,     INF,   INF,   3,     0}
    };

    
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            dist[i][j] = cost[i][j];

  
    for (k = 0; k < N; k++) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }

    
    for (i = 0; i < N; i++) {
        printf("Routing table for Node %d:\n", i);
        printf("Destination\tCost\n");
        for (j = 0; j < N; j++) {
            if (dist[i][j] == INF)
                printf("%d\t\tINF\n", j);
            else
                printf("%d\t\t%d\n", j, dist[i][j]);
        }
        printf("\n");
    }

    return 0;
}

