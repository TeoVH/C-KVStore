#include <stdio.h>
#include "kv_store.h"

int main() {
    // Crear HashTable
    HashTable *table = createHashTable();
    printf("HashTable creada\n");

    // Insertar datos
    insert(table, "steam_id_123", "Counter-Strike");
    insert(table, "steam_id_456", "Dota 2");

    // Verificar inserción
    printf("Before deletion:\n");
    printf("User steam_id_123 plays: %s\n", get(table, "steam_id_123"));

    // Eliminar clave
    delete(table, "steam_id_123");

    // Verificar eliminación
    printf("\nAfter deletion:\n");
    char *result = get(table, "steam_id_123");
    if (result) {
        printf("User steam_id_123 plays: %s\n", result);
    } else {
        printf("User steam_id_123 not found\n");
    }

    freeHashTable(table);
    return 0;
}
