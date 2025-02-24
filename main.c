// main.c - Menú interactivo para consultas
#include "kv_store.h"
#include <time.h>
#include <dirent.h>
#include <stdlib.h>

// Función para verificar si la carpeta de datos existe
int verificar_carpeta(const char *path) {
    DIR *dir = opendir(path);
    if (dir) {
        closedir(dir);
        return 1;
    }
    return 0;
}

// Función para mostrar el menú
void mostrar_menu() {
    printf("\n--------------------------------------------------------------------------\n");
    printf("Menú de Consultas\n");
    printf("--------------------------------------------------------------------------\n");
    printf("a. Indicar cuales son los 10 juegos más recomendados\n");
    printf("b. Indicar cuales son los 10 juegos menos recomendados\n");
    printf("c. Indicar cuales son los 10 usuarios con más recomendaciones\n");
    printf("d. Indicar cuales son los juegos que más recomiendan los 10 usuarios\n");
    printf("e. Salir del programa\n");
    printf("--------------------------------------------------------------------------\n");
}

int main() {
    const char *carpeta_datos = "FOLDER";
    
    if (!verificar_carpeta(carpeta_datos)) {
        printf("Error: La carpeta '%s' no existe.\n", carpeta_datos);
        return 1;
    }
    
    // Crear tablas hash separadas para cada tipo de datos
    HashTable *table_games = create_hash_table();
    HashTable *table_recommendations = create_hash_table();
    HashTable *table_users = create_hash_table();
    
    printf("Cargando games.csv...\n");
    load_games("FOLDER/games.csv", table_games);
    printf("Cargando recommendations.csv...\n");
    load_recommendations("FOLDER/recommendations.csv", table_recommendations);
    printf("Cargando users.csv...\n");
    load_users("FOLDER/users.csv", table_users);
    printf("Carga completada.\n");
    
    // Opcional: Verificar la carga de cada tabla (para depuración)
    printf("\nVerificando la tabla hash de juegos...\n");
    debug_hash_table(table_games);
    printf("\nVerificando la tabla hash de recomendaciones...\n");
    debug_hash_table(table_recommendations);
    printf("\nVerificando la tabla hash de usuarios...\n");
    debug_hash_table(table_users);
    
    char opcion;
    do {
        mostrar_menu();
        printf("Seleccione una opción: ");
        scanf(" %c", &opcion);
        
        switch (opcion) {
            case 'a':
		clock_t start_time_a = clock();
		printf("Mostrando los 10 juegos mas recomendados");
                top_10_most_recommended(table_games, table_recommendations);
		clock_t end_time_a = clock();
		printf("Tiempo de ejecución de la consulta: %.3f segundos\n", ((double)(end_time_a - start_time_a)) / CLOCKS_PER_SEC);
                break;
            case 'b':
		clock_t start_time_b = clock();
                printf("Mostrando los 10 juegos menos recomendados...\n");
                top_10_least_recommended(table_games, table_recommendations);
		clock_t end_time_b = clock();
		printf("Tiempo de ejecución de la consulta: %.3f segundos\n", ((double)(end_time_b - start_time_b)) / CLOCKS_PER_SEC);
                break;
            case 'c':
		clock_t start_time_c = clock();
                printf("Mostrando los 10 usuarios con más recomendaciones...\n");
                top_10_users_most_reviews(table_users);
		clock_t end_time_c = clock();
		printf("Tiempo de ejecución de la consulta: %.3f segundos\n", ((double)(end_time_c - start_time_c)) / CLOCKS_PER_SEC);
                break;
            case 'd':
		clock_t start_time_d = clock();
                printf("Mostrando los juegos que más recomiendan los 10 usuarios...\n");
                top_games_by_top_users(table_games, table_recommendations, table_users);
		clock_t end_time_d = clock();
		printf("Tiempo de ejecución de la consulta: %.3f segundos\n", ((double)(end_time_d - start_time_d)) / CLOCKS_PER_SEC);
                break;
            case 'e':
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (opcion != 'e');
    
    free_hash_table(table_games);
    free_hash_table(table_recommendations);
    free_hash_table(table_users);
    
    return 0;
}
