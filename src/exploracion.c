#include "juego.h"

#define PASOS_POR_EVENTO 3
#define CANTIDAD_FRASES 4

static const char *nombresZona[] = {"Antokiba", "Masadora", "Soufrabi"};

static const char *frasesCaminata[CANTIDAD_FRASES] =
{
    "Caminas entre los arboles.",
    "Sigues por el sendero.",
    "Cruzas un pequeno arroyo.",
    "Avanzas entre la neblina."
};

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

        printf("%s", frasesCaminata[rand() % CANTIDAD_FRASES]);
        if(evento == NADA)
            printf(" No encuentras nada.");
        printf("\n");

        eventoFinal = ejecutar_evento(gs, evento);
        sleep(1);
    }
}

void exploracion(s_GameState *gs, int zona)
{
    bool seguir = true;

    printf("\nEntras a %s...\n", nombresZona[zona]);

    while(seguir && gs->ejecutando)
    {
        explorar_hasta_evento(gs);

        if(gs->ejecutando)
        {
            printf("\nSeguir explorando? ");
            seguir = leer_si_no();
        }
    }
}