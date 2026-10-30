#include "juego.h"

#define PASOS_POR_EVENTO 3
#define CANTIDAD_FRASES 4
#define CANTIDAD_ZONAS 3

static const char *nombresZona[CANTIDAD_ZONAS] = {"Antokiba", "Masadora", "Soufrabi"};

static const char *frasesCaminata[CANTIDAD_FRASES] =
{
    "Caminas entre los arboles.",
    "Sigues por el sendero.",
    "Cruzas un pequeno arroyo.",
    "Avanzas entre la neblina."
};

void mostrar_paso_exploracion(bool encontroAlgo)
{
    printf("%s", COLOR_CIAN);
    ui_escribir_lento(frasesCaminata[rand() % CANTIDAD_FRASES], 20);
    printf("%s", COLOR_RESET);

    if(!encontroAlgo)
        printf(" No encuentras nada.");
    printf("\n");
    ui_pausa_ms(PAUSA_MEDIA);
}

/** Camina paso a paso; cada PASOS_POR_EVENTO pasos se genera un evento. */
void explorar_hasta_evento(s_GameState *gs)
{
    int pasos = 0;
    int eventoFinal = 0;

    while(!eventoFinal && gs->ejecutando)
    {
        e_TipoEvento evento = NADA;

        pasos++;
        if(pasos % PASOS_POR_EVENTO == 0)
            evento = definir_evento();

        mostrar_paso_exploracion(evento != NADA);
        eventoFinal = ejecutar_evento(gs, evento);
        sleep(1);
    }
}

void exploracion(s_GameState *gs, int zona)
{
    bool seguir = true;

    if(zona < 0 || zona >= CANTIDAD_ZONAS)
    {
        ui_mensaje_error("Zona invalida");
        return;
    }

    ui_limpiar_pantalla();
    ui_titulo(nombresZona[zona], COLOR_VERDE);
    printf("\n%sEntras a %s...%s\n\n", COLOR_VERDE, nombresZona[zona], COLOR_RESET);
    ui_pausa_ms(PAUSA_LARGA);

    while(seguir && gs->ejecutando)
    {
        explorar_hasta_evento(gs);

        if(gs->ejecutando)
        {
            printf("\nSeguir explorando? ");
            seguir = leer_si_no();

            if(seguir)
                ui_limpiar_pantalla();
        }
    }

    if(gs->ejecutando)
    {
        ui_mensaje_info("Regresas al campamento...");
        ui_pausa_ms(PAUSA_MEDIA);
    }
}