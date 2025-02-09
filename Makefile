CC = gcc
CFLAGS = -Wall -Wextra -O3
TARGET = kv_store

all: $(TARGET)

$(TARGET): main.o kv_store.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o kv_store.o

main.o: main.c kv_store.h
	$(CC) $(CFLAGS) -c main.c

kv_store.o: kv_store.c kv_store.h
	$(CC) $(CFLAGS) -c kv_store.c

clean:
	rm -f *.o $(TARGET)

