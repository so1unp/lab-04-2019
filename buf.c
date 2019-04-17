// 
// Ejercicio 3 - Problema del búfer limitado modificado
// 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>

// Tamaño del buffer
#define BUFFER_SIZE 10

// Mueve el cursor a la linea x, columna y (ver codigos ANSI)
#define xy(x, y) printf("\033[%d;%dH", x, y)

// Borra desde el cursor hasta el fin de la linea
#define clear_eol(x) print(x, 10, "\033[K")

// Borra la pantalla
#define clear() printf("\033[H\033[J")

// Mutex para sincronizar el acceso a la pantalla
static pthread_mutex_t screen = PTHREAD_MUTEX_INITIALIZER;

static void* writer(void*);
static void* reader(void*);
static void setup_draw(void);
static void draw(int, int, int);

int counter;
int buffer[BUFFER_SIZE]; // buffer de lectura y escritura (circular)

int out = 0; // indice que usa el lector 
int in = 0;  // indice que usa el productor

// Semaforos y mutex para utilizar en la sincronización
sem_t sem_writer;
sem_t sem_reader;
pthread_mutex_t mutex;

// Puntero a un buffer donde se almacena que leyo el consumidor
int *reader_results;

// Productor / Escritor
static void* writer(void *p) 
{
    int item = 0;  // item a producir
    int num = 0;   // cantidad de items producidos
    
    while (num < counter) {
        // Agrega un item al buffer
        buffer[in] = item;
        draw(4, in, item);

        // Valor del proximo item
        item = item + 1;
        // Indice del buffer para el proximo item
        in = (in + 1) % BUFFER_SIZE; 

        num++;

        sleep(rand() % 3);
    }
    
    pthread_exit(0);
}

// Consumidor / Lector
static void* reader(void *p) 
{
    int num = 0;
    
    while (num < counter) {
        // Lee un item del buffer
        int item = buffer[out];
        draw(5, out, item);

        reader_results[num] = item;

        // Indice del próximo elemento
        out = (out + 1) % BUFFER_SIZE;
        num++;

        sleep(rand() % 3);
    }
    
    pthread_exit(0);
}

// Datos iniciales en la pantalla
static void setup_draw(void)
{
    int i;
    xy(2,1), printf("Buffer:");
    xy(4,1), printf("Prod:");
    xy(5,1), printf("Cons:");

    xy(2,10);
    for(i = 0; i < BUFFER_SIZE; i++) {
        xy(2,10+(i*4)), printf("-");
    }

    xy(4,10), printf("^");
    xy(5,10), printf("^");

    xy(6,1), fflush(stdout);
}

// Dibuja el símbolo ^ y el valor producido si es el productor
static void draw(int w, int i, int v)
{
    pthread_mutex_lock(&screen);

    xy(w, 10), printf("\033[K");
    xy(w, 10+(i*4));
    printf("^");
    if (w==4) {
        xy(2,10+(i*4));
        printf("%d",v);
    }
    xy(6,1), fflush(stdout);
    
    pthread_mutex_unlock(&screen);
}

int main(int argc, char** argv) 
{
    int i;
    pthread_t writer_t;
    pthread_t reader_t;

    if (argc < 2) {
        fprintf(stderr, "Uso: %s items\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Borra la pantalla
    clear();

    // contador de items a producir
    counter = atoi(argv[1]);

    // Reserva memoria para guardar lo que lee el consumidor
    reader_results = (int*) malloc(sizeof(int)*counter);

    // Imprime en pantalla datos iniciales
    setup_draw();
    
    // Crea el productor
    pthread_create(&writer_t, NULL, writer, NULL); 
    // Crea el consumidor
    pthread_create(&reader_t, NULL, reader, NULL); 

    // Espera a que ambos hilos finalicen
    pthread_join(reader_t, NULL);  
    pthread_join(writer_t, NULL);

    xy(7,1);
    
    // Imprime en pantalla lo que leyó el consumidor
    printf("Consumidor leyo: ");
    for (i = 0; i < counter; i++) {
        printf("%d ", reader_results[i]);
    }
    printf("\n");
    
    pthread_exit(0);
}
