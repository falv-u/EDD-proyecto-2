#ifndef HISTORIAL_H
#define HISTORIAL_H

#include "comunes.h"
#include "carta.h"
#define HISTORIAL_MAX 40

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

enum e_TipoAccion {
	A_CONSUMIBLE,
	A_REGISTRO,
	A_COMBATE_I,
	A_COM_TURNO,
	A_FIN_COMBATE,
	A_DESCARTE, /* Unica reversible */
};
typedef enum e_TipoAccion TipoAccion;

struct s_DatosHistorial 
{
	// TODO: agregar referencia al inventario
	char *descripcion; /**< descripcion de la accion */
	TipoAccion tipo;  /**< identificador del tipo de accion */
};
typedef struct s_DatosHistorial ContHistorial;

struct s_NodoHistorial
{
	ContHistorial datos;
	struct s_NodoHistorial *siguiente; /* hacia el mas antiguo */
};

struct s_Reversion {
	s_Carta carta;          /* copia de la carta descartada */
	int     posicion;       /* indice donde estaba en el inventario */
} ;

typedef struct s_Reversion s_Reversion;

struct s_historial
{
	struct s_NodoHistorial *nodo;
	int CantidadRegistros;
	bool HayReversionPendiente;
	s_Reversion reversion;
};

typedef struct s_historial *historial; 
typedef struct s_NodoHistorial *PosHistorial;

/**
 * @brief Inicializa la pila del historial.
 * @return 0 si todo va bien, -1 en caso de error.
 */
historial historial_make_empty(void);

/** 
 * @brief registra ultima accion
 * 
 * @param h	puntero a historial
 * @param desc	se pasa pasa por copia la descripcion de la accion.
 * @param t	tipo de accion.
 * @return 0 si todo va bien, -1 memoria llena y cualquier negativo para otros errores
 */
int historial_push(historial h, const char *desc, TipoAccion t);

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
PosHistorial historial_siguiente(const PosHistorial pos);


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

/**
 * @brief Libera completamente el historial: nodos, descripciones y el struct.
 * @param h  puntero a historial (se pone a NULL tras liberar).
 */
void historial_free(historial *h);

/**
 * @brief Registra una accion en el historial (push) y, si la acción es
 *        reversible (A_DESCARTE), guarda la 'foto' para poder deshacerla.
 *        La foto de la acción anterior, si existía, se descarta (ventana de 1).
 *
 * @param h          historial
 * @param desc       descripción (se copia)
 * @param t          tipo de acción
 * @param rev        datos de reversión; se usa solo si t == A_DESCARTE,
 *                   puede ser NULL en otro caso
 * @return 0 ok, -1 error (NULL, pila llena, sin memoria)
 */
int historial_registrar(historial h, const char *desc, TipoAccion t, const s_Reversion *rev);

/**
 * @brief Devuelve true si la ultima acción registrada es reversible
 *        y aun no se ha deshecho ni invalidado.
 */
bool historial_puede_deshacer(const historial h);

/**
 * @brief Consume la reversion pendiente (la invalida) y saca el nodo
 *        de la cima. El dev debe haber aplicado ya la reversion
 *        usando historial_obtener_reversion().
 * @return 0 ok, -1 si no hay nada reversible
 */
int historial_deshacer(historial h);

/**
 * @brief Copia la reversión pendiente en *out sin consumirla.
 * @return 0 ok, -1 si no hay reversión pendiente
 */
int historial_obtener_reversion(const historial h, s_Reversion *out);

/**
 * @brief Imprime todo el historial en consola, del mas reciente al mas antiguo.
 * @param h puntero a historial
 */
void historial_imprimir(const historial h);
#endif
