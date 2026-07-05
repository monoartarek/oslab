/*
 * Experiment 11: Banker's Algorithm (Deadlock Avoidance)
 * Compile : gcc exp11_bankers.c -o bankers
 * Run     : ./bankers
 */
#include <stdio.h>

int main() {
    int n, m, i, j;

    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resource types: ");
    scanf("%d", &m);

    int max[n][m], alloc[n][m], need[n][m];
    int available[m];

    printf("\nEnter the Maximum matrix (%d x %d):\n", n, m);
    for (i = 0; i < n; i++) {
        printf("Process P%d: ", i);
        for (j = 0; j < m; j++)
            scanf("%d", &max[i][j]);
    }

    printf("\nEnter the Allocation matrix (%d x %d):\n", n, m);
    for (i = 0; i < n; i++) {
        printf("Process P%d: ", i);
        for (j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);
    }

    printf("\nEnter the Available vector (%d values):\n", m);
    for (j = 0; j < m; j++)
        scanf("%d", &available[j]);

    /* Need = Max - Allocation */
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];

    int finish[n];
    for (i = 0; i < n; i++) finish[i] = 0;

    int work[m];
    for (j = 0; j < m; j++) work[j] = available[j];

    int safe_seq[n], count = 0;
    int found;

    do {
        found = 0;
        for (i = 0; i < n; i++) {
            if (!finish[i]) {
                int can_allocate = 1;
                for (j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        can_allocate = 0;
                        break;
                    }
                }
                if (can_allocate) {
                    for (j = 0; j < m; j++)
                        work[j] += alloc[i][j];
                    safe_seq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
    } while (found);

    printf("\n--- Need Matrix ---\n");
    for (i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (j = 0; j < m; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }

    if (count == n) {
        printf("\nSystem is in a SAFE state.\n");
        printf("Safe sequence: ");
        for (i = 0; i < count; i++)
            printf("P%d ", safe_seq[i]);
        printf("\n");
    } else {
        printf("\nSystem is in an UNSAFE state (deadlock may occur)!\n");
        printf("Processes stuck: ");
        for (i = 0; i < n; i++)
            if (!finish[i])
                printf("P%d ", i);
        printf("\n");
    }

    return 0;
}
