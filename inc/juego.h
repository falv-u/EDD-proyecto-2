#ifndef JUEGO_H
#define JUEGO_H

#include "carta.h"
#include "combate.h"
#include "iniciar_juego.h"
#include "inventario.h"
#include "personaje.h"
#include "comunes.h"

typedef struct s_GameState
{
    s_Personaje personaje;
} s_GameState;

#endif