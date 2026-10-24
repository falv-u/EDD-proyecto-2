#ifndef EXPLORACION_H
#define EXPLORACION_H

#include <stdio.h>
#include "eventos.h"

typedef struct s_GameState s_GameState;


/**
 * @brief Explora una zona hasta que ocurra un combate o un loot,
 *        y pregunta si se quiere seguir explorando.
 * @param zona 0 = Antokiba, 1 = Masadora, 2 = Soufrabi
 */
void exploracion(s_GameState *gs, int tipo_evento);

#endif