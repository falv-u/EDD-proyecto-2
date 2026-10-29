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
    const char *nombre;
    e_TipoCarta tipo;
    int valor;
} s_Carta;

/**
 * @brief Genera una carta aleatoria por probabilidad
 * 
 * @return Carta generada aleatoriamente.
 */
s_Carta carta_aleatoria();

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
#endif