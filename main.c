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
    
    HashTable *table = create_hash_table();
    
    printf("Cargando recommendations.csv...\n");
    load_recommendations("FOLDER/recommendations.csv", table);
    printf("Cargando games.csv...\n");
    load_games("FOLDER/games.csv", table);
    printf("Cargando users.csv...\n");
    load_users("FOLDER/users.csv", table);
    printf("Carga completada.\n");
    
    char opcion;
    do {
        mostrar_menu();
        printf("Seleccione una opción: ");
        scanf(" %c", &opcion);
        
        switch (opcion) {
            case 'a':
                printf("Mostrando los 10 juegos más recomendados...\n");
                break;
            case 'b':
                printf("Mostrando los 10 juegos menos recomendados...\n");
                break;
            case 'c':
                printf("Mostrando los 10 usuarios con más recomendaciones...\n");
                break;
            case 'd':
                printf("Mostrando los juegos que más recomiendan los 10 usuarios...\n");
                break;
            case 'e':
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (opcion != 'e');
    
    free_hash_table(table);
    return 0;
}
