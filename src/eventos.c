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

    printf("\n");
    ui_mensaje_aviso("!Encuentras una carta en el suelo!");
    ui_pausa_ms(PAUSA_MEDIA);
    printf("  ");
    imprimir_carta_sola(carta);
    printf("\nDeseas recogerla? ");

    if(leer_si_no())
    {
        agregar_carta(gs->inventario, carta, gs->inventario);
        ui_mensaje_exito("%s agregada al inventario.", carta.nombre);
    }
    else
    {
        ui_mensaje_info("Dejas la carta donde estaba.");
    }
    ui_pausa_ms(PAUSA_MEDIA);
}


void evento_desgracia(s_GameState *gs)
{
    int dano = 5 + rand() % 6;

    printf("\n");
    ui_mensaje_error("!Caes en una trampa de Greed Island! Pierdes %d de vida.", dano);
    gs->personaje.vida -= dano;

    if(gs->personaje.vida <= 0)
    {
        gs->personaje.vida = 0;
        ui_pantalla_game_over(gs->personaje.nombre);
        gs->ejecutando = false;
    }
    else
    {
        printf("  %s: ", gs->personaje.nombre);
        ui_barra_vida(gs->personaje.vida, gs->personaje.vidaMax, ANCHO_BARRA);
        printf("\n");
        ui_pausa_ms(PAUSA_MEDIA);
    }
}

int ejecutar_evento(s_GameState *gs, e_TipoEvento evento)
{
    switch(evento)
    {
        case COMBATE:
            combate(gs);
            return 1;
        case LOOTEO:
            evento_looteo(gs);
            return 1;
        case DESGRACIA:
            evento_desgracia(gs);
            return 0;
        case NADA:
        default:
            return 0;
    }
}
