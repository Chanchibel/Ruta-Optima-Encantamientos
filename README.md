# Ruta Óptima de Encantamientos - Minecraft

Una aplicación de escritorio diseñada para calcular la secuencia y combinación más eficiente de libros encantados en Minecraft, reduciendo al mínimo el coste de niveles de experiencia ($XP$) y evitando el molesto mensaje de **"Demasiado costoso"** (*Too Expensive*).

* **Cálculo Optimizado:** Algoritmo en C++ para encontrar el orden exacto de combinación en el yunque.
* **Interfaz Gráfica Intuitiva:** Desarrollada en C# Windows Forms para una fácil selección de ítems y niveles.
* **Personalización de Lista:** Permite agregar, editar o activar/desactivar encantamientos mediante una tabla interactiva.
* **Guardado Automático:** Persistencia de datos local utilizando archivos JSON.

---

## Tecnologías Utilizadas

* **C# (.NET / WinForms):** Capa de interfaz de usuario, gestión de datos e interacciones con la tabla.
* **C++ (DLL Nativa):** Motor de procesamiento y cálculo de rutas óptimas (PInvoke / C++/CLI).
* **JSON:** Formato de almacenamiento local para configuraciones y listas de encantamientos.

---

## Instalación y Uso

1. Ve a la sección de **[Releases (Lanzamientos)](../../releases)** en la columna derecha de este repositorio.
2. Descarga el archivo comprimido `.zip` de la versión más reciente.
3. Extrae todo el contenido del archivo comprimido en una misma carpeta.
4. Ejecuta el archivo **`Interfaz_grafica.exe`** para iniciar el programa.

> **Importante:** Mantén el archivo `Encantamientos.dll` en la misma carpeta que el ejecutable para evitar errores de ejecución.

---

## Licencia

Este proyecto fue desarrollado con fines educativos y de entretenimiento. Libre de uso y modificación.
