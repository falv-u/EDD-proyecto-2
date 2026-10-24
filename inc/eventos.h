#ifndef EVENTOS_H
#define EVENTOS_H

typedef enum
{
    NADA,
    COMBATE, // ESTADO PELEANDO
    LOOTEO, // ENCONTRANDO ITEM
    DESGRACIA // QUE PASE ALGO MALO
} e_TipoEvento;

/**
 * @brief Funcion para definir el evento que sucederá. 1 == COMBATE , 2 == LOOTEO, 3 == DESGRACIA, 0 == NADA
 * 
 * @return retorna 1 , 2 , 3 o 0
 */
int definir_evento();

#endif