#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void signal_handler(int signum) {
    printf("Señal recibida: %d\n", signum);
    if (signum == SIGTERM) {
        printf("SIGTERM recibido, pero el proceso continuará ejecutándose.\n");
    }
  /*
  else if (signum == SIGQUIT) {
        printf("SIGQUIT recibido, ignorando...\n");
    }
    */
}

int main() {
    printf("Proceso receptor de señales iniciado. PID: %d\n", getpid());

    // Configurar las señales a capturar
    signal(SIGINT, signal_handler);
    signal(SIGALRM, signal_handler);
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);
    signal(SIGTERM, signal_handler);  // Ahora maneja SIGTERM en lugar de terminar
    signal(SIGQUIT, signal_handler);  // Agregamos manejo de SIGQUIT
    signal(SIGQUIT, signal_handler);
    // Bucle infinito esperando señales
    while (1) {
        pause();
    }

    return 0;
}

