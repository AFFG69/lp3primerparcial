#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 100

void send_signal(pid_t pid, int signal, int delay) {
    sleep(delay); // Espera el tiempo indicado
    if (kill(pid, signal) == 0) {
        printf("Se envió la señal %d al proceso %d tras %d segundos\n", signal, pid, delay);
    } else {
        printf("El proceso %d no existe. No se puede enviar la señal %d\n", pid, signal);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <archivo>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error abriendo el archivo");
        return 1;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        pid_t pid;
        int signal, delay;
        if (sscanf(line, "%d %d %d", &pid, &signal, &delay) == 3) {
            if (kill(pid, 0) == 0) { // Verifica si el proceso existe antes de enviar la señal
                if (fork() == 0) {
                    send_signal(pid, signal, delay);
                    exit(0);
                }
            } else {
                printf("El proceso %d no existe. No se enviará la señal %d\n", pid, signal);
            }
        }
    }

    fclose(file);
    while (wait(NULL) > 0); // Espera a que terminen los hijos
    return 0;
}

