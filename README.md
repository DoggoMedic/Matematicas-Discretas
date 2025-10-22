# 📚 Matematicas-Discretas: Recorrido más corto entre vértices.

Este proyecto implementa las estructuras fundamentales para el análisis de grafos y digrafos, utilizando la representación de **Matriz de Adyacencia** y aplicando el algoritmo de **Dijkstra** para encontrar el camino más corto entre dos vértices.

---

## 🚀 Uso e Instalación

### 1. Requisitos
Necesitas un compilador de C estándar que soporte C99 o posterior (como GCC o Clang).

### 2. Compilación
Compila el archivo fuente `main.c` (asumiendo que es el único archivo fuente):

```bash
gcc -o main main.c
```

### 3. Ejecución
El programa se ejecuta especificando la acción (`path`), los vértices de origen y destino, y la ruta al archivo de datos del grafo.

```bash
./main [COMANDO] [ORIGEN] [DESTINO] [ARCHIVO]
```

**Parámetros:**

| Parámetro | Índice argv | Descripción | Ejemplo |
|-----------|-------------|-------------|---------|
| `[COMANDO]` | `argv[1]` | La operación a realizar. Actualmente solo soporta `path`. | `path` |
| `[ORIGEN]` | `argv[2]` | Vértice de inicio (un solo carácter, e.g., 'a'). | `a` |
| `[DESTINO]` | `argv[3]` | Vértice de destino (un solo carácter, e.g., 'e'). | `e` |
| `[ARCHIVO]` | `argv[4]` | Ruta al archivo .txt que contiene la definición del grafo. | `data/grafo.txt` |

**Ejemplo de uso:**
```bash
./main path b e data/mi_grafo.txt
```

---

## ⚙️ Formato del Archivo de Grafo

El archivo de entrada debe ser un archivo de texto plano con exactamente tres líneas:

| Línea | Contenido | Descripción |
|-------|-----------|-------------|
| 1 | Tipo de grafo | `graph` (No dirigido) o `digraph` (Dirigido) |
| 2 | Lista de vértices | Cadena con todos los vértices del grafo. (a-z)|
| 3 | Cadena de aristas | Pares de vértices concatenados que representan las aristas o arcos. |

**Ejemplo (`grafo.txt`):**
```plaintext
digraph
abcde
abacbdce
```

---

## 🛠️ Funcionalidades

- ✅ Representación de grafos mediante matriz de adyacencia
- ✅ Soporte para grafos dirigidos y no dirigidos
- ✅ Algoritmo de Dijkstra para caminos más cortos
- ✅ Entrada/salida mediante archivos de texto

---

## 📝 Notas

- Los vértices deben ser caracteres individuales (a-z)
- El programa asume que todas las aristas tienen peso 1
- Para grafos no dirigidos, las aristas son bidireccionales
