CC=gcc
BIN=./bin
CFLAGS=-Wall -g -pthread -lm

PROG=glob ph philo buf sem_open sem_wait sem_post sem_unlink sem_getvalue

LIST=$(addprefix $(BIN)/, $(PROG))

.PHONY: all
all: $(LIST)

$(BIN)/%: %.c
	$(CC) -o $@ $< $(CFLAGS)

%: %.c
	$(CC) -o $(BIN)/$@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(BIN)/ej* $(BIN)/hola $(BIN)/sh
