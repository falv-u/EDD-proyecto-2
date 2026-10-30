/**
 * @file ui.c
 * @brief Implementacion de las utilidades de interfaz de terminal.
 */
 
/* Necesario antes de los includes para tener nanosleep() incluso con -std=c99 */
#define _POSIX_C_SOURCE 200809L
 
#include <stdio.h>
#include <string.h>
#include <time.h>
 
#include "ui.h"
 
void ui_limpiar_pantalla(void)
{
    printf("\033[2J\033[H");
    fflush(stdout);
}
 
void ui_pausa_ms(int milisegundos)
{
    struct timespec tiempo;
 
    fflush(stdout);
    if (milisegundos <= 0)
    {
        return;
    }
 
    tiempo.tv_sec = milisegundos / 1000;
    tiempo.tv_nsec = (long)(milisegundos % 1000) * 1000000L;
    nanosleep(&tiempo, NULL);
}
 
void ui_esperar_enter(void)
{
    int c;
 
    printf("\n%sPresiona ENTER para continuar...%s", COLOR_BLANCO, COLOR_RESET);
    fflush(stdout);
 
    c = getchar();
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}
 
void ui_linea(char caracter, const char *color)
{
    int i;
 
    printf("%s", color);
    for (i = 0; i < ANCHO_UI; i++)
    {
        putchar(caracter);
    }
    printf("%s\n", COLOR_RESET);
}
 
void ui_titulo(const char *texto, const char *color)
{
    int margen;
 
    if (texto == NULL)
    {
        return;
    }
 
    margen = (ANCHO_UI - (int)strlen(texto)) / 2;
    if (margen < 0)
    {
        margen = 0;
    }
 
    ui_linea('=', color);
    printf("%*s%s%s%s\n", margen, "", color, texto, COLOR_RESET);
    ui_linea('=', color);
}
 
void ui_escribir_lento(const char *texto, int msPorCaracter)
{
    int i;
 
    if (texto == NULL)
    {
        return;
    }
 
    for (i = 0; texto[i] != '\0'; i++)
    {
        putchar(texto[i]);
        ui_pausa_ms(msPorCaracter);
    }
    fflush(stdout);
}
 
void ui_mensaje_con_etiqueta(const char *color, const char *etiqueta,
                             const char *formato, va_list argumentos)
{
    printf("%s%s%s ", color, etiqueta, COLOR_RESET);
    vprintf(formato, argumentos);
    printf("\n");
}
 
void ui_mensaje_info(const char *formato, ...)
{
    va_list argumentos;
 
    va_start(argumentos, formato);
    ui_mensaje_con_etiqueta(COLOR_CIAN, "[i]", formato, argumentos);
    va_end(argumentos);
}
 
void ui_mensaje_exito(const char *formato, ...)
{
    va_list argumentos;
 
    va_start(argumentos, formato);
    ui_mensaje_con_etiqueta(COLOR_VERDE, "[OK]", formato, argumentos);
    va_end(argumentos);
}
 
void ui_mensaje_aviso(const char *formato, ...)
{
    va_list argumentos;
 
    va_start(argumentos, formato);
    ui_mensaje_con_etiqueta(COLOR_AMARILLO, "[!]", formato, argumentos);
    va_end(argumentos);
}
 
void ui_mensaje_error(const char *formato, ...)
{
    va_list argumentos;
 
    va_start(argumentos, formato);
    ui_mensaje_con_etiqueta(COLOR_ROJO, "[ERROR]", formato, argumentos);
    va_end(argumentos);
}
 
void ui_opcion_menu(int numero, const char *texto)
{
    printf(" %s%d.%s %s\n", COLOR_AMARILLO, numero, COLOR_RESET, texto);
}
 
void ui_barra_vida(int actual, int maximo, int ancho)
{
    const char *color;
    int llenos;
    int porcentaje;
    int i;
 
    if (maximo <= 0 || ancho <= 0)
    {
        return;
    }
 
    if (actual < 0)
    {
        actual = 0;
    }
    if (actual > maximo)
    {
        actual = maximo;
    }
 
    llenos = (actual * ancho) / maximo;
    porcentaje = (actual * 100) / maximo;
 
    if (porcentaje > 50)
    {
        color = COLOR_VERDE;
    }
    else if (porcentaje > 25)
    {
        color = COLOR_AMARILLO;
    }
    else
    {
        color = COLOR_ROJO;
    }
 
    printf("[%s", color);
    for (i = 0; i < ancho; i++)
    {
        putchar(i < llenos ? '#' : '-');
    }
    printf("%s] %s%d/%d%s", COLOR_RESET, color, actual, maximo, COLOR_RESET);
}
 
void ui_pantalla_titulo(void)
{
    ui_limpiar_pantalla();
    printf("\n\n");
    ui_titulo("G R E E D   I S L A N D", COLOR_AMARILLO);
    printf("\n%s", COLOR_CIAN);
    ui_escribir_lento("   Un RPG inspirado en Hunter x Hunter...", VELOCIDAD_TEXTO);
    printf("%s\n", COLOR_RESET);
    ui_pausa_ms(PAUSA_MEDIA);
    ui_esperar_enter();
}
 
void ui_pantalla_game_over(const char *nombre)
{
    ui_limpiar_pantalla();
    printf("\n\n");
    ui_titulo("G A M E   O V E R", COLOR_ROJO);
    printf("\n%s  %s ha caido en Greed Island...\n  ", COLOR_ROJO,
           nombre != NULL ? nombre : "El jugador");
    ui_escribir_lento("Tu aventura termina aqui.", VELOCIDAD_TEXTO);
    printf("%s\n", COLOR_RESET);
    ui_pausa_ms(PAUSA_MEDIA);
    ui_esperar_enter();
}