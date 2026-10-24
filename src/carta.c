#include "juego.h"

s_Carta carta_aleatoria()
{
   const s_Carta cartas_disp[] = 
   {
        {"Aliento del Arcangel", CURACION, 0},
        {"Espada de la Verdad", ATAQUE, 5},
        {"Martillo Eterno", ATAQUE, 7},
        {"Collar del Paladin", DEFENSA, 2},
        {"Armadura del Sacrificio", DEFENSA, 5},
        {"Balanza Dorada", ESPECIAL, 0},
        {"Restaurador Vital", CURACION, 30},
        {"Genio Caprichoso", ESPECIAL, 0},
        {"Pocion Rejuvenecedora", VIDAMAXIMA, 10},
        {"Esteroides", VIDAMAXIMA, 20}
   };

   int porcentaje = rand()%100;
   int pos = -1;
   s_Carta carta;

   if(porcentaje <  5) carta = cartas_disp[0];
   else if(porcentaje < 20) carta = cartas_disp[1];
   else if(porcentaje < 30) carta = cartas_disp[2];
   else if(porcentaje < 45) carta = cartas_disp[3];
   else if(porcentaje < 55) carta = cartas_disp[4];
   else if(porcentaje < 60) carta = cartas_disp[5];
   else if(porcentaje < 75) carta = cartas_disp[6];
   else if(porcentaje < 85)carta = cartas_disp[7];
   else if(porcentaje < 95) carta = cartas_disp[8];
   else if(porcentaje < 100) carta = cartas_disp[9];

   return carta;
}

void imprimir_carta_sola(s_Carta carta)
{
    static const char *tipos[] = {"ataque", "curacion", "defensa", "vida maxima", "especial"};

    printf(" [%s] tipo: %s, valor: %d\n", carta.nombre, tipos[carta.tipo], carta.valor);
}