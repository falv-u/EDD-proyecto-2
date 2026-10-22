#ifndef PERSONAJE_H
#define PERSONAJE_H

typedef struct s_GameState s_GameState;

typedef struct 
{
    int vida;
    int ataque;
    int defensa;
    int vidaMax;
    const char *nombre;
} s_Personaje;

void personaje();
void crear_personaje(s_Personaje *pj, int vida, int vidaMax, int ataque, int defensa, const char *nombre);
void asignar_personaje(s_Personaje *pj);
void imprimir_personaje(s_Personaje *pj);

#endif