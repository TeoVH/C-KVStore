#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kv_store.h"

// Función para inicializar la tabla hash
HashTable* createHashTable() {
    HashTable *table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) {
        fprintf(stderr, "Error al asignar memoria para la tabla hash\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->buckets[i] = NULL;
    }
    return table;
}

void freeHashTable(HashTable *table) {
    if (!table) return;

    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = table->buckets[i];
        while (current) {
            Node *temp = current;
            current = current->next;
            free(temp->key);   // Liberar memoria de la clave
            free(temp->value); // Liberar memoria del valor
            free(temp);        // Liberar el nodo
        }
    }
    free(table);  // Liberar la tabla hash
}
