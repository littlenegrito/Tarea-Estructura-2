#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct { // estructura de pélicula incluyendo cada campo relevante
  char id[100];
  char title[100];
  List *genres;
  List *directors;
  float rating;
  int range;
  int year;
  int decade;
} Film;

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Base de Datos de Películas");
  puts("========================================");

  puts("1) Cargar Películas");
  puts("2) Buscar por id");
  puts("3) Buscar por director");
  puts("4) Buscar por género");
  puts("5) Buscar por década");
  puts("6) Buscar por década y género");
  puts("7) Salir");
}

/**
 * Compara dos claves de tipo string para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo string.
 *
 * @param key1 Primer puntero a la clave string.
 * @param key2 Segundo puntero a la clave string.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

/**
 * Compara dos claves de tipo entero para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo entero.
 *
 * @param key1 Primer puntero a la clave entera.
 * @param key2 Segundo puntero a la clave entera.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}
void borrarComillas(char *str) { 
    int len = strlen(str); //Obtenemos la longitud de la cadena
    if (str[0] == '"') { //SI el primer carácter es una comilla:
        memmove(str, str + 1, len); //Movemos el inicio de la cadena a un espacio a la derecha
        len--; //Bajamos en 1 el tamaño de la cadena

    }
    if (len > 0 && str[len - 1] == '"') { //Si el último carácter es una comilla
        str[len - 1] = '\0'; //Lo cambiamos al carácter nulo para que ahí termine la cadena.

    }

}
char *espacioInicial(char *str) {

    while (isspace((unsigned char)*str)) str++; //Mientras el primer carácter es un espacio, avanzamos el puntero del inicio de la cadena

    return str; //Retornamos la cadena cambiada

}
/**
 * Carga películas desde un archivo CSV y las almacena en un mapa por ID.
 */
void cargar_peliculas(Map *pelis_byid) {
  // Intenta abrir el archivo CSV que contiene datos de películas
  FILE *archivo = fopen("data/Top1500.csv", "r");
  if (archivo == NULL) {
    perror(
        "Error al abrir el archivo"); // Informa si el archivo no puede abrirse
    return;
  }

  char **campos;
  // Leer y parsear una línea del archivo CSV. La función devuelve un array de
  // strings, donde cada elemento representa un campo de la línea CSV procesada.
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    // Crea una nueva estructura Film y almacena los datos de cada película
    Film *peli = (Film *)malloc(sizeof(Film));
    strcpy(peli->id, campos[1]);        // Asigna ID
    strcpy(peli->title, campos[5]);     // Asigna título
   
    
    peli->genres = list_create(); 
    borrarComillas(campos[11]); // Borrar comillas en caso de que haya
    char *token = strtok(campos[11], ","); // Cortar campo generos hasta coma
    while (token != NULL) { 
      char *clean_token = espacioInicial(token); //Limpiaremos el género en caso de que tenga espacios al principio
    list_pushBack(peli->genres, strdup(clean_token)); //Agregar el género a la lista 
    token = strtok(NULL, ","); //Volvemos a cortar la siguiente parte de la cadena hasta la coma
    }

    peli->directors = list_create(); 
    borrarComillas(campos[14]); //Borrr las comillas en caso de que haya
    char *token2 = strtok(campos[14], ","); //Cortar campo directores hasta coma
    while (token2 != NULL) { 
        char *clean_token2 = espacioInicial(token2); //Limpiar director si tiene espacio al principio
        list_pushBack(peli->directors, strdup(clean_token2)); // Agregar director a la lista

        token2 = strtok(NULL, ","); //Siguiente cadena hasta la coma

    }
    
    
    peli->year = atoi(campos[10]); // Asigna año, convirtiendo de cadena a entero
    peli->decade = (peli->year / 10) * 10;
    
    peli->rating = atof(campos[8]);
    peli->range = (int) peli->rating % 10;
    // Inserta la película en el mapa usando el ID como clave
    map_insert(pelis_byid, peli->id, peli);
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas

  // Itera sobre el mapa para mostrar las películas cargadas
  MapPair *pair = map_first(pelis_byid);
  while (pair != NULL) {
    Film *peli = pair->value;
    printf("ID: %s, Título: %s, Año: %d, Rating: %.1f, Decada: %d\n", peli->id, peli->title,
         peli->year, peli->rating, peli->decade);
    
    printf("Géneros: ");
    char *generoAux = list_first(peli->genres);
    while (generoAux != NULL) { // recorrer lista de generos
      printf("%s",generoAux);
      generoAux = list_next(peli->genres); // siguiente genero
      if (generoAux != NULL) { // cuando se termina la cadena
        printf(", "); // separar por coma
      }
    }
    printf("\nDirectores: ");
    char *directorAux = list_first(peli->directors); 
    while (directorAux != NULL) { // recorrer lista de directores
      printf("%s",directorAux);
      directorAux = list_next(peli->directors); // siguiente director
      if (directorAux != NULL) { // cuando se termina la cadena
        printf(", "); // separar por coma
      }
    }
    printf("\n\n");
    pair = map_next(pelis_byid); // Avanza al siguiente par en el mapa
  }
}

/**
 * Busca y muestra la información de una película por su ID en un mapa.
 */
void buscar_por_id(Map *pelis_byid) {
  char id[10]; // Buffer para almacenar el ID de la película

  // Solicita al usuario el ID de la película
  printf("Ingrese el id de la película: ");
  scanf("%s", id); // Lee el ID del teclado

  // Busca el par clave-valor en el mapa usando el ID proporcionado
  MapPair *pair = map_search(pelis_byid, id);

  // Si se encontró el par clave-valor, se extrae y muestra la información de la
  // película
  if (pair != NULL) {
    Film *peli =
        pair->value; // Obtiene el puntero a la estructura de la película
    // Muestra el título y el año de la película
    printf("Título: %s, Año: %d\n", peli->title, peli->year);
  } else {
    // Si no se encuentra la película, informa al usuario
    printf("La película con id %s no existe\n\n", id);
  }
}
/**
 * Muestra lista de peliculas segun variable
 */
void mostrarPeliculas(List *peliculas, char* filtro)
{
  Film *peli = list_first(peliculas);
  while (peli != NULL) { // recorrer lista de peliculas
    printf("Título: %s, Año: %d, Rating: %.1f\n", peli->title, peli->year, peli->rating); // imprimir informacion de la pelicula
    peli = list_next(peliculas); // avanzar lista de peliculas
  }  
  printf("\nSe han encontrado %d películas de %s\n\n", list_size(peliculas), filtro);
}
/**
 * Función que crea lista de peliculas según filtros string en base a un mapa
 */
List* peliculasFiltro(Map *mapa, char *filtro, char* variable) {
  List *listaPeliculas = list_create(); // crear lista para guardar peliculas
  
  MapPair *pair = map_first(mapa);
  while (pair != NULL) { // recorrer mapa
    Film *peli = pair->value; // acceder a la pelicula
    
    if(strcmp(variable, "genero") == 0){ // si es filtro genero
      char *generoAux = list_first(peli->genres); // acceder al primer genero
      while (generoAux != NULL) { // recorrer lista generos
        if (strcmp(generoAux, filtro) == 0) { // si coincide genero
          list_pushBack(listaPeliculas, peli); //insertar pelicula 
        }
        generoAux = list_next(peli->genres);
      }
    }
    if(strcmp(variable, "director") == 0){ // si es filtro director
      char *directorAux = list_first(peli->directors); // acceder al primer genero
        while (directorAux != NULL) { // recorrer lista generos
          if (strcmp(directorAux, filtro) == 0) { // si coincide genero
            list_pushBack(listaPeliculas, peli); //insertar pelicula 
          }
          directorAux = list_next(peli->directors);
        }
    }
    pair = map_next(mapa); // avanzar a siguiente pelicula del mapa
  }
  return listaPeliculas;
}
/**
 * Función que crea lista de peliculas según filtros int en base a un mapa
 */
List* peliculasFiltroInt(Map *mapa, int filtro, char* variable) {
  List *listaPeliculas = list_create(); // crear lista para guardar peliculas

  MapPair *pair = map_first(mapa);
  while (pair != NULL) { // recorrer mapa
    Film *peli = pair->value; // acceder a la pelicula

    if(strcmp(variable, "decada") == 0){ // si es filtro genero
      if (peli->decade == filtro){ // si es filtro director
        list_pushBack(listaPeliculas, peli); //insertar pelicula 
      }
    }
   
    pair = map_next(mapa); // avanzar a siguiente pelicula del mapa
  }
  return listaPeliculas;
}
/**
 * Busca y muestra las peliculas de un género especifico.
 */
void buscar_por_genero(Map *mapa) {
  char genre[10]; // Buffer para almacenar cadena
  
  printf("Ingrese un género: ");
  scanf(" %[^\n]", genre); 
  List *peliculas = peliculasFiltro(mapa, genre, "genero"); // obtener peliculas del genero
  
  if (list_first(peliculas) == NULL) { // comprobar si la lista esta vacia
    printf("\nNo se encontraron películas para ese género\n\n");
    return;
  }
  printf("\nPelículas del género:\n\n");
  mostrarPeliculas(peliculas, genre);
}
/**
 * Busca y muestra las peliculas de un director especifico.
 */
void buscar_por_director(Map *mapa) {
  char director[100]; // Buffer para almacenar cadenas
  char filtro[] = "director";
  printf("Ingrese un director: ");
  scanf(" %[^\n]", director); 
  List *peliculas = peliculasFiltro(mapa, director, filtro); // obtener peliculas del genero
  
  if (list_first(peliculas) == NULL) { // comprobar si la lista esta vacia
    printf("\nNo se encontraron películas de ese director\n\n");
    return;
  }
  printf("\nPelículas del director %s:\n\n", director);
  mostrarPeliculas(peliculas, director);
}
/**
 * Busca y muestra las peliculas de una década especifica.
 */
void buscar_por_decada(Map *mapa) {
  char anio[10]; // Buffer para almacenar cadenas
  char filtro[] = "decada";
  printf("Ingrese un año o década entre 1900-2020: ");
  scanf(" %[^\n]", anio); 
  int decada = (atoi(anio) / 10) *10; // convertir a int y calcular decada
  
  List *peliculas = peliculasFiltroInt(mapa, decada, filtro); // obtener peliculas del genero

  if (list_first(peliculas) == NULL) { // comprobar si la lista esta vacia
    printf("\nNo se encontraron películas de esa década\n\n");
    return;
  }
  printf("\nPelículas de la década %d:\n\n", decada);
  
  Film *peli = list_first(peliculas);
  while (peli != NULL) { // recorrer lista de peliculas
    printf("Título: %s, Año: %d, Rating: %.f\n", peli->title, peli->year, peli->rating); // imprimir informacion de la pelicula
    peli = list_next(peliculas); // avanzar lista de peliculas
  }  
  printf("\nSe han encontrado %d películas de %d\n\n", list_size(peliculas), decada);
}
/**
 * Función que crea lista de peliculas según filtro string en base a otra lista
 */
List* peliculasFiltroLista(List *peliculas, char *filtro, char* variable) {
  List *listaFiltro = list_create();
  
  Film *peli = list_first(peliculas); // acceder a la pelicula
  while (peli != NULL) {
    if(strcmp(variable, "genero") == 0){ // comparar
      char *generoAux = list_first(peli->genres); // recorrer lista generos
      while (generoAux != NULL) {
        if (strcmp(generoAux, filtro) == 0) { // si coincide genero
          list_pushBack(listaFiltro, peli); // guardar pelicula en lista
          break; 
        }
        generoAux = list_next(peli->genres); // siguiente genero
      }
    }
    peli = list_next(peliculas); // siguiente pelicula de lista
  }
  return listaFiltro;
}

void buscar_por_decadaGenero(Map *mapa) {
  char anio[10]; // Buffer para almacenar cadenas
  char genero[100]; 
  printf("Ingrese una década (1900 - 2020): ");
  scanf(" %[^\n]", anio); 
  int decada = (atoi(anio) / 10) *10; // convertir año en decada
  
  printf("Ingrese un género: ");
  scanf(" %[^\n]", genero); 

  List *peliculasDecada = peliculasFiltroInt(mapa, decada, "decada"); // obtener peliculas de decada especifica
  List *peliculasDecadaGenero = peliculasFiltroLista(peliculasDecada, genero, "genero"); // obtener peliculas de genero desde una lista

  if (list_first(peliculasDecadaGenero) == NULL) {
    printf("\nNo se encontraron películas para esa década y género\n\n");
    return;
  }
  printf("\nPelículas de la década %d y género %s:\n\n", decada, genero);
  mostrarPeliculas(peliculasDecadaGenero, genero);
}

int main() {
  char opcion; // Variable para almacenar una opción ingresada por el usuario
               // (sin uso en este fragmento)

  // Crea un mapa para almacenar películas, utilizando una función de
  // comparación que trabaja con claves de tipo string.
  Map *pelis_byid = map_create(is_equal_str);

  // Recuerda usar un mapa por criterio de búsqueda

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_peliculas(pelis_byid);
      break;
    case '2':
      buscar_por_id(pelis_byid);
      break;
    case '3':
      buscar_por_director(pelis_byid);
      break;
    case '4':
      buscar_por_genero(pelis_byid);
      break;
    case '5':
      buscar_por_decada(pelis_byid);
      break;
    case '6':
      buscar_por_decadaGenero(pelis_byid);
      break;
    default:
    }
    presioneTeclaParaContinuar();

  } while (opcion != '7');

  return 0;
}
