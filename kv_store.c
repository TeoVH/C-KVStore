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

void insert(HashTable *table, const char *key, const char *value) {
    if (!table || !key || !value) return;  // Verificar que los punteros sean válidos

    unsigned int index = hashFunction(key);  // Obtener índice con función hash

    // Crear un nuevo nodo para la clave-valor
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "Error al asignar memoria para el nodo\n");
        exit(EXIT_FAILURE);
    }

    // Copiar clave y valor en memoria dinámica
    newNode->key = strdup(key);
    newNode->value = strdup(value);
    newNode->next = NULL;

    // Insertar en la tabla hash (manejo de colisiones con encadenamiento)
    if (table->buckets[index] == NULL) {
        table->buckets[index] = newNode;
    } else {
        // Agregar al inicio de la lista enlazada en caso de colisión
        newNode->next = table->buckets[index];
        table->buckets[index] = newNode;
    }
}

unsigned int hashFunction(const char *key) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c (DJB2)
    }
    return hash % TABLE_SIZE;  // Devolver índice válido
}

char* get(HashTable *table, const char *key) {
    if (!table || !key) return NULL;

    unsigned int index = hashFunction(key);
    Node *current = table->buckets[index];

    while (current) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;  // Si la clave no se encuentra
}

void delete(HashTable *table, const char *key) {
    if (!table || !key) return;  // Verificar que la tabla y la clave sean válidas

    unsigned int index = hashFunction(key);  // Obtener índice en la tabla hash
    Node *current = table->buckets[index];
    Node *prev = NULL;

    while (current) {
        if (strcmp(current->key, key) == 0) {  // Si encontramos la clave
            if (prev) {
                prev->next = current->next;  // Saltar el nodo actual
            } else {
                table->buckets[index] = current->next;  // Si es el primer nodo, actualizar el bucket
            }
            free(current->key);  // Liberar memoria de la clave
            free(current->value);  // Liberar memoria del valor
            free(current);  // Liberar el nodo
            return;  // Salir de la función después de eliminar
        }
        prev = current;
        current = current->next;
    }
}

