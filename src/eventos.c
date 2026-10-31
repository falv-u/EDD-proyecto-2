#include "juego.h"


e_TipoEvento definir_evento()
{
    int prob = rand()%100;

    if(prob < 40)
        return COMBATE;
    else if(prob<60)
        return LOOTEO;
    else if(prob<70)
        return DESGRACIA;
    else
        return NADA;

}

/** Encuentra una carta aleatoria y pregunta si se recoge. */
void evento_looteo(s_GameState *gs)
{
    s_Carta carta = carta_aleatoria();
    char descripcion[128];

    printf("\n");
    ui_mensaje_aviso("!Encuentras una carta en el suelo!");
    snprintf(descripcion, sizeof(descripcion), "Se encontró %s",
             carta.nombre ? carta.nombre : "una carta");
    historial_registrar(gs->Historial, descripcion, A_REGISTRO, NULL);
    ui_pausa_ms(PAUSA_MEDIA);
    printf("  ");
    imprimir_carta_sola(carta);
    printf("\nDeseas recogerla? ");

    if(leer_si_no())
    {
        agregar_carta(gs->inventario, carta, gs->inventario);
        ui_mensaje_exito("%s agregada al inventario.", carta.nombre);
        snprintf(descripcion, sizeof(descripcion), "Se recogió %s",
                 carta.nombre ? carta.nombre : "una carta");
        historial_registrar(gs->Historial, descripcion, A_REGISTRO, NULL);
    }
    else
    {
        ui_mensaje_info("Dejas la carta donde estaba.");
        snprintf(descripcion, sizeof(descripcion), "Se dejó %s",
                 carta.nombre ? carta.nombre : "una carta");
        historial_registrar(gs->Historial, descripcion, A_REGISTRO, NULL);
    }
    ui_pausa_ms(PAUSA_MEDIA);
}


void evento_desgracia(s_GameState *gs)
{
    int dano = 5 + rand() % 6;
    char descripcion[128];

    printf("\n");
    ui_mensaje_error("!Caes en una trampa de Greed Island! Pierdes %d de vida.", dano);
    gs->personaje.vida -= dano;

    snprintf(descripcion, sizeof(descripcion), "Cayó en una trampa (-%d PV)", dano);
    historial_registrar(gs->Historial, descripcion, A_REGISTRO, NULL);

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
