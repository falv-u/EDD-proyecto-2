#include "juego.h"

int main(void)
{
    s_GameState gs = {0};

    asignar_personaje(&gs.personaje);
    imprimir_personaje(&gs.personaje);

    return 0;
}