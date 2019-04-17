//
// Ejercicio 2 -- sem_open
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

int main(int argc, char* argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Uso: %s nombre valor\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sem_t *sem = sem_open();

    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

