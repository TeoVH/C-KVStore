# C-KVStore 🚀  

Un **Key-Value Store de alto rendimiento** implementado en **C**, diseñado para almacenamiento en memoria y recuperación rápida de datos.  
Este proyecto es parte del curso de **Sistemas Operativos (ST0257)** en **Universidad EAFIT**.  

---

## **📌 Descripción del Proyecto**  
Este proyecto consiste en diseñar e implementar un **Key-Value Store**, un componente clave en muchos sistemas distribuidos.  
El objetivo principal es **almacenar en caché datos de acceso frecuente** para mejorar el rendimiento de las aplicaciones, similar a una base de datos o un motor de recomendaciones.  

---

## **📌 Características**  
- 🔥 **Almacenamiento rápido en memoria** mediante una **tabla hash**.  
- ⚡ **Función hash optimizada** para reducir colisiones.  
- 🚀 **Manejo eficiente de memoria** (asignación dinámica y liberación adecuada).  
- 🛠 **Operaciones implementadas**:  
  - ✅ **Insertar (`insert()`)** - Almacena un par clave-valor en la tabla hash.  
  - ✅ **Buscar (`get()`)** - Recupera un valor dada una clave.  
  - ✅ **Eliminar (`delete()`)** - Elimia un valor dada una clave.  

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
./kv_store
```
