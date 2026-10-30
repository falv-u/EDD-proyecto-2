#ifndef UI_H
#define UI_H
 
#include <stdarg.h>
 
/**
 * @file ui.h
 * @brief Utilidades de interfaz de terminal: colores, limpieza de pantalla,
 *        pausas, mensajes con formato y elementos graficos en modo texto.
 *
 * Solo usa secuencias ANSI basicas (8 colores en negrita) y caracteres ASCII,
 * por lo que funciona en cualquier terminal Linux.
 */
 
/* Colores ANSI basicos (en negrita para que se vean bien en fondos oscuros) */
#define COLOR_RESET    "\033[0m"
#define COLOR_ROJO     "\033[1;31m"
#define COLOR_VERDE    "\033[1;32m"
#define COLOR_AMARILLO "\033[1;33m"
#define COLOR_AZUL     "\033[1;34m"
#define COLOR_MAGENTA  "\033[1;35m"
#define COLOR_CIAN     "\033[1;36m"
#define COLOR_BLANCO   "\033[1;37m"
 
/* Medidas de la interfaz */
#define ANCHO_UI 44          /* ancho de lineas y titulos */
#define ANCHO_BARRA 15       /* ancho de las barras de vida */
 
/* Pausas en milisegundos */
#define PAUSA_CORTA 400
#define PAUSA_MEDIA 800
#define PAUSA_LARGA 1500
#define VELOCIDAD_TEXTO 25   /* ms por caracter en el efecto maquina de escribir */
 
/** @brief Limpia la pantalla y deja el cursor arriba a la izquierda. */
void ui_limpiar_pantalla(void);
 
/**
 * @brief Detiene la ejecucion por un tiempo (vacia stdout antes de dormir).
 * @param milisegundos Tiempo de espera (si es <= 0 solo vacia el buffer).
 */
void ui_pausa_ms(int milisegundos);
 
/** @brief Muestra "Presiona ENTER para continuar" y espera la tecla. */
void ui_esperar_enter(void);
 
/**
 * @brief Imprime una linea horizontal de ANCHO_UI caracteres.
 * @param caracter Caracter de relleno (por ejemplo '=' o '-').
 * @param color    Secuencia de color (por ejemplo COLOR_CIAN).
 */
void ui_linea(char caracter, const char *color);
 
/**
 * @brief Imprime un titulo centrado entre dos lineas.
 * @param texto Texto del titulo (no NULL).
 * @param color Color de las lineas y del texto.
 */
void ui_titulo(const char *texto, const char *color);
 
/**
 * @brief Escribe un texto caracter por caracter (efecto maquina de escribir).
 * @param texto            Texto a escribir (no NULL).
 * @param msPorCaracter    Espera entre caracteres (<= 0 imprime de una vez).
 */
void ui_escribir_lento(const char *texto, int msPorCaracter);
 
/**
 * @brief Imprime un mensaje con etiqueta de color usando va_list.
 * @param color     Color de la etiqueta.
 * @param etiqueta  Texto de la etiqueta (por ejemplo "[OK]").
 * @param formato   Formato estilo printf.
 * @param argumentos Argumentos variables ya iniciados con va_start.
 */
void ui_mensaje_con_etiqueta(const char *color, const char *etiqueta,
                             const char *formato, va_list argumentos);
 
/** @brief Mensaje informativo (cian). Acepta formato estilo printf. */
void ui_mensaje_info(const char *formato, ...);
 
/** @brief Mensaje de exito (verde). Acepta formato estilo printf. */
void ui_mensaje_exito(const char *formato, ...);
 
/** @brief Mensaje de aviso (amarillo). Acepta formato estilo printf. */
void ui_mensaje_aviso(const char *formato, ...);
 
/** @brief Mensaje de error (rojo). Acepta formato estilo printf. */
void ui_mensaje_error(const char *formato, ...);
 
/**
 * @brief Imprime una opcion de menu numerada ("1. Texto").
 * @param numero Numero de la opcion.
 * @param texto  Descripcion de la opcion.
 */
void ui_opcion_menu(int numero, const char *texto);
 
/**
 * @brief Imprime una barra de vida con color segun el porcentaje restante
 *        (verde > 50%, amarillo > 25%, rojo en otro caso) y "actual/maximo".
 * @param actual Vida actual.
 * @param maximo Vida maxima (debe ser > 0).
 * @param ancho  Cantidad de caracteres de la barra.
 */
void ui_barra_vida(int actual, int maximo, int ancho);
 
/** @brief Pantalla de bienvenida del juego. */
void ui_pantalla_titulo(void);
 
/**
 * @brief Pantalla de fin del juego.
 * @param nombre Nombre del personaje caido (puede ser NULL).
 */
void ui_pantalla_game_over(const char *nombre);
 
#endif
 