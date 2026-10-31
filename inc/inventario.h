#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <stdbool.h>
#include "carta.h"

typedef struct s_GameState s_GameState;

typedef struct s_Inventario *PtrAlNodo;
typedef PtrAlNodo Lista;
typedef PtrAlNodo Posicion;

typedef struct s_Inventario
{
    s_Carta carta;
    Posicion next;
    Posicion prev;
} s_Inventario;

/**
 * @brief Muestra el menu del inventario y permite realizar operaciones relacionadas al inventario
 * 
 * @param gs Puntero al estado del juego.
 */
void inventario(s_GameState *gs);

/**
 * @brief Elimina todas las cartas del inventario sin eliminar el nodo centinela.
 * 
 * @param I Lista del inventario.
 */
void vaciar_inventario(Lista I);

/**
 * @brief Agrega una carta después de una posición determinada del inventario. 
 * 
 * @param I Lista del inventario.
 * @param carta Carta a agregar
 * @param P Posicion despues de la cual se insertara la carta
 */
void agregar_carta(Lista I, s_Carta carta, Posicion P);

/**
 * @brief Elimina una carta del inventario y actualiza los enlaces entre los nodos.
 * 
 * @param I Lista del inventario.
 * @param P Posicion de la carta que se quiere eliminar.
 */
void eliminar_carta(Lista I, Posicion P);

/**
 * @brief Muestra todas las cartas del inventario.
 * 
 * @param I Lista del inventario.
 */
void imprimir_cartas(Lista L);

/**
 * @brief Registra el descarte de una carta en el historial y la elimina del inventario.
 *
 * @param gs Puntero al estado del juego
 * @param puntCarta Puntero a la carta que se quiere descartar
 */
void descartar_carta(s_GameState *gs, Posicion puntCarta);

/**
 * @brief Deshace el descarte de la ultima carta: la reinserta en el
 *        inventario en la posicion original y saca la accion del historial.
 *
 * Solo se puede deshacer si la ultima accion registrada fue descartar una
 * carta; en otro caso avisa al jugador y no modifica nada.
 *
 * @param gs Puntero al estado del juego
 * @return true si se deshizo el descarte, false si no era posible
 */
bool restaurar_carta(s_GameState *gs);

/**
 * @brief Cuenta y devuelve la cantidad de cartas que hay en el inventario.
 * 
 * @param I Lista del inventario.
 * @return int Cantidad de cartas almacenadas en el inventario.
 */
int cantidad_cartas(Lista I);

/**
 * @brief Pregunta si el jugador desea utilizarla.
 * 
 * @param puntCarta Posicion de la carta que se quiere utilizar
 * @return true 
 * @return false 
 */
bool confirmar_uso_carta(Posicion puntCarta);

/**
 * @brief Busca una carta por su número de posición en el inventario.
 * 
 * @param I Lista del inventario.
 * @param posicion Numero de la carta a buscar.
 * @return Posicion de la carta o NULL si no existe.
 */
Posicion encontrar_carta(Lista I, int posicion);

/**
 * @brief Busca una carta del inventario mediante su nombre.
 * 
 * @param I Lista del inventario.
 * @param nombre Nombre de la carta a desea buscar.
 * @return Posicion de la carta o NULL si no se encuentra.
 */
Posicion buscar_carta_nombre(Lista I, const char *nombre);

/**
 * @brief Busca una carta del inventario mediante su id unico.
 *
 * @param I  Lista del inventario.
 * @param id Identificador de la carta a buscar.
 * @return Posicion de la carta o NULL si no se encuentra.
 */
Posicion buscar_carta_id(Lista I, int id);

/**
 * @brief Permite seleccionar una carta del inventario ingresando su ID.
 *
 * @param I Lista del inventario.
 * @return Posicion seleccionada o NULL si no es valida.
 */
Posicion seleccionar_carta(Lista I);

/**
 * @brief Crea un inventario con nodo centinela o vacía uno ya existente.
 * 
 * @param I Lista del inventario.
 * @return Lista del inventario o NULL si ocurre un error.
 */
Lista crear_inventario(Lista I);

/**
 * @brief Limpia la pantalla y muestra el menu del inventario.
 * @param cantidad Cantidad de cartas guardadas.
 */
void mostrar_menu_inventario(int cantidad);


/**
 * @brief Pide un nombre por teclado y muestra la carta encontrada (si existe).
 * @param I Lista del inventario (no NULL).
 */
void buscar_carta_interactivo(Lista I);

/**
 * @brief Pide un ID por teclado y muestra la carta encontrada (si existe).
 * @param I Lista del inventario (no NULL).
 */
void buscar_carta_id_interactivo(Lista I);

#endif