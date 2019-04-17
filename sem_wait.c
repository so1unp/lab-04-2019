//
// Ejercicio 2 -- sem_wait
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Uso: %s semaforo\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sem_t *sem  = sem_open();

    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    if (sem_wait() == -1) {
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }

    printf("[%ld] sem_wait ok!\n", (long) getpid());

    exit(EXIT_SUCCESS);
}

