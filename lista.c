#include "lista.h"
#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct nodo {
	void* dato;
	struct nodo* prox;
} nodo_t;

struct lista {
	struct nodo* prim;
	struct nodo* ult;
	size_t largo;
};

struct lista_iter {
	lista_t* lista;
	struct nodo* act;
	struct nodo* ant;
};


/* *****************************************************************
 *                    FUNCIONES AUXILIARES
 * *****************************************************************/

nodo_t* nodo_crear(void* valor) {
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (!nodo) return NULL;
	nodo->dato = valor;
	nodo->prox = NULL;
	return nodo;
}

void nodo_destruir(nodo_t* nodo) {
	free(nodo);
}


/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t *lista_crear(void) {
	lista_t* lista = malloc(sizeof(lista_t));
	if (!lista) return NULL;
	lista->prim = NULL;
	lista->ult = NULL;
	lista->largo = 0;
	return lista;
}

bool lista_esta_vacia(const lista_t *lista) {
	return lista->largo == 0;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
	nodo_t* nuevo_prim = nodo_crear(dato);
	if (!nuevo_prim) return false;
	if (lista_esta_vacia(lista)) lista->ult = nuevo_prim;
	else nuevo_prim->prox = lista->prim;
	lista->prim = nuevo_prim;
	lista->largo += 1;
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
	nodo_t* nuevo_ult = nodo_crear(dato);
	if (!nuevo_ult) return false;
	if (lista_esta_vacia(lista)) lista->prim = nuevo_ult;
	else lista->ult->prox = nuevo_ult;
	lista->ult = nuevo_ult;
	lista->largo += 1;
	return true;
}

void *lista_borrar_primero(lista_t *lista) {
	if (lista_esta_vacia(lista)) return NULL;
	void* dato = lista->prim->dato;
	nodo_t* borrado = lista->prim;
	if (lista_largo(lista) == 1) {
		lista->prim = NULL;
		lista->ult = NULL;
	} else {
		lista->prim = lista->prim->prox;
	}
	nodo_destruir(borrado);
	lista->largo -= 1;
	return dato;
}

void *lista_ver_primero(const lista_t *lista) {
	if (lista_esta_vacia(lista)) return NULL;
	return lista->prim->dato;
}

void *lista_ver_ultimo(const lista_t* lista) {
	if (lista_esta_vacia(lista)) return NULL;
	return lista->ult->dato;
}

size_t lista_largo(const lista_t *lista) {
	return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)) {
	while (!lista_esta_vacia(lista)) {
		if (destruir_dato != NULL) {
			destruir_dato(lista->prim->dato);
			lista_borrar_primero(lista);
		} else {
			lista_borrar_primero(lista);
		}
	}
	free(lista);
}


/* ******************************************************************
 *                    PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
	nodo_t* act = lista->prim;
	while (act) {
		if (!visitar(act->dato, extra)) return;
		act = act->prox;
	}
}


/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

lista_iter_t *lista_iter_crear(lista_t *lista) {
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if (!iter) return NULL;
	iter->lista = lista;
	iter->act = lista->prim;
	iter->ant = NULL;
	return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
	if (lista_iter_al_final(iter)) return false;
	iter->ant = iter->act;
	iter->act = iter->act->prox;
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
	return iter->act == NULL ? NULL : iter->act->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
	return iter->act == NULL;
}

void lista_iter_destruir(lista_iter_t *iter) {
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
	nodo_t* nuevo = nodo_crear(dato);
	if (!nuevo) return false;

	nodo_t* viejo = iter->act;
	iter->act = nuevo;
	if (iter->lista->largo == 0) {
		iter->lista->prim = nuevo;
		iter->lista->ult = nuevo;
	} else if (viejo == NULL) {
		iter->ant->prox = nuevo;
		iter->lista->ult = nuevo;
	} else {
		iter->act->prox = viejo;
		if (iter->ant == NULL) iter->lista->prim = nuevo;
		else {
			iter->ant->prox = nuevo;
			if (viejo->prox == NULL) iter->lista->ult = viejo;
		}
	}
	
	iter->lista->largo++;
	return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
	if (iter->act == NULL) return NULL;

	nodo_t* borrado = iter->act;
	void* dato = iter->act->dato;
	if (iter->lista->largo == 1){
		iter->act = NULL; 
		iter->lista->prim = NULL;
		iter->lista->ult = NULL;
	} else {
		if (iter->act->prox == NULL) iter->lista->ult = iter->ant;
		iter->act = iter->act->prox;
		if (iter->ant == NULL) iter->lista->prim = iter->act;
		else iter->ant->prox = iter->act;
	}

	iter->lista->largo--;
	nodo_destruir(borrado);
	return dato;
}
