#ifndef ENEMIGO_H
#define ENEMIGO_H
 
typedef struct
{
    int id;             /* numero de aparicion: define el orden de combate */
    const char *nombre;
    int vida;
    int vidaMax;
    int ataque;
    int defensa;
} s_Enemigo;
 
/**
 * @brief Crea un enemigo aleatorio a partir de las plantillas disponibles.
 * @param id Numero de aparicion (1, 2, 3...) dentro del grupo.
 * @return Enemigo con vida completa.
 */
s_Enemigo enemigo_aleatorio(int id);
 
#endif
 