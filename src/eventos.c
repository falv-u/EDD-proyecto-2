#include "juego.h"


int definir_evento()
{
    int prob = rand()%100;

    if(prob < 40)
        return 1;
    else if(prob<60)
        return 2;
    else if(prob<70)
        return 3;
    else 
        return 0;

}

/** Encuentra una carta aleatoria y pregunta si se recoge. */
void evento_looteo(s_GameState *gs)
{
    s_Carta carta = carta_aleatoria();

    printf("\n!Encuentras una carta en el suelo!\n");
    imprimir_carta_sola(carta);
    printf("Deseas recogerla? ");

    if(leer_si_no())
    {
        agregar_carta(gs->inventario, carta, gs->inventario);
        printf("%s agregada al inventario.\n", carta.nombre);
    }
    else
    {
        printf("Dejas la carta donde estaba.\n");
    }

}


void evento_desgracia(s_GameState *gs)
{
    int dano = 5 + rand() % 6;

    printf("\n!Caes en una trampa de Greed Island! Pierdes %d de vida.\n", dano);
    gs->personaje.vida -= dano;

    if(gs->personaje.vida <= 0)
    {
        gs->personaje.vida = 0;
        printf("%s ha caido... GAME OVER\n", gs->personaje.nombre);
        gs->ejecutando = false;
    }
    else
    {
        printf("%s: %d/%d PV\n", gs->personaje.nombre, gs->personaje.vida, gs->personaje.vidaMax);
    }
}

int ejecutar_evento(s_GameState *gs, e_TipoEvento evento)
{
    switch(evento)
    {
        case COMBATE:
            // evento_combate(gs);
            printf("evento combate \n");
            return 1;
        case LOOTEO:
            // evento_looteo(gs)
            printf("evento loot\n");
            return 1;
        case DESGRACIA:
            evento_desgracia(gs);
            printf("evento desgracia\n");
            return 0;
        case NADA:
        default:
            return 0;
    }
}
