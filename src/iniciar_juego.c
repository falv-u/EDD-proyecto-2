#include "juego.h"

void iniciar_juego(s_GameState *gs)
{
    gs->ejecutando = true;
    asignar_personaje(&gs->personaje);
    imprimir_personaje(&gs->personaje);

    for(int i = 0; i < 3; i++)
    {
        agregar_carta(gs->inventario, carta_aleatoria(), gs->inventario);
    }
}