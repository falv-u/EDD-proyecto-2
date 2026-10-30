#include "juego.h"

void crear_personaje(s_Personaje *pj, int vida, int vidaMax, int ataque, int defensa, const char *nombre)
{
    pj->vida = vida;
    pj->vidaMax = vidaMax;
    pj->ataque = ataque;
    pj->defensa = defensa;

    char *copiaNombre = malloc(strlen(nombre) + 1);

    if(copiaNombre == NULL)
    {
        ui_mensaje_error("No se pudo reservar memoria para el nombre");
        pj->nombre = NULL;
        return;
    }

    strcpy(copiaNombre, nombre);
    pj->nombre = copiaNombre;
}

void asignar_personaje(s_Personaje *pj)
{
    int opcion = -1;

    ui_limpiar_pantalla();
    ui_titulo("ELIGE A TU PERSONAJE", COLOR_AMARILLO);

    printf("\n%s1. Gon%s\n   Vida maxima: 110 | Ataque: 12 | Defensa: 6\n", COLOR_VERDE, COLOR_RESET);
    printf("\n%s2. Killua%s\n   Vida maxima: 90 | Ataque: 15 | Defensa: 4\n", COLOR_CIAN, COLOR_RESET);
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
            ui_mensaje_error("Opcion invalida");
            break;
    }

    if(pj->nombre != NULL)
    {
        printf("\n");
        ui_mensaje_exito("Has elegido a %s!", pj->nombre);
        ui_pausa_ms(PAUSA_MEDIA);
    }
}

void imprimir_personaje(s_Personaje *pj)
{
    ui_titulo(pj->nombre != NULL ? pj->nombre : "???", COLOR_VERDE);
    printf("Vida     ");
    ui_barra_vida(pj->vida, pj->vidaMax, ANCHO_BARRA);
    printf("\nAtaque   %s%d%s\n", COLOR_ROJO, pj->ataque, COLOR_RESET);
    printf("Defensa  %s%d%s\n", COLOR_AZUL, pj->defensa, COLOR_RESET);
    ui_linea('-', COLOR_VERDE);
}