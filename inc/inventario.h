#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <stdbool.h>
#include "carta.h"

typedef struct s_GameState s_GameState;

typedef struct s_Inventario *PtrAlNodo;
typedef PtrAlNodo Lista;
typedef PtrAlNodo Posicion;

typedef struct s_Inventario
{
    s_Carta carta;
    Posicion next;
    Posicion prev;
} s_Inventario;


void inventario(s_GameState *gs);
void vaciar_inventario(Lista I);
void agregar_carta(Lista I, s_Carta carta, Posicion P);
void eliminar_carta(Lista I, Posicion P);
void imprimir_cartas(Lista L);
void descartar_carta(s_GameState *gs, Posicion puntCarta);
int cantidad_cartas(Lista L);
bool usar_carta(s_GameState *gs, Posicion puntCarta);
bool confirmar_uso_carta(Posicion puntCarta);
Posicion encontrar_carta(Lista I, int posicion);
Posicion buscar_carta_nombre(Lista I, const char *nombre);
Posicion seleccionar_carta(Lista I);
Lista crear_inventario(Lista I);

#endif