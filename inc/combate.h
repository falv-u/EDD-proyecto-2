#ifndef COMBATE_H
#define COMBATE_H

#include "juego.h"

#define MAX_GRUPO 3 //maximos de enemigos por grupo
#define PROB_CRITICOS 10 //% de golpe critico

typedef struct s_GameState s_GameState;

void combate(s_GameState *gs);

int calcular_danho(int ataque,int defensa, bool *critico);

void atacar_enemigo(const s_GameState *gs, s_Enemigo *enemigo);

void mostrar_estado(const s_GameState *gs, const s_Enemigo *enemigo, int enEspera);

bool usar_carta_en_combate(s_GameState *gs, s_Enemigo *enemigo_actual);

Cola generar_grupo();
#endif