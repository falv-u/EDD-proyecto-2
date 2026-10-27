#include <stdlib.h>
#include <stdio.h>
#include "enemigo.h"
 
s_Enemigo enemigo_aleatorio(int id)
{
    /* {id, nombre, vida, vidaMax, ataque, defensa} */
    const s_Enemigo plantillas[] =
    {
        {0, "Goblin",           30, 30, 10, 2},
        {0, "Lobo Salvaje",     40, 40, 12, 3},
        {0, "Bandido",          45, 45, 14, 4},
        {0, "Ogro",             70, 70, 16, 6},
        {0, "Bestia de Greed",  90, 90, 20, 8}
    };
    int cantidad = sizeof(plantillas) / sizeof(plantillas[0]);
    s_Enemigo enemigo = plantillas[rand() % cantidad];
 
    enemigo.id = id;
    return enemigo;
}
 