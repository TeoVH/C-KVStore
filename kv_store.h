// kv_store.h - Definición de estructuras y funciones
#ifndef KV_STORE_H
#define KV_STORE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_SIZE 1000003 // Número primo cercano para reducir colisiones

// Estructura para almacenar juegos
typedef struct {
    int app_id;
    char title[256];
    int positive_ratio;
    int user_reviews;
} Game;

// Estructura para almacenar usuarios
typedef struct {
    int user_id;
    int recommendations;
} User;

// Estructura para almacenar recomendaciones
typedef struct {
    int app_id;
    int user_id;
    bool is_recommended;
    float hours;
    int review_id;
} Recommendation;

// Nodo de la tabla hash
typedef struct HashNode {
    int key;
    void *value;
    struct HashNode *next;
} HashNode;

// Estructura de la tabla hash
typedef struct {
    HashNode **buckets;
} HashTable;

// Funciones para manejar la tabla hash
HashTable* create_hash_table();
void insert(HashTable *table, int key, void *value);
void* search(HashTable *table, int key);
void delete(HashTable *table, int key);
void free_hash_table(HashTable *table);

// Funciones para manejar los datos del Key-Value Store
void load_games(const char *filename, HashTable *table);
void load_users(const char *filename, HashTable *table);
void load_recommendations(const char *filename, HashTable *table);
void verify_hash_table(HashTable *table);

#endif // KV_STORE_H

