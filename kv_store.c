// kv_store.c - Implementación de la tabla hash y funciones
#include "kv_store.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definir el tamaño de la tabla hash
#define HASH_TABLE_SIZE 2000003 // Número primo optimizado

// Función hash mejorada usando mezcla de bits
unsigned int hash(int key) {
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = (key >> 16) ^ key;
    return key % HASH_TABLE_SIZE;
}

// Función para crear una tabla hash
HashTable* create_hash_table() {
    HashTable *table = (HashTable*)malloc(sizeof(HashTable));
    table->buckets = (HashNode**)calloc(HASH_TABLE_SIZE, sizeof(HashNode*));
    return table;
}

// Insertar un elemento en la tabla hash
void insert(HashTable *table, int key, void *value) {
    if (key == 0) return; // Evitar claves inválidas
    unsigned int index = hash(key);
    HashNode *newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->key = key;
    newNode->value = value;
    newNode->next = table->buckets[index];
    table->buckets[index] = newNode;
}

// Buscar un elemento en la tabla hash
void* search(HashTable *table, int key) {
    if (key == 0) return NULL;
    unsigned int index = hash(key);
    HashNode *node = table->buckets[index];
    while (node) {
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

// Función para depurar la tabla hash (para desarrollo)
void debug_hash_table(HashTable *table) {
    int total_elements = 0;
    int non_empty_buckets = 0;
    int max_bucket_size = 0;
    
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        int bucket_size = 0;
        HashNode *node = table->buckets[i];
        while (node) {
            total_elements++;
            bucket_size++;
            node = node->next;
        }
        if (bucket_size > 0) {
            non_empty_buckets++;
            if (bucket_size > max_bucket_size) {
                max_bucket_size = bucket_size;
            }
        }
    }
    
    printf("\n[DEBUG] Tabla Hash:\n");
    printf("Total de elementos almacenados: %d\n", total_elements);
    printf("Buckets no vacíos: %d\n", non_empty_buckets);
    printf("Tamaño máximo en un bucket: %d\n", max_bucket_size);
    
    // Imprimir algunas claves almacenadas
    printf("\nEjemplo de claves almacenadas:\n");
    int printed = 0;
    for (int i = 0; i < HASH_TABLE_SIZE && printed < 5; i++) {
        if (table->buckets[i]) {
            printf("Bucket %d: Clave %d\n", i, table->buckets[i]->key);
            printed++;
        }
    }
}

// Función para cargar recommendations.csv en la tabla hash
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
        
        if (rec->user_id != 0 && rec->app_id != 0) { // Validar claves
            insert(table, rec->user_id, rec);
        } else {
            free(rec);
        }
    }
    
    fclose(file);
    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Tiempo de carga de recommendations.csv: %.3f segundos\n", elapsed_time);
}

// Función para cargar games.csv en la tabla hash
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
        
        insert(table, game->app_id, game);
    }
    
    fclose(file);
    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Tiempo de carga de games.csv: %.3f segundos\n", elapsed_time);
}

// Función para cargar users.csv en la tabla hash
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
	strtok(NULL, ",");
        user->recommendations = atoi(strtok(NULL, ","));
        
        insert(table, user->user_id, user);
    }
    
    fclose(file);
    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Tiempo de carga de users.csv: %.3f segundos\n", elapsed_time);
}

// Función para obtener el título de un juego dado su app_id
const char* get_game_title(HashTable *table, int app_id) {
    Game *game = (Game*)search(table, app_id);
    if (!game) {
        return "Juego no encontrado";
    }
    return game->title;
}

// Función de comparación para ordenar juegos de mayor a menor (por número de recomendaciones)
int compare_games(const void *a, const void *b) {
    return ((GameCount *)b)->count - ((GameCount *)a)->count;
}

// Función para encontrar los 10 juegos más recomendados y mostrar sus nombres
void top_10_most_recommended(HashTable *table_games, HashTable *table_recommendations) {
    printf("\nLos 10 juegos más recomendados:\n");
    
    // Inicializar un array dinámico para almacenar los juegos y sus conteos
    int capacity = 10000;
    GameCount *games = malloc(capacity * sizeof(GameCount));
    if (!games) {
        printf("Error: No se pudo asignar memoria para juegos.\n");
        return;
    }
    int game_count = 0;
    
    // Recorrer la tabla hash de recomendaciones
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *node = table_recommendations->buckets[i];
        while (node) {
            Recommendation *rec = (Recommendation *)node->value;
            if (rec && rec->is_recommended) {
                int found = 0;
                // Buscar si ya se registró este juego en el arreglo
                for (int j = 0; j < game_count; j++) {
                    if (games[j].app_id == rec->app_id) {
                        games[j].count++;
                        found = 1;
                        break;
                    }
                }
                // Si no se encontró, agregarlo al arreglo
                if (!found) {
                    // Reasignar memoria si se ha alcanzado la capacidad
                    if (game_count >= capacity) {
                        capacity *= 2;
                        GameCount *temp = realloc(games, capacity * sizeof(GameCount));
                        if (!temp) {
                            printf("Error: No se pudo reasignar memoria.\n");
                            free(games);
                            return;
                        }
                        games = temp;
                    }
                    games[game_count].app_id = rec->app_id;
                    games[game_count].count = 1;
                    game_count++;
                }
            }
            node = node->next;
        }
    }
    
    // Ordenar el arreglo por número de recomendaciones (de mayor a menor)
    qsort(games, game_count, sizeof(GameCount), compare_games);
    
    // Imprimir los 10 juegos más recomendados junto con sus nombres
    for (int i = 0; i < 10 && i < game_count; i++) {
        const char *title = get_game_title(table_games, games[i].app_id);
        printf("Juego: %s (ID: %d), Recomendaciones: %d\n", title, games[i].app_id, games[i].count);
    }
    
    free(games);
}

// Función de comparación para ordenar juegos de menor a mayor (por número de recomendaciones)
int compare_games_asc(const void *a, const void *b) {
    return ((GameCount *)a)->count - ((GameCount *)b)->count;
}

// Función para encontrar los 10 juegos menos recomendados y mostrar sus nombres
void top_10_least_recommended(HashTable *table_games, HashTable *table_recommendations) {
    printf("\nLos 10 juegos menos recomendados:\n");

    // Crear un array dinámico para almacenar los juegos y sus conteos
    int capacity = 10000;
    GameCount *games = malloc(capacity * sizeof(GameCount));
    if (!games) {
        printf("Error: No se pudo asignar memoria para juegos.\n");
        return;
    }
    int game_count = 0;

    // Recorrer la tabla hash de recomendaciones
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *node = table_recommendations->buckets[i];
        while (node) {
            Recommendation *rec = (Recommendation *)node->value;
            if (rec && !rec->is_recommended) {
                int found = 0;
                // Buscar si ya se registró este juego en el arreglo
                for (int j = 0; j < game_count; j++) {
                    if (games[j].app_id == rec->app_id) {
                        games[j].count++;
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    if (game_count >= capacity) {
                        capacity *= 2;
                        GameCount *temp = realloc(games, capacity * sizeof(GameCount));
                        if (!temp) {
                            printf("Error: No se pudo reasignar memoria.\n");
                            free(games);
                            return;
                        }
                        games = temp;
                    }
                    games[game_count].app_id = rec->app_id;
                    games[game_count].count = 1;
                    game_count++;
                }
            }
            node = node->next;
        }
    }

    // Ordenar los juegos por número de recomendaciones de menor a mayor
    qsort(games, game_count, sizeof(GameCount), compare_games);

    // Imprimir los 10 juegos menos recomendados junto con sus nombres
    for (int i = 0; i < 10 && i < game_count; i++) {
        const char *title = get_game_title(table_games, games[i].app_id);
        printf("Juego: %s (ID: %d), Recomendaciones: %d\n", title, games[i].app_id, games[i].count);
    }

    free(games);
}

// Función de comparación para ordenar usuarios de mayor a menor según reviews
int compare_users(const void *a, const void *b) {
    return ((UserCount *)b)->reviews - ((UserCount *)a)->reviews;
}

// Función para encontrar los 10 usuarios con más reviews y mostrarlos
void top_10_users_most_reviews(HashTable *table_users) {
    printf("\nLos 10 usuarios con más reviews:\n");

    // Crear un array dinámico para almacenar los usuarios y sus reviews.
    // Se asume que no habrá más de 10,000 usuarios distintos (se expande si es necesario)
    int capacity = 10000;
    UserCount *usersArr = malloc(capacity * sizeof(UserCount));
    if (!usersArr) {
        printf("Error: No se pudo asignar memoria para usuarios.\n");
        return;
    }
    int user_count = 0;

    // Recorrer la tabla hash de usuarios
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *node = table_users->buckets[i];
        while (node) {
            User *user = (User *)node->value;
            if (user) {
                // Si se alcanza la capacidad, se expande el arreglo
                if (user_count >= capacity) {
                    capacity *= 2;
                    UserCount *temp = realloc(usersArr, capacity * sizeof(UserCount));
                    if (!temp) {
                        printf("Error: No se pudo reasignar memoria para usuarios.\n");
                        free(usersArr);
                        return;
                    }
                    usersArr = temp;
                }
                // Almacenar la información relevante para la consulta
                usersArr[user_count].user_id = user->user_id;
                usersArr[user_count].reviews = user->recommendations;
                user_count++;
            }
            node = node->next;
        }
    }

    // Ordenar los usuarios por número de reviews en forma descendente
    qsort(usersArr, user_count, sizeof(UserCount), compare_users);

    // Imprimir los 10 primeros
    for (int i = 0; i < 10 && i < user_count; i++) {
        printf("Usuario ID: %d, Reviews: %d\n", usersArr[i].user_id, usersArr[i].reviews);
    }

    free(usersArr);
}

// Función para encontrar los juegos que más recomiendan los 10 usuarios con más reviews
void top_games_by_top_users(HashTable *table_games, HashTable *table_recommendations, HashTable *table_users) {
    printf("\nTop 10 Juegos Recomendados por los 10 Usuarios con Más Reviews:\n");

    // 1. Recopilar todos los usuarios en un arreglo dinámico
    int capacity_users = 10000;
    UserCount *usersArr = malloc(capacity_users * sizeof(UserCount));
    if (!usersArr) {
        printf("Error: No se pudo asignar memoria para usuarios.\n");
        return;
    }
    int total_users = 0;
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *node = table_users->buckets[i];
        while (node) {
            User *user = (User *)node->value;
            if (user) {
                if (total_users >= capacity_users) {
                    capacity_users *= 2;
                    UserCount *temp = realloc(usersArr, capacity_users * sizeof(UserCount));
                    if (!temp) {
                        printf("Error: No se pudo reasignar memoria para usuarios.\n");
                        free(usersArr);
                        return;
                    }
                    usersArr = temp;
                }
                usersArr[total_users].user_id = user->user_id;
                usersArr[total_users].reviews = user->recommendations;
                total_users++;
            }
            node = node->next;
        }
    }

    // Ordenar usuarios de mayor a menor según reviews
    qsort(usersArr, total_users, sizeof(UserCount), compare_users);

    // Extraer los 10 usuarios con más reviews
    int top_n = (total_users < 10) ? total_users : 10;
    int top_user_ids[10];
    for (int i = 0; i < top_n; i++) {
        top_user_ids[i] = usersArr[i].user_id;
    }
    free(usersArr);

    // 2. Filtrar recomendaciones: contar las recomendaciones (is_recommended==true)
    // solo de los usuarios top (top_user_ids)
    int capacity_games = 10000;
    GameCount *games = malloc(capacity_games * sizeof(GameCount));
    if (!games) {
        printf("Error: No se pudo asignar memoria para juegos.\n");
        return;
    }
    int game_count = 0;

    // Recorrer la tabla de recomendaciones
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *node = table_recommendations->buckets[i];
        while (node) {
            Recommendation *rec = (Recommendation *)node->value;
            if (rec && rec->is_recommended) {
                // Verificar si el usuario está entre los top
                int is_top = 0;
                for (int j = 0; j < top_n; j++) {
                    if (rec->user_id == top_user_ids[j]) {
                        is_top = 1;
                        break;
                    }
                }
                if (is_top) {
                    int found = 0;
                    for (int j = 0; j < game_count; j++) {
                        if (games[j].app_id == rec->app_id) {
                            games[j].count++;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        if (game_count >= capacity_games) {
                            capacity_games *= 2;
                            GameCount *temp = realloc(games, capacity_games * sizeof(GameCount));
                            if (!temp) {
                                printf("Error: No se pudo reasignar memoria para juegos.\n");
                                free(games);
                                return;
                            }
                            games = temp;
                        }
                        games[game_count].app_id = rec->app_id;
                        games[game_count].count = 1;
                        game_count++;
                    }
                }
            }
            node = node->next;
        }
    }

    // 3. Ordenar los juegos por la cantidad de recomendaciones en forma descendente
    qsort(games, game_count, sizeof(GameCount), compare_games);

    // 4. Mostrar los 10 juegos más recomendados por los top usuarios
    int top_games_n = (game_count < 10) ? game_count : 10;
    for (int i = 0; i < top_games_n; i++) {
        const char *title = get_game_title(table_games, games[i].app_id);
        printf("Juego: %s (ID: %d), Recomendaciones: %d\n", title, games[i].app_id, games[i].count);
    }

    free(games);
}

