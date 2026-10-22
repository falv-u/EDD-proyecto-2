#include "juego.h"

void cartas(s_GameState *gs)
{
   const s_Carta cartas[] = 
   {
        {"Aliento del Arcangel", CURACION, 0},
        {"Espada de la Verdad", ATAQUE, 5},
        {"Martillo Eterno", ATAQUE, 7},
        {"Collar del Paladin", DEFENSA, 2},
        {"Armadura del Sacrificio", DEFENSA, 5},
        {"Balanza Dorada", ESPECIAL, 0},
        {"Restaurador Vital", CURACION, 15},
        {"Genio Caprichoso", ESPECIAL, 0},
        {"Pocion Rejuvenecedora", VIDAMAXIMA, 10},
        {"Esteroides", VIDAMAXIMA, 20}
   };
}