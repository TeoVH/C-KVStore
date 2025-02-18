#ifndef KV_STORE_H
#define KV_STORE_H

#include <stdint.h>
#include <sys/types.h>

// Prototipo para obtener el siguiente número primo (para la tabla hash dinámica)
size_t nextPrime(size_t n);

// Estructura de un nodo (bucket) en la tabla hash
typedef struct Node {
    char *key;
    char *value;
    struct Node *next;
} Node;

// Estructura de la tabla hash (dinámica)
typedef struct {
    size_t size;    // Tamaño de la tabla hash (calculado en tiempo de ejecución)
    Node **buckets; // Array dinámico de punteros a Node
} HashTable;

// Funciones principales de la tabla hash
HashTable* createHashTableForFile(const char *filename, size_t avgLineSize);
void freeHashTable(HashTable *table);
void insert(HashTable *table, const char *key, const char *value);
unsigned int hashFunction(const char *key, HashTable *table);
char* get(HashTable *table, const char *key);
void delete(HashTable *table, const char *key);

// Funciones para cargar CSVs
void loadCSV(HashTable *table, const char *filename);
void loadGamesCSV(HashTable *table, const char *filename);
void loadUsersCSV(HashTable *table, const char *filename);

#endif

