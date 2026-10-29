#ifndef PERSONAJE_H
#define PERSONAJE_H

typedef struct s_GameState s_GameState;

typedef struct 
{
    int vida;
    int ataque;
    int defensa;
    int vidaMax;
    const char *nombre;
} s_Personaje;

/**
 * @brief Inicializa un personaje con sus estadísticas y reserva memoria para su nombre.
 * 
 * @param pj Puntero al personaje a crear
 * @param vida Vida del personaje
 * @param vidaMax Vida maxima del personaje
 * @param ataque Puntos de ataque del personaje
 * @param defensa Puntos de defensa del personaje
 * @param nombre Nombre del personaje
 */
void crear_personaje(s_Personaje *pj, int vida, int vidaMax, int ataque, int defensa, const char *nombre);

/**
 * @brief Permite elegir un personaje con estadisticas unicas
 * 
 * @param pj Puntero al personaje a asignar 
 */
void asignar_personaje(s_Personaje *pj);

/**
 * @brief Muestra el nombre y las estadísticas del personaje.
 * 
 * @param pj Puntero al personaje a mostrar la informacion
 */
void imprimir_personaje(s_Personaje *pj);

#endif