#ifndef JUEGO_H
#define JUEGO_H

#include "carta.h"
#include "historial.h"
#include "combate.h"
#include "iniciar_juego.h"
#include "inventario.h"
#include "personaje.h"
#include "comunes.h"
#include "eventos.h"
#include "exploracion.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct s_GameState
{
    s_Personaje personaje;
    Lista inventario;
    bool ejecutando;
    historial Historial;
} s_GameState;

#endif