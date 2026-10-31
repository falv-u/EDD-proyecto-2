#ifndef CARTA_H
#define CARTA_H

typedef struct s_GameState s_GameState;

typedef enum
{
    ATAQUE,
    CURACION,
    DEFENSA,
    VIDAMAXIMA,
    ESPECIAL
} e_TipoCarta;

typedef struct
{
    char *nombre;
    e_TipoCarta tipo;
    int valor;
    int id;       /* identificador unico de la carta */
} s_Carta;

/**
 * @brief Genera una carta aleatoria por probabilidad
 *
 * @return Carta generada aleatoriamente (con id unico asignado).
 */
s_Carta carta_aleatoria();

/**
 * @brief Genera el siguiente identificador unico de carta.
 *
 * Los ids son correlativos y nunca se repiten dentro de una misma
 * ejecucion del juego.
 *
 * @return Id unico para una nueva carta.
 */
int generar_id_carta();

/**
 * @brief Muestra el nombre, tipo y valor de una carta
 * 
 * @param carta Carta que se quiere imprimir
 */
void imprimir_carta_sola(s_Carta carta);

/**
 * @brief Muestra una descripcion de la carta
 * 
 * @param carta Carta a mostrar descripcion
 */
void mostrar_descripcion_carta(s_Carta carta);

/**
 * @brief Entrega el color ANSI asociado al tipo de carta.
 * @param tipo Tipo de carta.
 * @return Secuencia de color (COLOR_BLANCO si el tipo es desconocido).
 */
const char *color_tipo_carta(e_TipoCarta tipo);

#endif