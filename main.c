// main.c - Menú interactivo para consultas
#include "kv_store.h"
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
                top_10_most_recommended(table_games, table_recommendations);
                break;
            case 'b':
                printf("Mostrando los 10 juegos menos recomendados...\n");
                top_10_least_recommended(table_games, table_recommendations);
                break;
            case 'c':
                printf("Mostrando los 10 usuarios con más recomendaciones...\n");
                top_10_users_most_reviews(table_users);
                break;
            case 'd':
                printf("Mostrando los juegos que más recomiendan los 10 usuarios...\n");
                // Implementar consulta para usuarios
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
