//
// Ejercicio 4 -- cena de los filosofos
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

#define N 5
const char *names[N] = { "Socrates", "Kant", "Spinoza", "Wittgenstein", "Camus" };

#define M 5                     
const char *topic[M] = { "Espagueti!", "Vida", "El ser", "Star Trek", "Futbol" };

// Mueve el cursor a la linea x, columna y (ver codigos ANSI)
#define xy(x, y) printf("\033[%d;%dH", x, y)

// Borra desde el cursor hasta el fin de la linea
#define clear_eol(x) print(x, 17, "\033[K")

// Borra la pantalla 
#define clear() printf("\033[H\033[J") 

// Mutex
static pthread_mutex_t screen = PTHREAD_MUTEX_INITIALIZER;

// Imprime en pantalla, en la posición x/y la cadena *fmt
void print(int y, int x, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    pthread_mutex_lock(&screen);
    xy(y + 1, x), vprintf(fmt, ap);
    xy(N + 1, 1), fflush(stdout);
    pthread_mutex_unlock(&screen);
}

// El filosofo come
void eat(int id)
{
    int f[2]; // tenedores 
    int ration, i; 

    // los tenedores a tomar
    f[0] = id;
    f[1] = (id + 1) % N;

    clear_eol(id);
    print(id, 18, "..oO (necesito tenedores)");
    sleep(2);

    // trata de tomar los tenedores
    for (i = 0; i < 2; i++) {
        if (!i) {
            clear_eol(id);
	    }

        print(id, 18 + (f[i] != id) * 12, "tenedor%d", f[i]);

        // espera para tomar el segundo tenedor
        sleep(3);
    }

    // come durante un tiempo
    for (i = 0, ration = 3 + rand() % 8; i < ration; i++) {
        print(id, 40 + i * 4, "ñom");
        sleep(1);
    }
}

// el filosofo piensa
void think(int id)
{
    int i, t;
    char buf[64] = { 0 };

    do {
        clear_eol(id);

        // piensa en algo...
        sprintf(buf, "..oO (%s)", topic[t = rand() % M]);

        // imprime lo que piensa
        for (i = 0; buf[i]; i++) {
            print(id, i + 18, "%c", buf[i]);
            if (i < 5)
                sleep(1);
        }

        sleep(2 + rand() % 2);
    } while (t);
}

void *filosofo(void *p)
{
    int id = *(int *) p;
    print(id, 1, "%15s", names[id]);
    while (1) {
        think(id);
        eat(id);
    }
}

int main()
{
    int i;
    int id[N]; // id para cada filosofo
    pthread_t tid[N]; // hilos

    clear();

    for (i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(tid + i, 0, filosofo, id + i);
    }

    pthread_exit(0);
}

