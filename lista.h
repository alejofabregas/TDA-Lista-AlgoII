#ifndef LISTA_H
#define LISTA_H

#include <stdlib.h>
#include <stdbool.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct lista;
typedef struct lista lista_t;

typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// POST: Devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// PRE: La lista fue creada.
// POST: Devuelve verdadero si la lista no tiene elementos enlazados, false en caso contrario.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento a la lista en el principio. Devuelve falso en caso de error.
// PRE: La lista fue creada.
// POST: Se agregó un nuevo elemento a la lista, dato se encuentra al inicio
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento a la lista en el final. Devuelve falso en caso de error.
// PRE: La lista fue creada.
// POST: Se agregó un nuevo elemento a la lista, dato se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero, y se devuelve su valor, si está vacía, devuelve NULL.
// PRE: La lista fue creada.
// POST: Se devolvió el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// PRE: La lista fue creada.
// POST: Se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del ultimo, si está vacía devuelve NULL.
// PRE: La lista fue creada.
// POST: Se devolvió el ultimo elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

// PRE: La lista fue creada.
// POST: Devuelve el largo de la lista.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// PRE: La lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// POST: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));


/* ******************************************************************
 *                    PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/
 
//PRE: Recibe la lista a iterar, una funcion visitar para aplicar a cada elemento de la lista,
// y un elemento extra para compartir informacion entre interaciones.
//POST: Itera la lista elemento por elemento aplicando la funcion visitar a cada uno de ellos, 
// utilizando el elemento extra si se paso como parametro a visitar. Si en alguna iteracion
// la funcion visitar devuelve false, se interrumpe la iteracion. En caso contrario, se itera
// hasta el ultimo elemento de la lista.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);


/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

// Crea un iterador de lista.
// PRE: Recibe una lista previamente creada. No modificar la lista mientras 
// se usa el iterador.
// POST: Devuelve un iterador en la primer posicion de la lista o NULL si no se pudo crear.
lista_iter_t *lista_iter_crear(lista_t *lista);

// PRE: Recibe un iterador creado.
// POST: Devuelve true si avanza, false sino, el iterador ahora esta en la posicion siguiente.
bool lista_iter_avanzar(lista_iter_t *iter);

// PRE: Recibe un iterador creado.
// POST: Devuelve el dato en la posicion actual de la lista.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// PRE: Recibe un iterador creado.
// POST: Devuelve true si esta en una posicion invalida.
bool lista_iter_al_final(const lista_iter_t *iter);

// PRE: Recibe un interador creado.
// POST: Destruye el iterador.
void lista_iter_destruir(lista_iter_t *iter);

// PRE: Recibe un interador creado.
// POST: Inserta un elemento en la posicion actual del iterador. Devuelve true si se pudo
// sino false.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// PRE: Recibe un interador creado.
// POST: Borra el elemento de la posicion actual del iterador, y devuelve el dato borrado.
void *lista_iter_borrar(lista_iter_t *iter);


/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

void pruebas_lista_estudiante(void);


#endif
