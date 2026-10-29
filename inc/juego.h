#ifndef JUEGO_H
#define JUEGO_H

#include "carta.h"
#include "cola.h"
#include "combate.h"
#include "comunes.h"
#include "enemigo.h"
#include "eventos.h"
#include "exploracion.h"
#include "historial.h"
#include "inventario.h"
#include "partida.h"
#include "personaje.h"

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