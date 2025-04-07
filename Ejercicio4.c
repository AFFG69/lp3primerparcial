#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define MAX_NODES 100

void create_tree(int node_number, int max_nodes, int *values)
{
    if (node_number >= max_nodes)
    {
        return;
    }

    values[node_number] = rand() % 100; // Guardar el número aleatorio

    pid_t left_child, right_child;

    left_child = fork();
    if (left_child == 0)
    {
        create_tree(2 * node_number + 1, max_nodes, values);
        exit(0);
    }

    right_child = fork();
    if (right_child == 0)
    {
        create_tree(2 * node_number + 2, max_nodes, values);
        exit(0);
    }

    wait(NULL);
    wait(NULL);
}
void print_subtree(int *values, int index, int max_nodes, int depth, char *prefix, int is_left) {
    if (index >= max_nodes) return;

    // Construir nuevo prefijo
    char new_prefix[256];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_left ? "│   " : "    ");

    // Imprimir subárbol derecho primero (así se ve arriba)
    print_subtree(values, 2 * index + 2, max_nodes, depth + 1, new_prefix, 0);

    // Imprimir nodo actual
    printf("%s", prefix);
    printf("%s── ", is_left ? "├" : "└");
    printf("%d(nodo=%d)\n", values[index], index);

    // Imprimir subárbol izquierdo
    print_subtree(values, 2 * index + 1, max_nodes, depth + 1, new_prefix, 1);
}

void print_tree(int *values, int max_nodes) {
    print_subtree(values, 0, max_nodes, 0, "", 0);
}


/*
void print_tree(int *values, int max_nodes)
{
    int level = 0;
    int nodes_in_level = 1;
    int index = 0;

    while (index < max_nodes)
    {
        for (int i = 0; i < nodes_in_level && index < max_nodes; i++, index++)
        {
            printf("%d(%d) ", values[index], index);
        }
        printf("\n");
        nodes_in_level *= 2;
    }
}
*/
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    if (N <= 0 || N > MAX_NODES)
    {
        printf("El número de nodos debe estar entre 1 y %d.\n", MAX_NODES);
        return 1;
    }

    int shm_id = shmget(IPC_PRIVATE, N * sizeof(int), IPC_CREAT | 0666);
    int *values = (int *)shmat(shm_id, NULL, 0);

    srand(getpid());
    create_tree(0, N, values);

    printf("Salida:\n");
    print_tree(values, N);

    shmdt(values);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
