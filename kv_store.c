#include <stdio.h>  // Biblioteca estándar de entrada y salida
#include <stdlib.h> // Biblioteca estándar de funciones de utilidad
#include <string.h> // Biblioteca para manipulación de cadenas
#include "kv_store.h" // Archivo de cabecera con las declaraciones de funciones y estructuras

// Función para inicializar la tabla hash
HashTable* createHashTable() {
    HashTable *table = (HashTable*)malloc(sizeof(HashTable)); // Asignar memoria para la tabla hash
    if (!table) { // Verificar si la asignación de memoria falló
        fprintf(stderr, "Error al asignar memoria para la tabla hash\n");
        exit(EXIT_FAILURE); // Terminar el programa si hay un error
    }
    for (int i = 0; i < TABLE_SIZE; i++) { // Inicializar todos los buckets a NULL
        table->buckets[i] = NULL;
    }
    return table; // Devolver el puntero a la tabla hash creada
}

// Función para liberar la memoria de la tabla hash
void freeHashTable(HashTable *table) {
    if (!table) return; // Si la tabla es NULL, no hacer nada

    for (int i = 0; i < TABLE_SIZE; i++) { // Recorrer todos los buckets
        Node *current = table->buckets[i];
        while (current) { // Liberar todos los nodos en la lista enlazada
            Node *temp = current;
            current = current->next;
            free(temp->key);   // Liberar memoria de la clave
            free(temp->value); // Liberar memoria del valor
            free(temp);        // Liberar el nodo
        }
    }
    free(table);  // Liberar la tabla hash
}

// Función para insertar un par clave-valor en la tabla hash
void insert(HashTable *table, const char *key, const char *value) {
    if (!table || !key || !value) return;  // Verificar que los punteros sean válidos

    unsigned int index = hashFunction(key);  // Obtener índice con función hash

    // Crear un nuevo nodo para la clave-valor
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) { // Verificar si la asignación de memoria falló
        fprintf(stderr, "Error al asignar memoria para el nodo\n");
        exit(EXIT_FAILURE);
    }

    // Copiar clave y valor en memoria dinámica
    newNode->key = strdup(key);
    newNode->value = strdup(value);
    newNode->next = NULL;

    // Insertar en la tabla hash (manejo de colisiones con encadenamiento)
    if (table->buckets[index] == NULL) {
        table->buckets[index] = newNode; // Si no hay colisión, insertar directamente
    } else {
        // Agregar al inicio de la lista enlazada en caso de colisión
        newNode->next = table->buckets[index];
        table->buckets[index] = newNode;
    }
}

// Función hash para obtener el índice de una clave
unsigned int hashFunction(const char *key) {
    unsigned long hash = 5381; // Valor inicial del hash
    int c;
    while ((c = *key++)) { // Calcular el hash usando el algoritmo DJB2
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }
    return hash % TABLE_SIZE;  // Devolver índice válido
}

// Función para obtener el valor asociado a una clave
char* get(HashTable *table, const char *key) {
    if (!table || !key) return NULL; // Verificar que los punteros sean válidos

    unsigned int index = hashFunction(key); // Obtener índice con función hash
    Node *current = table->buckets[index];

    while (current) { // Recorrer la lista enlazada en el bucket correspondiente
        if (strcmp(current->key, key) == 0) { // Si la clave coincide, devolver el valor
            return current->value;
        }
        current = current->next;
    }
    return NULL;  // Si la clave no se encuentra, devolver NULL
}