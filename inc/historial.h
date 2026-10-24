#ifndef HISTORIAL_H
#define HISTORIAL_H

#include "comunes.h"

/** 
 * @file historial.h
 * @brief manejo y gestion del historial como pila 
 *
 * Este archivo contiene las definiciones de la estructura y el tipo historial 
 * junto a los prototipos de las funciones respectivas.   
 *
 * @author Francisco Alvarado 
 */

/**
 * @brief estructura historial basada en pila
 */ 

struct s_DatosHistorial 
{
	// TODO: agregar referencia al inventario
	int acc		   /**< identificador de la accion anterior para undo*/
	char *descripcion; /**< descripcion de la accion */
	tipo_accion tipo;  /**< identificador del tipo de accion */
};
typedef s_DatosHistorial ContHistorial;

struct s_NodoHistorial
{
	ContHistorial datos;
	struct s_NodoHistorial *siguiente; /* hacia el mas antiguo */
};
struct s_historial
{
	struct s_NodoHistorial *nodo;
	int cantidad_registros;
};

typedef struct s_historial *historial; 
typedef struct s_NodoHistorial *PosHistorial;

/**
 * @brief Inicializa la pila del historial.
 * @return 0 si todo va bien, -1 en caso de error.
 */
historial historial_make_empty(historial);

/** 
 * @brief registra ultima accion
 * 
 * @param h	puntero a historial
 * @param acc	identificador a la accion anterior
 * @param desc	se pasa pasa por copia la descripcion de la accion.
 * @param t	tipo de accion.
 * @return 0 si todo va bien, -1 memoria llena y cualquier negativo para otros errores
 */
int historial_push(historial h, int acc, const char **desc, tipo_accion t );

/**
 * @brief Deshace (saca) la accion mas reciente de la pila.
 *
 * Libera la descripcion y el nodo. El llamador debe aplicar la reversion
 * con los datos del nodo ANTES de llamar a esta funcion (o pasarlos por
 * puntero de salida, ver variante).
 *
 * @param h  puntero a historial.
 * @return 0 si se deshizo, -1 si la pila esta vacia o h es NULL.
 */
int historial_pop(historial h);

/**
 * @brief Consulta la accion mas reciente sin sacarla.
 * @param h  puntero a historial.
 * @return puntero a los datos de la cima, o NULL si esta vacia.
 */
PosHistorial historial_top(const historial h);

/**
 * @brief Consulta la accion mas reciente sin sacarla.
 * @param h  puntero a historial.
 * @return puntero a los datos de la cima, o NULL si esta vacia.
 */
historial historial_siguiente(const historial h);


/**
 * @brief Indica si la pila esta vacia.
 * @return 1 si vacia, 0 si no, -1 si detecta datos invalidos.
 */
int historial_is_empty(const historial h);

/**
 * @brief Libera todos los nodos y la descripcion de cada uno.
 * No libera el struct s_historial en si (lo hace el llamador).
 * @param h  puntero a historial.
 */
void historial_destroy(historial h);

#endif
