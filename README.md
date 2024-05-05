# Tarea-Estructura-2

Descripcion:
Este programa se centra en la busqueda y manejo de un catálogo de películas, incluyendo la función de cargar el catalogo, filtrar por género, director, década, o buscar un id específico. 

## Cómo compilar y ejecutar:

1. Visita [Repl.it](https://repl.it/).
2. Crea una nueva cuenta o inicia sesión si ya tienes una.
3. Una vez en tu dashboard, selecciona "New Repl" y elige "Import from GitHub".
4. Pega la URL del repositorio: `(https://github.com/littlenegrito/Tarea-Estructura-2.git)`.
5. [Repl.it](http://repl.it/) clonará el repositorio y preparará un entorno de ejecución.
6. Antes de ejecutar, copia el codigo `(gcc tdas/*.c main.c -Wno-unused-result -o main)` en la configuración de compilar
7. Luego, copia el codigo `(gcc -o main main.c tdas/extra.c tdas/list.c tdas/map.c && ./main)` en la configuración de ejecutar 
8. Presiona el botón "Run" para compilar y ejecutar la aplicación.

## Funcionalidades

### Funcionando correctamente:

- Cargar un catálogo de películas de tipo excel o csv.
- Mostrar información completa de cada película del archivo subido.
- Busqueda de id de pélicula.
- Mostrar lista de películas filtradas por género y el total encontradas.
- Mostrar lista de películas filtradas por director y el total encontradas.
- Mostrar lista de películas filtradas por década y el total encontradas.
- Mostrar lista de películas filtradas por género y década y el total encontradas.

### Problemas conocidos:

- La funcionalidad de filtrar por rango de calificaciones sigue en desarrollo.
