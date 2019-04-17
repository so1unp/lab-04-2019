//
// Ejercicio 2 -- sem_post
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
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

    if (sem_post() == -1) {
        perror("sem_post");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

