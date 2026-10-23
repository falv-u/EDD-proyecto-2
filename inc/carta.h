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

s_Carta carta_aleatoria();

#endif