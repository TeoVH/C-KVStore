// kv_store.h - Definición de estructuras y funciones
#ifndef KV_STORE_H
#define KV_STORE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_SIZE 2000003 // Número primo cercano para reducir colisiones

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

// Estructura auxiliar para ordenar juegos
typedef struct {
    int app_id;
    int count;
} GameCount;

// Estructura auxiliar para usuarios (para la consulta)
typedef struct {
    int user_id;
    int reviews;
} UserCount;

// Funciones para manejar la tabla hash
HashTable* create_hash_table();
void insert(HashTable *table, int key, void *value);
void* search(HashTable *table, int key);
void delete(HashTable *table, int key);
void free_hash_table(HashTable *table);
void debug_hash_table(HashTable *table);

// Funciones para cargar los archivos CSV en las tablas hash
void load_games(const char *filename, HashTable *table);
void load_users(const char *filename, HashTable *table);
void load_recommendations(const char *filename, HashTable *table);

// Función para obtener el título de un juego a partir de su app_id
const char* get_game_title(HashTable *table, int app_id);
int compare_games(const void *a, const void *b);
void top_10_most_recommended(HashTable *table_games, HashTable *table_recommendations);
void top_games_by_top_users(HashTable *table_games, HashTable *table_recommendations, HashTable *table_users);
void top_10_users_most_reviews(HashTable *table_users);

#endif // KV_STORE_H

