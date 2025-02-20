#include "kv_store.h"
#include <time.h>

// Función para crear una tabla hash
HashTable* create_hash_table() {
    HashTable *table = (HashTable*)malloc(sizeof(HashTable));
    table->buckets = (HashNode**)calloc(HASH_TABLE_SIZE, sizeof(HashNode*));
    return table;
}

// Función hash mejorada usando mezcla de bits
unsigned int hash(int key) {
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = (key >> 16) ^ key;
    return key % HASH_TABLE_SIZE;
}

// Insertar un elemento en la tabla hash
void insert(HashTable *table, int key, void *value) {
    unsigned int index = hash(key);
    HashNode *newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->key = key;
    newNode->value = value;
    newNode->next = table->buckets[index];
    table->buckets[index] = newNode;
    //printf("Insertado en hash: Clave=%d, Índice=%u\n", key, index); // Debugging
}

// Buscar un elemento en la tabla hash
void* search(HashTable *table, int key) {
    unsigned int index = hash(key);
    HashNode *node = table->buckets[index];
    while (node) {
        //printf("Buscando clave: %d en índice: %u, encontrada: %d\n", key, index, node->key); // Debugging
        if (node->key == key) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

// Eliminar un elemento de la tabla hash
void delete(HashTable *table, int key) {
    unsigned int index = hash(key);
    HashNode *node = table->buckets[index];
    HashNode *prev = NULL;
    while (node) {
        if (node->key == key) {
            if (prev) {
                prev->next = node->next;
            } else {
                table->buckets[index] = node->next;
            }
            free(node);
            return;
        }
        prev = node;
        node = node->next;
    }
}

// Liberar memoria de la tabla hash
void free_hash_table(HashTable *table) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *node = table->buckets[i];
        while (node) {
            HashNode *temp = node;
            node = node->next;
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

// Función para cargar recommendations.csv en la tabla hash y medir el tiempo
void load_recommendations(const char *filename, HashTable *table) {
    clock_t start_time = clock();
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir recommendations.csv");
        return;
    }
    
    char line[512];
    fgets(line, sizeof(line), file); // Leer encabezado y descartarlo
    
    while (fgets(line, sizeof(line), file)) {
        Recommendation *rec = (Recommendation*)malloc(sizeof(Recommendation));
        
        char *token = strtok(line, ",");
        rec->app_id = atoi(token);
        token = strtok(NULL, ","); // Saltar 'helpful'
        token = strtok(NULL, ","); // Saltar 'funny'
        token = strtok(NULL, ","); // Saltar 'date'
        rec->is_recommended = strcmp(strtok(NULL, ","), "true") == 0;
        rec->hours = atof(strtok(NULL, ","));
        rec->user_id = atoi(strtok(NULL, ","));
        rec->review_id = atoi(strtok(NULL, ","));
        
        insert(table, rec->user_id, rec);
    }
    
    fclose(file);
    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Tiempo de carga de recommendations.csv: %.3f segundos\n", elapsed_time);
}

// Función para cargar games.csv en la tabla hash y medir el tiempo
void load_games(const char *filename, HashTable *table) {
    clock_t start_time = clock();
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir games.csv");
        return;
    }
    
    char line[512];
    fgets(line, sizeof(line), file); // Leer encabezado y descartarlo
    
    while (fgets(line, sizeof(line), file)) {
        Game *game = (Game*)malloc(sizeof(Game));
        
        char *token = strtok(line, ",");
        game->app_id = atoi(token);
        token = strtok(NULL, ",");
        strncpy(game->title, token, 255);
        game->title[255] = '\0';
        token = strtok(NULL, ","); // Saltar 'date_release'
        token = strtok(NULL, ","); // Saltar 'win'
        token = strtok(NULL, ","); // Saltar 'mac'
        token = strtok(NULL, ","); // Saltar 'linux'
        token = strtok(NULL, ","); // Saltar 'rating'
        game->positive_ratio = atoi(strtok(NULL, ","));
        game->user_reviews = atoi(strtok(NULL, ","));
        
        insert(table, game->app_id, game);
    }
    
    fclose(file);
    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Tiempo de carga de games.csv: %.3f segundos\n", elapsed_time);
}

// Función para cargar users.csv en la tabla hash y medir el tiempo
void load_users(const char *filename, HashTable *table) {
    clock_t start_time = clock();
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir users.csv");
        return;
    }
    
    char line[512];
    fgets(line, sizeof(line), file); // Leer encabezado y descartarlo
    
    while (fgets(line, sizeof(line), file)) {
        User *user = (User*)malloc(sizeof(User));
        
        user->user_id = atoi(strtok(line, ","));
        user->recommendations = atoi(strtok(NULL, ","));
        
        insert(table, user->user_id, user);
    }
    
    fclose(file);
    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Tiempo de carga de users.csv: %.3f segundos\n", elapsed_time);
}

void verify_hash_table(HashTable *table) {
    int count = 0;
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *node = table->buckets[i];
        while (node) {
            count++;
            if (count <= 10) { // Mostrar solo los primeros 10 elementos
                printf("Clave: %d\n", node->key);
            }
            node = node->next;
        }
    }
    printf("Total de elementos almacenados en la tabla hash: %d\n", count);
}
