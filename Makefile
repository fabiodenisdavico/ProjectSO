CC = gcc
CFLAGS = -Wall -g -I./include

SRC = src/main.c src/operatore.c src/utente.c src/ipc_utils.c src/piatti.c 
#aggiunto piatti.c da Fabio il 26 12
TARGET = mensa

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)
	@echo "Compilato!"

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f src/*.o
	./clean_ipc.sh

.PHONY: all run clean