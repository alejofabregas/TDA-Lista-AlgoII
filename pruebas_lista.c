#define _POSIX_C_SOURCE 200809L
#include "lista.h"
#include "pila.h" 
#include "testing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SUMA_ELEM 0
#define CANT_ITERACIONES 1
#define PALABRA 0
#define PALABRA_ARMADA 1

/* *****************************************************************
 *                    FUNCIONES AUXILIARES
 * *****************************************************************/

// Para prueba destruir:

void pila_destruir_wrapper(void* pila){
	pila_destruir(pila);
}

// Para el iterador interno:

bool sumar_elementos(void* dato, void* extra) {
	*(int*)extra += *(int*)dato;
	return true;
}

bool suma_primeros_3(void* dato, void* extra) {
	int* suma_y_cant_iter = (int*)extra;
	suma_y_cant_iter[SUMA_ELEM] += *(int*)dato;
	suma_y_cant_iter[CANT_ITERACIONES] += 1;
	return suma_y_cant_iter[1] < 3 ? true : false;
}

bool encontrar_palabra(void* dato, void* extra) {
	char* caracter_iterado = (char*)dato;
	char** palabras = (char**)extra;
	strcat(palabras[PALABRA_ARMADA], caracter_iterado);
	if (strcmp(palabras[PALABRA_ARMADA], palabras[PALABRA]) == 0) return false;
	return true;
}


/* ******************************************************************
 *                    PRUEBAS DE LA LISTA
 * *****************************************************************/

static void prueba_lista_vacia(void) {
    lista_t *lista = lista_crear();
    printf("--INICIO DE PRUEBAS CON LISTA VACIA--\n");

    /* Inicio de pruebas */
    print_test("Crear lista", lista != NULL);
    print_test("Lista recien creada esta vacia", lista_esta_vacia(lista));
    print_test("El largo de la lista devuelve 0", lista_largo(lista) == 0);
    print_test("Ver el primer elemento de la lista vacia devuelve NULL", lista_ver_primero(lista) == NULL);
    print_test("Ver el ultimo elemento de la lista vacia devuelve NULL", lista_ver_ultimo(lista) == NULL);
    print_test("Borrar elemento lista vacia devuelve NULL", lista_borrar_primero(lista) == NULL);

    /* Destruyo la lista*/
    lista_destruir(lista,NULL);
}

static void prueba_insertar_borrar_2num(void) {
	printf("\n--PRUEBAS DE INSERTAR Y BORRAR DOS NUMEROS--\n");
	lista_t* lista = lista_crear();
	int uno = 1;
	int dos = 2;
	
	print_test("Inserto el 1 al principio", lista_insertar_primero(lista, &uno));
	print_test("El primero es 1", lista_ver_primero(lista) == &uno);
	print_test("El ultimo es 1", lista_ver_ultimo(lista) == &uno);
	print_test("Me fijo si esta vacia", !lista_esta_vacia(lista));
	print_test("Inserto el 2 al final", lista_insertar_ultimo(lista, &dos));
	print_test("El primero es 1 todavia", lista_ver_primero(lista) == &uno);
	print_test("El ultimo es 2", lista_ver_ultimo(lista) == &dos);
	print_test("Me fijo si esta vacia", !lista_esta_vacia(lista));
	print_test("Borro el 1", lista_borrar_primero(lista) == &uno);
	print_test("El primero es 2", lista_ver_primero(lista) == &dos);
	print_test("Borro el 2", lista_borrar_primero(lista) == &dos);
	print_test("Me fijo si esta vacia", lista_esta_vacia(lista)); // Caso borde: borre el elemento que era el primero y el ultimo a la vez
	print_test("Esta vacia, pero borro el primero igual", lista_borrar_primero(lista) == NULL);
	print_test("Me fijo si esta vacia", lista_esta_vacia(lista));
	print_test("Inserto el 1 de nuevo", lista_insertar_ultimo(lista, &uno));
	print_test("Ahora no esta vacia", lista_esta_vacia(lista) == false);
	print_test("Borro el 1", lista_borrar_primero(lista) == &uno);
	print_test("Ahora esta vacia", lista_esta_vacia(lista) == true);
	lista_destruir(lista, NULL);
}

static void prueba_insertar_y_borrar(void) {
	lista_t *lista = lista_crear();
	int valor1 = 3, valor3 = 5;
	char valor2 = 'h';
	printf("\n--PRUEBAS INSERTAR Y BORRAR ELEMENTOS VARIOS--\n");

	/* Borrar y ver primero de lista recien creada invalido, devuelve NULL*/
	print_test("Eliminar elemento de lista recien creada devuelve NULL",lista_borrar_primero(lista) == NULL);
	print_test("Ver primero y ultimo elemento  de lista recien creada devuelve NULL", lista_ver_primero(lista) == NULL && lista_ver_ultimo(lista) == NULL);

	/* Agrego dos elementos y chequeo que los valores en los topes sean correctos*/
	print_test("Agregar elemento valor1 a la lista", lista_insertar_primero(lista, &valor1));
	print_test("Ver si el primero y ultimo es valor1", lista_ver_primero(lista) == &valor1 && lista_ver_ultimo(lista) == &valor1);
	print_test("Agregar el elemento valor2 al final de la lista", lista_insertar_ultimo(lista, &valor2));
	print_test("Ver si el primero sigue siendo valor1", lista_ver_primero(lista) == &valor1);
	print_test("Ver si el ultimo es valor2", lista_ver_ultimo(lista) == &valor2);
	print_test("Agregar el elemento valor2 al inicio de la lista", lista_insertar_primero(lista, &valor3));
	print_test("Ver si ahora el primero es valor3", lista_ver_primero(lista) == &valor3);

	/* ELimino elementos y chequeo si al quedar vacio devuelve NULL tanto borrar elemento como ver topes*/
	print_test("Verificar que el largo es 3", lista_largo(lista) == 3);
	print_test("Ver si se elimina el primer elemento valor3", lista_borrar_primero(lista) == &valor3);
	print_test("Ver si se elimina el siguiente primero valor1", lista_borrar_primero(lista) == &valor1);
	print_test("Ver si se elimina el ultimo valor2", lista_borrar_primero(lista) == &valor2);
	print_test("Ver si el primero y ultimo ahora no existen", lista_ver_primero(lista) == NULL && lista_ver_ultimo(lista) == NULL);
  
	/* Verificar si vacia se comporta como recien creada, y chequear si se puede agregar NULL*/
	print_test("Eliminar elemento de lista cuyos elementos fueron borrados", lista_borrar_primero(lista) == NULL);
	print_test("Verificar que el largo es 0", lista_largo(lista) == 0);
	print_test("Ver si el primero y ultimo son NULL", lista_ver_primero(lista) == NULL && lista_ver_ultimo(lista) == NULL);
	print_test("Agregar elemento NULL a la lista", lista_insertar_primero(lista, NULL));
	print_test("Agregar elemento valor1 a la lista", lista_insertar_ultimo(lista, &valor1));
	print_test("Ver si el primero es NULL y el ultimo valor1", lista_ver_primero(lista) == NULL && lista_ver_ultimo(lista) == &valor1);

	/* Destruyo la lista*/
	lista_destruir(lista,NULL);
}

static void prueba_volumen(void) {
	printf("\n--INICIO DE PRUEBAS DE VOLUMEN--\n");

	lista_t *lista = lista_crear();
	size_t elementos = 1000;
  
    /*Prueba de correcta creacion*/
    print_test("Crear lista", lista != NULL);

    /*Pruebo insertar en todas las posiciones con insertar primero*/
    size_t* valores = malloc(2*elementos*sizeof(size_t));
    bool ok = true;
    for (size_t i = 0; i < elementos; i++) {
        // Si algun elemento no se pudo insertar correctamente, ok sera false
        ok &= lista_insertar_primero(lista, &valores[i]);
    }
    print_test("Se pudieron insertar todos los 1000 elementos con insertar primero", ok);
    /*Pruebo que lo guardado sea correcto*/
    print_test("El largo de la lista es 1000", lista_largo(lista) == elementos);

      /*Pruebo insertar en todas las posiciones con insertar ultimo*/
  	ok = true;
    for (size_t i = 0; i < elementos; i++) {
        // Si algun elemento no se pudo insertar correctamente, ok sera false
        ok &= lista_insertar_ultimo(lista, &valores[i]);
    }
    print_test("Se pudieron insertar todos los 1000 elementos con insertar ultimo", ok);
    print_test("El largo de la lista es 2000", lista_largo(lista) == 2*elementos);

    /*Pruebo borrar todas las posiciones*/
    ok = true;
    elementos = 2*elementos;
    for (size_t i = 0; i < elementos; i++) {
        lista_borrar_primero(lista);
    }
    print_test("Se borraron los 2000 elementos, el largo de la lista es 0", lista_largo(lista) == 0);
    
    /*Destruyo la lista*/
    free(valores);
    lista_destruir(lista,NULL);
}

static void prueba_destruccion(void) {
	lista_t *lista = lista_crear();
	pila_t *pila = pila_crear();

	printf("\n--INICIO DE PRUEBAS DE DESTRUCCION CON FUNCION DESTRUIR--\n");

    // Inicio de pruebas 
    print_test("Crear lista", lista != NULL);
    print_test("Crear pila", pila != NULL);

    // Apilo elementos en la pila
    size_t elementos = 5;
    size_t* valores = malloc(elementos*sizeof(size_t));
    bool ok = true;
    for (size_t i = 0; i < elementos; i++) {
        // Si algun elemento no se pudo apilar correctamente, ok sera false
        ok &= pila_apilar(pila, &valores[i]);
    }
    print_test("Se pudieron apilar los 5 elementos", ok);

    ok = true;
    ok &= lista_insertar_primero(lista,pila);
    print_test("Se pudieron apilar los elementos de la pila", ok);
    
    // Destruyo la lista usando funcion de destruir pila
    free(valores);
    lista_destruir(lista,pila_destruir_wrapper);
}


/* ******************************************************************
 *                    PRUEBAS DEL ITERADOR INTERNO
 * *****************************************************************/

static void prueba_iter_interno_suma_2elem() {
	printf("\n--PRUEBAS DE ITERADOR INTERNO - SUMA DE 2 ELEMENTOS--\n");
	lista_t* lista = lista_crear();
	int uno = 1;
	int dos = 2;
	int suma = 0;
	lista_insertar_primero(lista, &uno);
	lista_insertar_primero(lista, &dos);
	lista_iterar(lista, sumar_elementos, &suma);
	print_test("La suma de los elementos es 3", suma == 3);
	lista_destruir(lista, NULL);
}

static void prueba_iter_interno_suma_10elem() {
	printf("\n--PRUEBAS DE ITERADOR INTERNO - SUMA DE 10 ELEMENTOS--\n");
	lista_t* lista = lista_crear();
	int suma = 0;
	int resultado_esperado = 45;
	int datos[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	for (int i = 0; i < 10; i++) {
		lista_insertar_primero(lista, (void*)&datos[i]);
	}
	lista_iterar(lista, sumar_elementos, &suma);
	print_test("El resultado de la suma de los elementos con el iterador interno es correcto", suma == resultado_esperado);
	lista_destruir(lista, NULL);
}

static void prueba_iter_interno_lista_vacia() {
	printf("\n--PRUEBAS DE ITERADOR INTERNO - ITERAR LISTA VACIA--\n");
	lista_t* lista = lista_crear();
	int suma = 0;
	lista_iterar(lista, sumar_elementos, &suma);
	print_test("La lista vacia iterada esta vacia", lista_esta_vacia(lista));
	print_test("El resultado de la suma es cero", suma == 0);
	lista_destruir(lista, NULL);
}

static void prueba_iter_interno_suma_primeros_3() {
	printf("\n--PRUEBA DE ITERADOR INTERNO - SUMA DE PRIMEROS 3 ELEMENTOS--\n");
	lista_t* lista = lista_crear();
	int* suma_y_cant_iter = malloc(2 * sizeof(int));
	suma_y_cant_iter[SUMA_ELEM] = 0;
	suma_y_cant_iter[CANT_ITERACIONES] = 0;
	int datos[] = {1, 2, 3, 4, 5};
	for (int i = 0; i < 5; i++) {
		lista_insertar_ultimo(lista, (void*)&datos[i]);
	}
	lista_iterar(lista, suma_primeros_3, suma_y_cant_iter);
	print_test("La suma de los primeros 3 elementos es correcta", suma_y_cant_iter[0] == 6);
	lista_destruir(lista, NULL);
	free(suma_y_cant_iter);
}

static void prueba_iter_interno_iterar_hasta_palabra() {
	printf("\n--PRUEBA DE ITERADOR INTERNO - ITERAR HASTA ARMAR PALABRA--\n");
	lista_t* lista = lista_crear();
	char texto[] = "casaxyz";
	char palabra[] = "casa";
	char* palabra_armada = malloc(sizeof(texto));
	*(palabra_armada) = '\0';
	char** palabras = malloc(2 * sizeof(palabra_armada));
	palabras[PALABRA] = palabra;
	palabras[PALABRA_ARMADA] = palabra_armada;
	for (size_t i = 0; i < strlen(texto); i++) {
		lista_insertar_ultimo(lista, (void*)strndup(&texto[i], 1));
	}
	lista_iterar(lista, encontrar_palabra, palabras);
	print_test("La palabra armada en la iteración es igual a la palabra a comparar", strcmp(palabras[PALABRA_ARMADA], palabras[PALABRA]) == 0);
	lista_destruir(lista, free);
	free(palabra_armada);
	free(palabras);
}


/* ******************************************************************
 *                    PRUEBAS DEL ITERADOR EXTERNO
 * *****************************************************************/

static void prueba_iter_externo_lista_vacia(void) {
	printf("\n--INICIO DE PRUEBAS DE ITERADOR EXTERNO - ITERAR LISTA VACIA--\n");

	lista_t *lista = lista_crear();
	//Prueba de correcta creacion
	print_test("Crear lista", lista != NULL);
	print_test("Lista esta vacia", lista_esta_vacia(lista));
    
	lista_iter_t *iter = lista_iter_crear(lista);
	//Prueba de correcta creacion
	print_test("Crear iterador", iter != NULL);
	print_test("Ver actual es el primero de la lista, NULL", lista_ver_primero(lista) == lista_iter_ver_actual(iter));
	print_test("Lista iter al final es true", lista_iter_al_final(iter));

	/* Destruyo la lista y el iterador*/
	lista_destruir(lista,NULL);
	lista_iter_destruir(iter);
}

static void prueba_iter_externo(void) {
	printf("\n--INICIO DE PRUEBAS DE ITERADOR EXTERNO - ITERAR HASTA FINAL--\n");
	lista_t *lista = lista_crear();
	/*Prueba de correcta creacion*/
	print_test("Crear lista", lista != NULL);
	size_t elementos = 50;
	size_t* valores = malloc(elementos * sizeof(size_t));
    bool ok = true;
    for (size_t i = 0; i < elementos; i++) {
        // Si algun elemento no se pudo insertar correctamente, ok sera false
        ok &= lista_insertar_ultimo(lista, &valores[i]);
    }
    print_test("Se pudieron insertar todos los elementos",ok);

    lista_iter_t *iter = lista_iter_crear(lista);
	/*Prueba de correcta creacion*/
	print_test("Crear iterador", iter != NULL);
	/*Prueba inicial*/
	print_test("Ver actual es el primero", lista_iter_ver_actual(iter) == lista_ver_primero(lista));

	ok = true;
	while(!lista_iter_al_final(iter)){
		for (size_t i = 0; i < elementos; i++) {
		 	ok &= lista_iter_ver_actual(iter) == lista_ver_primero(lista);
		 	lista_iter_borrar(iter);
		}
	}
	print_test("Se itera correctamente hasta el final de la lista", ok);

	/* Destruyo la lista y el iterador*/
	free(valores);
	lista_destruir(lista,NULL);
	lista_iter_destruir(iter);
}

static void prueba_iter_externo_insertar(void) {
 	printf("\n--INICIO DE PRUEBAS DE ITERADOR EXTERNO INSERTAR - EN PRINCIPIO Y FINAL--\n");

	lista_t *lista = lista_crear();
	/*Prueba de correcta creacion*/
	print_test("Crear lista", lista != NULL);

	int valor1 = 3, valor2 = 4, valor3 = 5, valor4 = 8, valor5 = 6, valor6 = 10;

	print_test("Agregar elemento valor1 a la lista", lista_insertar_primero(lista, &valor1));
	print_test("Agregar el elemento valor2 ultimo en la lista", lista_insertar_ultimo(lista, &valor2));
	print_test("Agregar el elemento valor3 ultimo en la lista", lista_insertar_ultimo(lista, &valor3));
	print_test("El largo de la lista es 3", lista_largo(lista) == 3);
  	
	lista_iter_t *iter = lista_iter_crear(lista);
	/*Prueba de correcta creacion*/
	print_test("Crear iterador", iter != NULL);

	/*Prueba inicial*/
	print_test("Ver actual es el primero", lista_iter_ver_actual(iter) == lista_ver_primero(lista));

    /*Prueba insertar elemento al principio de la lista*/
	print_test("Insertar un elemento al principio", lista_iter_insertar(iter, &valor4));
	print_test("Iterador ahora esta en el nuevo", lista_iter_ver_actual(iter) == &(valor4));
	print_test("Ahora es el primero de la lista", lista_ver_primero(lista) == &(valor4));
	print_test("El largo de la lista ahora es 4", lista_largo(lista) == 4);

	print_test("Avanzar en la lista devuelve true", lista_iter_avanzar(iter));
	print_test("Ver actual ahora es el segundo (antes de insertar era primero)", lista_iter_ver_actual(iter) == &(valor1));
	print_test("Avanzar en la lista devuelve true", lista_iter_avanzar(iter));
	lista_iter_avanzar(iter);
	print_test("Ver actual es el ultimo", lista_iter_ver_actual(iter) == lista_ver_ultimo(lista));

	/*Prueba insertar elemento con iterador en ultimo de la lista*/
	print_test("Insertar un elemento al final con iterador", lista_iter_insertar(iter, &valor5));
	print_test("Iterador ahora esta en el nuevo", lista_iter_ver_actual(iter) == &(valor5));
	print_test("El ultimo de la lista sigue siendo el mismo", lista_ver_ultimo(lista) == &(valor3));

	/*Prueba insertar elemento con iterador al final de la lista*/
	print_test("Avanzar en la lista devuelve true", lista_iter_avanzar(iter));
	lista_iter_avanzar(iter);
	print_test("Esta en el final de la lista", lista_iter_al_final(iter));
	print_test("Insertar un elemento al final con iterador", lista_iter_insertar(iter, &valor5));
	print_test("Ahora es el ultimo de la lista", lista_ver_ultimo(lista) == &(valor5));
	
	/*Prueba insertar elemento al final de la lista con la primitiva lista_insertar_ultimo*/
	print_test("Insertar un elemento al final con la primitiva lista_insertar_ultimo", lista_insertar_ultimo(lista, &valor6));
	print_test("Ahora es el ultimo de la lista", lista_ver_ultimo(lista) == &valor6);
	print_test("El largo de la lista ahora es 7", lista_largo(lista) == 7);

	/* Destruyo la lista y el iterador*/
	lista_destruir(lista,NULL);
	lista_iter_destruir(iter);
}

static void prueba_iter_externo_borrar(void) {
	printf("\n--INICIO DE PRUEBAS DE ITERADOR EXTERNO - BORRAR EN PRINCIPIO Y FINAL--\n");

	lista_t *lista = lista_crear();
	//Prueba de correcta creacion
	print_test("Crear lista", lista != NULL);

	int valor1 = 3, valor2 = 4, valor3 = 5, valor4 = 8, valor5 = 6;

	print_test("Agregar elemento valor1 a la lista", lista_insertar_primero(lista, &valor1));
	print_test("Agregar el elemento valor2 ultimo en la lista", lista_insertar_ultimo(lista, &valor2));
	print_test("Agregar el elemento valor3 ultimo en la lista", lista_insertar_ultimo(lista, &valor3));
	print_test("Agregar el elemento valor4 ultimo en la lista", lista_insertar_ultimo(lista, &valor4));
	print_test("Agregar el elemento valor3 ultimo en la lista", lista_insertar_ultimo(lista, &valor5));
	print_test("El largo de la lista es 5", lista_largo(lista) == 5);
    
	lista_iter_t *iter1 = lista_iter_crear(lista);
	//Prueba de correcta creacion
	print_test("Crear iterador", iter1 != NULL);

	//Prueba inicial
	print_test("Ver actual es el primero", lista_iter_ver_actual(iter1) == lista_ver_primero(lista));

	//Prueba borrar elemento al principio de la lista
	print_test("Borrar elemento del principio", lista_iter_borrar(iter1) == &(valor1));
	print_test("Iterador ahora esta en el nuevo primero", lista_iter_ver_actual(iter1) == &(valor2));
	print_test("Ahora es el primero de la lista", lista_ver_primero(lista) == &(valor2));
	print_test("El largo de la lista ahora es 4", lista_largo(lista) == 4);

	print_test("Avanzar en la lista devuelve true", lista_iter_avanzar(iter1));
	print_test("Ver actual ahora es segundo (antes de borrar era tercero)", lista_iter_ver_actual(iter1) == &(valor3));

	//Prueba borrar elemento al final de la lista
	lista_iter_avanzar(iter1);
	lista_iter_avanzar(iter1);
	print_test("Borrar elemento del final", lista_iter_borrar(iter1) == &(valor5));
	print_test("El ultimo ahora es el valor4", lista_ver_ultimo(lista) == &(valor4));
	print_test("El largo de la lista ahora es 3", lista_largo(lista) == 3);
	
	lista_iter_destruir(iter1);
	
	lista_iter_t *iter = lista_iter_crear(lista);
	//Prueba borrar todos los elementos de la lista
	print_test("Ver actual es el primero", lista_iter_ver_actual(iter) == lista_ver_primero(lista));
	print_test("Borrar elemento del principio", lista_iter_borrar(iter) == &(valor2));
	print_test("Ahora el primero de la lista es valor3", lista_ver_primero(lista) == &(valor3));
	print_test("Borrar elemento del principio", lista_iter_borrar(iter) == &(valor3));
	print_test("Ahora el primero de la lista es valor4", lista_ver_primero(lista) == &(valor4));
	print_test("Borrar elemento del principio", lista_iter_borrar(iter) == &(valor4));
	print_test("Ahora el primero de la lista es NULL", lista_ver_primero(lista) == NULL);
	print_test("Iterador en el final de lista", lista_iter_al_final(iter));
	print_test("El largo de la lista ahora es 0", lista_largo(lista) == 0);
	print_test("Borrar en lista vacia da NULL", lista_iter_borrar(iter) == NULL);
	
	// Destruyo la lista y el iterador
	lista_destruir(lista,NULL);
	lista_iter_destruir(iter);
}

static void prueba_iter_externo_insertar_borrar_medio(void) {
	printf("\n--INICIO DE PRUEBAS DE ITERADOR EXTERNO - INSERTAR Y BORRAR EN MEDIO--\n");

	lista_t *lista = lista_crear();
	//Prueba de correcta creacion
	print_test("Crear lista", lista != NULL);

	int valor1 = 3, valor2 = 4, valor3 = 5, valor4 = 8, valor5 = 6;

	print_test("Agregar elemento valor1 a la lista", lista_insertar_primero(lista, &valor1));
	print_test("Agregar el elemento valor2 ultimo en la lista", lista_insertar_ultimo(lista, &valor2));
	print_test("Agregar el elemento valor3 ultimo en la lista", lista_insertar_ultimo(lista, &valor3));
	print_test("Agregar el elemento valor3 ultimo en la lista", lista_insertar_ultimo(lista, &valor4));

	lista_iter_t *iter = lista_iter_crear(lista);
	//Prueba de correcta creacion
	print_test("Crear iterador", iter != NULL);

	//Prueba inicial
	print_test("Ver actual es el primero", lista_iter_ver_actual(iter) == lista_ver_primero(lista));
	print_test("Avanzar en la lista devuelve true", lista_iter_avanzar(iter));
	lista_iter_avanzar(iter);

	void* iterado = lista_iter_ver_actual(iter);
	/*Prueba insertar elemento en el medio de la lista*/
	print_test("insertar un elemento en el medio", lista_iter_insertar(iter, &valor5));
	print_test("ver actual ahora es el nuevo agregado valor5", lista_iter_ver_actual(iter) == &(valor5));
	lista_iter_avanzar(iter);
	print_test("ver actual ahora es el que corrimos", lista_iter_ver_actual(iter) == iterado);
	print_test("el largo de la lista ahora es 5", lista_largo(lista) == 5);
	
	// Destruyo el iterador y creo uno nuevo
	lista_iter_destruir(iter);
	lista_iter_t *iterador = lista_iter_crear(lista);

	//Prueba de correcta creacion
	print_test("Crear otro iterador", iterador != NULL);
	print_test("Ver actual es el primero", lista_iter_ver_actual(iterador) == lista_ver_primero(lista));
	lista_iter_avanzar(iterador);
	lista_iter_avanzar(iterador);

	/*Prueba borrar elemento en el medio de la lista*/
	print_test("borrar un elemento en el medio", lista_iter_borrar(iterador));
	print_test("ver actual ahora es siguiente", lista_iter_ver_actual(iterador) == &(valor3));
	print_test("el largo de la lista ahora es 4", lista_largo(lista) == 4);

	// Destruyo la lista y el iterador
	lista_destruir(lista,NULL);
	lista_iter_destruir(iterador);
}



void pruebas_lista_estudiante() {
	prueba_lista_vacia();
	prueba_insertar_borrar_2num();
	prueba_insertar_y_borrar();
	prueba_volumen();
	prueba_destruccion();
	prueba_iter_interno_suma_2elem();
	prueba_iter_interno_suma_10elem();
	prueba_iter_interno_lista_vacia();
	prueba_iter_interno_suma_primeros_3();
	prueba_iter_interno_iterar_hasta_palabra();
	prueba_iter_externo_lista_vacia();
	prueba_iter_externo();
	prueba_iter_externo_insertar();
	prueba_iter_externo_borrar();
	prueba_iter_externo_insertar_borrar_medio();
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_lista_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
