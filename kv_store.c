#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "kv_store.h"

// -------------------- Funciones para calcular el tamaño de la tabla hash dinámicamente --------------------

int isPrime(size_t n) {
    if (n < 2) return 0;
    for (size_t i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

size_t nextPrime(size_t n) {
    while (!isPrime(n)) {
        n++;
    }
    return n;
}

size_t estimateNumLines(size_t filesize, size_t avgLineSize) {
    return filesize / avgLineSize;
}

size_t computeTableSize(size_t numLines) {
    size_t estimatedSize = (size_t)(numLines * 1.3);
    return nextPrime(estimatedSize);
}

HashTable* createHashTableForFile(const char *filename, size_t avgLineSize) {
    struct stat st;
    if (stat(filename, &st) != 0) {
        perror("Error obteniendo tamaño del archivo");
        return NULL;
    }
    size_t filesize = st.st_size;
    size_t numLines = estimateNumLines(filesize, avgLineSize);
    size_t tableSize = computeTableSize(numLines);

    HashTable *table = malloc(sizeof(HashTable));
    if (!table) {
        fprintf(stderr, "Error asignando memoria para la tabla hash\n");
        exit(EXIT_FAILURE);
    }
    table->size = tableSize;
    table->buckets = calloc(tableSize, sizeof(Node*));
    if (!table->buckets) {
        fprintf(stderr, "Error asignando memoria para los buckets\n");
        free(table);
        exit(EXIT_FAILURE);
    }
    return table;
}

// -------------------- Funciones de la Tabla Hash --------------------

void freeHashTable(HashTable *table) {
    if (!table) return;
    for (size_t i = 0; i < table->size; i++) {
        Node *current = table->buckets[i];
        while (current) {
            Node *temp = current;
            current = current->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

unsigned int hashFunction(const char *key, HashTable *table) {
    unsigned int hash = 2166136261u;
    while (*key) {
        hash ^= (unsigned char)*key++;
        hash *= 16777619;
    }
    return hash % table->size;
}

void insert(HashTable *table, const char *key, const char *value) {
    if (!table || !key || !value) return;
    unsigned int index = hashFunction(key, table);

    Node *current = table->buckets[index];
    while (current) {
        if (strcmp(current->key, key) == 0)
            return;
        current = current->next;
    }

    Node *newNode = malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "Error al asignar memoria para el nodo\n");
        exit(EXIT_FAILURE);
    }
    newNode->key = strdup(key);
    newNode->value = strdup(value);
    newNode->next = table->buckets[index];
    table->buckets[index] = newNode;
}

char* get(HashTable *table, const char *key) {
    unsigned int index = hashFunction(key, table);
    Node *current = table->buckets[index];
    while (current) {
        if (strcmp(current->key, key) == 0)
            return current->value;
        current = current->next;
    }
    return NULL;
}

void delete(HashTable *table, const char *key) {
    if (!table || !key) return;
    unsigned int index = hashFunction(key, table);
    Node *current = table->buckets[index];
    Node *prev = NULL;
    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (prev)
                prev->next = current->next;
            else
                table->buckets[index] = current->next;
            free(current->key);
            free(current->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// -------------------- Funciones para Cargar CSVs --------------------

// Carga games.csv usando fgets()
// Formato esperado: app_id,title,...
void loadGamesCSV(HashTable *table, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error abriendo games.csv");
        return;
    }
    char line[4096];
    // Saltar el encabezado
    if (!fgets(line, sizeof(line), file)) {
        perror("Error leyendo encabezado de games.csv");
        fclose(file);
        return;
    }
    while (fgets(line, sizeof(line), file)) {
        char key[100], value[200];
        if (sscanf(line, "%99[^,],%199[^,\n]", key, value) == 2) {
            insert(table, key, value);
        }
    }
    fclose(file);
}

// Carga users.csv usando lectura en bloques alineados a 4096 bytes (por ejemplo, usando un buffer de 256 KB)
void loadUsersCSV(HashTable *table, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error abriendo users.csv");
        return;
    }
    
    // Usaremos un buffer de 256 KB, que es 262144 bytes (múltiplo de 4096)
    const size_t BUFSIZE = 262144;
    char *buffer = malloc(BUFSIZE);
    if (!buffer) {
        perror("Error asignando buffer para users.csv");
        fclose(file);
        return;
    }
    
    size_t dataLen = 0;  // Longitud de los datos pendientes de procesar
    
    // Leer y descartar el encabezado
    if (fgets(buffer, BUFSIZE, file) == NULL) {
        perror("Error leyendo encabezado de users.csv");
        free(buffer);
        fclose(file);
        return;
    }
    // Procesar bloque por bloque
    while (!feof(file)) {
        size_t bytesRead = fread(buffer + dataLen, 1, BUFSIZE - dataLen - 1, file);
        if (bytesRead == 0)
            break;
        dataLen += bytesRead;
        buffer[dataLen] = '\0';
        
        // Procesar líneas completas en el buffer
        char *line_start = buffer;
        char *newline;
        while ((newline = strchr(line_start, '\n')) != NULL) {
            *newline = '\0';
            char key[100], value[100];
            // Formato esperado: user_id,products,reviews
            if (sscanf(line_start, "%99[^,],%99[^,\n]", key, value) == 2) {
                insert(table, key, value);
            }
            line_start = newline + 1;
        }
        // Mover los datos incompletos al inicio del buffer
        dataLen = strlen(line_start);
        memmove(buffer, line_start, dataLen + 1);
    }
    
    free(buffer);
    fclose(file);
}

// Función loadCSV que decide qué CSV cargar basado en el nombre del archivo
void loadCSV(HashTable *table, const char *filename) {
    if (strstr(filename, "games")) {
        loadGamesCSV(table, filename);
    } else if (strstr(filename, "users")) {
        loadUsersCSV(table, filename);
    } else {
        printf("Archivo desconocido: %s\n", filename);
    }
}

