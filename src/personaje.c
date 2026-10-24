#include "juego.h"
/*
void personaje(s_GameState *gs)
{
    
}*/

void crear_personaje(s_Personaje *pj, int vida, int vidaMax, int ataque, int defensa, const char *nombre)
{
    pj->vida = vida;
    pj->vidaMax = vidaMax;
    pj->ataque = ataque;
    pj->defensa = defensa;
    pj->nombre = nombre;
}

void asignar_personaje(s_Personaje *pj)
{
    int opcion = -1;

    printf("\nSeleccione un personaje\n");
    printf("\n1. Gon\n\nVida maxima: 110\nAtaque: 12\nDefensa: 6\n");
    printf("\n2. Killua\n\nVida maxima: 90\nAtaque: 15\nDefensa: 4\n");
    imprimir_ingresar_opcion();
    opcion = leer_entero_en_rango(1, 2);

    switch(opcion)
    {
        case 1:
            crear_personaje(pj, 110, 110, 12, 6, "Gon");
            break;
        case 2:
            crear_personaje(pj, 90, 90, 15, 4, "Killua");
            break;
        default:
            printf("\nError: opcion invalida\n");
            break;
    }
}

void imprimir_personaje(s_Personaje *pj)
{
    printf("\n<<%s>>\n\nVida maxima: %d\nVida actual: %d\nAtaque: %d\nDefensa: %d\n", pj->nombre, pj->vidaMax, pj->vida, pj->ataque, pj->defensa);
}