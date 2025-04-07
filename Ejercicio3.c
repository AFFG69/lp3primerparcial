#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t signal_received = 0;

void signal_handler(int signum)
{
    if (signum == SIGUSR1)
    {
        signal_received = 1;
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Este programa espera como mínimo 2 atributos\n");
        return -1;
    }

    int child_status;
    pid_t process1, process2;

    process1 = fork();

    if (process1 == 0)
    {
        // Proceso hijo 1
        printf("Soy el proceso hijo con pid %d e imprimo el calendario:\n", getpid());
        execlp("cal", "cal", argv[1], argv[2], NULL);
        perror("Error ejecutando cal");
        exit(1);
    }
    else
    {
        printf("Proceso padre esperando por hijo %d que imprimirá el calendario\n", process1);
        process2 = fork();

        if (process2 == 0)
        {
            // Proceso hijo 2
            printf("Soy el proceso hijo con pid %d y esperaré la señal SIGUSR1\n", getpid());

            // Configurar el manejador de señales
            struct sigaction sa;
            sa.sa_handler = signal_handler;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sigaction(SIGUSR1, &sa, NULL);

            // Esperar la señal SIGUSR1
            while (!signal_received)
            {
                pause();
            }

            printf("Proceso hijo %d recibió la señal SIGUSR1 y ahora termina.\n", getpid());
            exit(0);
        }
        else
        {
            printf("Proceso padre esperando por hijo %d que esperará señal SIGUSR1\n", process2);

            // Esperar a que el proceso hijo 1 termine
            waitpid(process1, &child_status, 0);
            printf("Soy el proceso padre y el hijo %d ha terminado\n", process1);

            // Esperar a que el proceso hijo 2 termine
            waitpid(process2, &child_status, 0);
        }
    }

    return 0;
}
