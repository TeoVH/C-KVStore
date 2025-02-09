#ifndef KV_STORE_H
#define KV_STORE_H

#define TABLE_SIZE 10007  // Tamaño de la tabla hash (número primo para menos colisiones)

// Estructura de un nodo (bucket) en la tabla hash
typedef struct Node {
    char *key;
    char *value;
    struct Node *next;
} Node;

// Estructura de la tabla hash
typedef struct {
    Node *buckets[TABLE_SIZE];  // Array de punteros a nodos
} HashTable;

// Funciones principales
HashTable* createHashTable();
void freeHashTable(HashTable *table);

#endif
