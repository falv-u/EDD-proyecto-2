#include "juego.h"

int main(void)
{
    int pasos = 0;
    s_GameState gs = {0};

    gs.inventario = crear_inventario(NULL);
    iniciar_juego(&gs);
    imprimir_cartas(gs.inventario);

    while(gs.ejecutando)
    {
        pasos++;
        if(pasos > 3)
        {
            pasos = 0;
            //exploracion(&gs);
        }
        sleep(1);
    }

    return 0;
}