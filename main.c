#include <stdio.h>
#include <time.h>
#include "kv_store.h"

int main() {
    // Crear la tabla hash en función del tamaño del archivo games.csv (promedio estimado de 100 bytes por línea)
    HashTable *table = createHashTableForFile("games.csv", 100);
    if (!table) {
        fprintf(stderr, "Error creando la tabla hash\n");
        return 1;
    }

    // Medir tiempo de carga de games.csv
    clock_t start = clock();
    loadCSV(table, "games.csv");
    clock_t end = clock();
    printf("Tiempo de carga (games.csv): %f segundos\n", (double)(end - start) / CLOCKS_PER_SEC);

    // Medir tiempo de carga de users.csv
    start = clock();
    loadCSV(table, "users.csv");
    end = clock();
    printf("Tiempo de carga (users.csv): %f segundos\n", (double)(end - start) / CLOCKS_PER_SEC);

    // Verificar algunos datos de games.csv (app_id -> title)
    printf("\nVerificación de games.csv:\n");
    printf("Juego 13500: %s\n", get(table, "13500"));
    printf("Juego 22364: %s\n", get(table, "22364"));
    printf("Juego 113020: %s\n", get(table, "113020"));

    // Verificar algunos datos de users.csv (user_id -> products)
    printf("\nVerificación de users.csv:\n");
    printf("Usuario 7360263: %s\n", get(table, "7360263"));
    printf("Usuario 14020781: %s\n", get(table, "14020781"));
    printf("Usuario 8762579: %s\n", get(table, "8762579"));

    freeHashTable(table);
    return 0;
}

