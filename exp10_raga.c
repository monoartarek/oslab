#include <stdio.h>
#include <stdbool.h>

int adj[20][20];
bool visited[20];
bool recStack[20];
int vertices;

bool isCyclicUtil(int v) {
    if (!visited[v]) {
        visited[v] = true;
        recStack[v] = true;

        for (int i = 0; i < vertices; i++) {
            if (adj[v][i]) {
                if (!visited[i] && isCyclicUtil(i))
                    return true;
                else if (recStack[i])
                    return true;
            }
        }
    }
    recStack[v] = false;
    return false;
}

int main() {
    int p, r;
    printf("Enter total number of Processes: ");
    if (scanf("%d", &p) != 1) return 1;
    printf("Enter total number of Resource Types: ");
    if (scanf("%d", &r) != 1) return 1;

    vertices = p + r; // Total nodes in the graph

    // Clear structures
    for (int i = 0; i < vertices; i++) {
        visited[i] = false;
        recStack[i] = false;
        for (int j = 0; j < vertices; j++) adj[i][j] = 0;
    }

    printf("\n--- Setup Adjacency Mappings ---\n");
    printf("Nodes 0 to %d map to Processes. Nodes %d to %d map to Resources.\n", p - 1, p, vertices - 1);
    
    int edges;
    printf("Enter number of direct edge links existing: ");
    if (scanf("%d", &edges) != 1) return 1;

    for (int i = 0; i < edges; i++) {
        int u, v;
        printf("Enter source vertex index and destination vertex index (u -> v): ");
        if (scanf("%d %d", &u, &v) != 2) return 1;
        adj[u][v] = 1;
    }

    bool deadlock = false;
    for (int i = 0; i < vertices; i++) {
        if (isCyclicUtil(i)) {
            deadlock = true;
            break;
        }
    }

    if (deadlock) {
        printf("\nResult: Cycle found in Resource Allocation Graph! Deadlock detected.\n");
    } else {
        printf("\nResult: Safe state! No cycles found. No Deadlocks detected.\n");
    }
    return 0;
}
