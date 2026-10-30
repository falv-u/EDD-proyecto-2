#ifndef EXPLORACION_H
#define EXPLORACION_H

#include <stdio.h>
#include <stdbool.h>
#include "eventos.h"

typedef struct s_GameState s_GameState;

/* Cantidad de zonas explorables */
#define CANTIDAD_ZONAS 3

/**
 * @brief Devuelve el nombre de la zona indicada.
 * @param zona 0 = Antokiba, 1 = Masadora, 2 = Soufrabi
 * @return Nombre de la zona (o "Desconocida" si la zona es invalida).
 */
const char *nombre_zona(int zona);

/**
 * @brief Explora una zona hasta que ocurra un combate o un loot,
 *        y pregunta si se quiere seguir explorando.
 * @param zona 0 = Antokiba, 1 = Masadora, 2 = Soufrabi
 */
void exploracion(s_GameState *gs, int tipo_evento);
void explorar_hasta_evento(s_GameState *gs);

/**
 * @brief Muestra una frase de caminata con efecto de escritura.
 * @param encontroAlgo true si en este paso ocurre un evento.
 */
void mostrar_paso_exploracion(bool encontroAlgo);

#endif