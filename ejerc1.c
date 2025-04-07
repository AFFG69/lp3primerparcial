#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Función para ordenar un arreglo usando quicksort
void quicksort(int *arr, int left, int right) {
    if (left < right) {
        int pivot = arr[right];
        int i = left - 1;
        for (int j = left; j < right; j++) {
            if (arr[j] < pivot) {
                i++;
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        int temp = arr[i + 1];
        arr[i + 1] = arr[right];
        arr[right] = temp;
        int pi = i + 1;
        
        quicksort(arr, left, pi - 1);
        quicksort(arr, pi + 1, right);
    }
}

// Función para imprimir la estructura del árbol con tabulaciones
void print_tree(int process_id, int *arr, int size, int depth, int ordenado) {
    for (int i = 0; i < depth; i++) {
        printf("\t");
    }
    
    if (ordenado) {
        printf("Proceso %d lista ordenada: ", process_id);
    } else {
        printf("Proceso %d: ", process_id);
    }
    
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    
    printf("\n");
}

// Función para manejar cada proceso de ordenamiento
void process_sort(int *arr, int size, int process_id, int depth, int max_processes) {
    if (size <= 0 || process_id >= max_processes) return;
    
    print_tree(process_id, arr, size, depth, 0); // Imprime el proceso sin ordenar
    
    if (size == 1 || process_id * 2 + 1 >= max_processes) {
        quicksort(arr, 0, size - 1);
        print_tree(process_id, arr, size, depth, 1); // Imprime la lista ordenada con el nuevo formato
        return;
    }
    
    int mid = size / 2;
    int left_size = mid;
    int right_size = size - mid;
    
    pid_t left_pid = fork();
    if (left_pid == 0) {
        process_sort(arr, left_size, process_id * 2 + 1, depth + 1, max_processes);
        exit(0);
    }
    
    pid_t right_pid = fork();
    if (right_pid == 0) {
        process_sort(arr + mid, right_size, process_id * 2 + 2, depth + 1, max_processes);
        exit(0);
    }
    
    wait(NULL);
    wait(NULL);
    
    quicksort(arr, 0, size - 1);
    print_tree(process_id, arr, size, depth, 1); // Imprime la lista ordenada con el nuevo formato
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s num_procesos lista_de_numeros\n", argv[0]);
        return 1;
    }
    
    int num_procesos = atoi(argv[1]);
    if (num_procesos != 1 && num_procesos != 3 && num_procesos != 7 && num_procesos != 15) {
        fprintf(stderr, "El número de procesos debe ser 1, 3, 7, 15, etc.\n");
        return 1;
    }
    
    int arr_size = argc - 2;
    int *arr = malloc(arr_size * sizeof(int));
    
    for (int i = 0; i < arr_size; i++) {
        arr[i] = atoi(argv[i + 2]);
    }
    
    printf("- Ejecución del proceso:\n");
    process_sort(arr, arr_size, 0, 0, num_procesos);
    
    free(arr);
    return 0;
}

