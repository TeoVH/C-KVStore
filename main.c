#include <stdio.h>
#include "kv_store.h"

int main() {
    HashTable *table = createHashTable();
    printf("Tabla hash creada con éxito\n");
    freeHashTable(table);
    return 0;
}

