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
 * @brief Funcion para definir el evento que sucederá.
 *
 * @return COMBATE, LOOTEO, DESGRACIA o NADA
 */
e_TipoEvento definir_evento(void);
/**
 * @brief   Ejecuta los eventos llamando a las funciones correspondientes
 * 
 * @param gs 
 * @param evento Tipo de evento
 * 
 * @return false si
 */
int ejecutar_evento(s_GameState *gs, e_TipoEvento evento);

/**
 * @brief Se genera una desgracia aleatoria al jugador, como perder una cierta cantidad de vida,
 * además se comprueba si despues de recibir el danho el juego continua o no,
 * 
 * @param gs 
 */
void evento_desgracia(s_GameState *gs);

/**
 * @brief Se genera una carta aleatoria y se pida si se queire anhadir al inventario o no.
 * 
 * @param gs 
 */
void evento_looteo(s_GameState *gs);
#endif