#include "juego.h"

s_Carta carta_aleatoria()
{
   s_Carta cartas_disp[] = 
   {
        {"Aliento del Arcangel", CURACION, 0},
        {"Espada de la Verdad", ATAQUE, 5},
        {"Martillo Eterno", ATAQUE, 7},
        {"Collar del Paladin", DEFENSA, 2},
        {"Armadura del Sacrificio", DEFENSA, 5},
        {"Armadura Dorada", DEFENSA, 3},
        {"Restaurador Vital", CURACION, 30},
        {"Genio Curativo", CURACION, 20},
        {"Pocion Rejuvenecedora", VIDAMAXIMA, 10},
        {"Esteroides", VIDAMAXIMA, 20}
   };

   int porcentaje = rand()%100;
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
    const char *tipos[] = {"ataque", "curacion", "defensa", "vida maxima", "especial"};

    printf("%s[%s]%s tipo: %s, ", color_tipo_carta(carta.tipo), carta.nombre,
           COLOR_RESET, tipos[carta.tipo]);

    if(carta.tipo == CURACION && carta.valor == 0)
        printf("recupera toda la vida\n");
    else
        printf("valor: %d\n", carta.valor);
}

void mostrar_descripcion_carta(s_Carta carta)
{
    const char *color = color_tipo_carta(carta.tipo);

    printf("\n");
    ui_titulo(carta.nombre, color);

    switch(carta.tipo)
    {
        case ATAQUE:
            printf("Efecto: Aumenta el ataque en %d puntos\n", carta.valor);
            break;

        case DEFENSA:
            printf("Efecto: Aumenta la defensa en %d puntos\n", carta.valor);
            break;

        case CURACION:
            if(carta.valor == 0)
                printf("Efecto: Recupera toda la vida\n");
            else
                printf("Efecto: Recupera %d puntos de vida\n", carta.valor);
            break;

        case VIDAMAXIMA:
            printf("Efecto: Aumenta la vida maxima en %d puntos\n", carta.valor);
            break;

        case ESPECIAL:
            if(strcmp(carta.nombre, "Balanza Dorada") == 0)
                printf("Efecto: Permite evitar un combate\n");
            else if(strcmp(carta.nombre, "Genio Caprichoso") == 0)
                printf("Efecto: Activa un efecto aleatorio\n");
            break;
    }

    ui_linea('-', color);
}

const char *color_tipo_carta(e_TipoCarta tipo)
{
    switch(tipo)
    {
        case ATAQUE:     return COLOR_ROJO;
        case CURACION:   return COLOR_VERDE;
        case DEFENSA:    return COLOR_AZUL;
        case VIDAMAXIMA: return COLOR_MAGENTA;
        case ESPECIAL:   return COLOR_AMARILLO;
        default:         return COLOR_BLANCO;
    }
}