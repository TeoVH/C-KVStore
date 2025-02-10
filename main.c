#include <stdio.h>
#include "kv_store.h"

int main() {
    // Crear tabla hash
    HashTable *table = createHashTable();
    printf("Tabla hash creada\n");

    // Insertar datos
    insert(table, "steam_id_123", "Counter-Strike");
    insert(table, "steam_id_456", "Dota 2");

    // Verificar inserción con get()
    printf("User steam_id_123 plays: %s\n", get(table, "steam_id_123"));
    printf("User steam_id_456 plays: %s\n", get(table, "steam_id_456"));

    freeHashTable(table);
    return 0;
}

