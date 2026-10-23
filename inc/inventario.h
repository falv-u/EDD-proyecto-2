#ifndef INVENTARIO_H
#define INVENTARIO_H

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
int imprimir_cartas(Lista L);
Posicion encontrar_carta(Lista I, int posicion);
Lista crear_inventario(Lista I);

#endif