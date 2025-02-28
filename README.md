# C-KVStore 🚀  

Un **Key-Value Store de alto rendimiento** implementado en **C**, diseñado para almacenamiento en memoria y recuperación rápida de datos.  
Este proyecto es parte del curso de **Sistemas Operativos (ST0257)** en **Universidad EAFIT**.

---

## **📌 Descripción del Proyecto**  
Este proyecto consiste en diseñar e implementar un **Key-Value Store**, un componente fundamental en sistemas distribuidos.  
El objetivo es **almacenar en caché datos de acceso frecuente** para mejorar el rendimiento de las aplicaciones, similar a lo que hace una base de datos o un motor de recomendaciones.  
Se trabaja con tres conjuntos de datos:  
- **games.csv**: Información de juegos (ID, título, reseñas, etc.).  
- **recommendations.csv**: Datos de recomendaciones de juegos, indicando si se recomienda o no.  
- **users.csv**: Información sobre los usuarios (ID y conteo de recomendaciones).

Para lograrlo, se utilizan **tablas hash separadas** para cada tipo de dato, lo que permite una gestión modular y eficiente.

---

## **📌 Características**  
- 🔥 **Almacenamiento rápido en memoria** mediante **tablas hash** separadas para juegos, recomendaciones y usuarios.  
- ⚡ **Función hash optimizada** usando mezcla de bits para minimizar colisiones.  
- 🚀 **Manejo dinámico de memoria**, con asignación inicial y expansión con `realloc()` cuando es necesario.  
- 🛠 **Operaciones implementadas**:  
  - ✅ **Insertar (`insert()`)**: Almacena un par clave-valor en la tabla hash.  
  - ✅ **Buscar (`search()`)**: Recupera un valor dada una clave.  
  - ✅ **Eliminar (`delete()`)**: Elimina un valor dada una clave.  
- 📊 Consultas de datos:
  - **Top 10 Juegos Más Recomendados:** Se acumulan las recomendaciones positivas a partir de los datos de recomendaciones y se ordenan los juegos en orden descendente según la cantidad de recomendaciones.
  - **Top 10 Juegos Menos Recomendados:** Se acumulan las recomendaciones negativas a partir de los datos de recomendaciones y se ordenan los juegos en orden ascendente según la cantidad de recomendaciones negativas.
  - **Top 10 Usuarios con Más Reviews:** Se ordenan los usuarios en función de la cantidad de reviews (del archivo users.csv) en orden descendente.
  - **Juegos Recomendados por los Top 10 Usuarios:** Se filtran las recomendaciones de los 10 usuarios con más reviews para obtener los juegos con mayor cantidad de recomendaciones entre ellos.

---

## **📌 Instalación y Compilación**  
Para configurar el proyecto, primero clona el repositorio:
```sh
git clone https://github.com/TU-USUARIO/C-KVStore.git
cd C-KVStore
```
Luego, compila el programa con make:
```sh
make
```
Después de compilar, ejecuta el programa con:
```sh
./kv_store -f FOLDER
```

---

## **📌 Uso y Ejecución**
El programa presenta un menú interactivo que permite realizar las siguientes consultas:
- a. Indicar cuáles son los 10 juegos más recomendados.
- b. Indicar cuáles son los 10 juegos menos recomendados.
- c. Indicar cuáles son los 10 usuarios con más recomendaciones.
- d. Indicar cuáles son los juegos que más recomiendan los 10 usuarios.
- e. Salir del programa.

Las consultas se realizan sobre los datos almacenados en las tablas hash, lo que permite respuestas rápidas y eficientes, incluso con grandes volúmenes de datos.
