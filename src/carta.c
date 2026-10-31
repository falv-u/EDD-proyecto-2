#include "juego.h"

/* Ultimo id generado: los ids de carta son correlativos y unicos */
static int ultimo_id_carta = 0;

int generar_id_carta()
{
    return ++ultimo_id_carta;
}

s_Carta carta_aleatoria()
{
   s_Carta cartas_disp[] =
   {
        {"Aliento del Arcangel", CURACION, 0, 0},
        {"Espada de la Verdad", ATAQUE, 5, 0},
        {"Martillo Eterno", ATAQUE, 7, 0},
        {"Collar del Paladin", DEFENSA, 2, 0},
        {"Armadura del Sacrificio", DEFENSA, 5, 0},
        {"Armadura Dorada", DEFENSA, 3, 0},
        {"Restaurador Vital", CURACION, 30, 0},
        {"Genio Curativo", CURACION, 20, 0},
        {"Pocion Rejuvenecedora", VIDAMAXIMA, 10, 0},
        {"Esteroides", VIDAMAXIMA, 20, 0}
   };

   int porcentaje = rand()%100;
   s_Carta carta = {0};

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

   /* Cada carta generada recibe un id unico */
   carta.id = generar_id_carta();

   return carta;
}

void imprimir_carta_sola(s_Carta carta)
{
    const char *tipos[] = {"ataque", "curacion", "defensa", "vida maxima", "especial"};

    printf("%s[%s]%s (ID %d) tipo: %s, ", color_tipo_carta(carta.tipo), carta.nombre,
           COLOR_RESET, carta.id, tipos[carta.tipo]);

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