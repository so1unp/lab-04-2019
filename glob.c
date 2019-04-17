//
// Ejercicio 1
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// variable global que incrementan los hilos
static int glob = 0;

// hilo
static void *hilo(void *arg)
{
    int loops = *((int *) arg);
    int loc, j;

    // incrementa glob
    for (j = 0; j < loops; j++) {
        loc = glob;
        loc++;
        glob = loc;
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    int loops, s;
 
    // controla numero de argumentos
    if (argc != 2) {
        fprintf(stderr, "Uso: %s ciclos\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    loops = atoi(argv[1]);

    // verifica argumentos
    if (loops < 1) {
        fprintf(stderr, "Error: ciclos debe ser mayor a cero.\n");
        exit(EXIT_FAILURE);
    }

    // crea hilo t1
    s = pthread_create(&t1, NULL, hilo, &loops);
    if (s != 0) {
        perror("pthread_create");
    }

    // crea hilo t2
    s = pthread_create(&t2, NULL, hilo, &loops);
    if (s != 0) {
        perror("pthread_create");
    }

    // espera por el hilo t1
    s = pthread_join(t1, NULL);
    if (s != 0) {
        perror("pthread_join");
    }

    // espera por el hilo t2
    s = pthread_join(t2, NULL);
    if (s != 0) {
        perror("pthread_join");
    }

    printf("glob = %d\n", glob);

    exit(EXIT_SUCCESS);
}
