#ifndef COLA_H
#define COLA_H
 
#include <stdbool.h>
 
#include "enemigo.h"
 
#define MAX_ENEMIGOS 5
 
/**
 * @file cola.h
 * @brief TDA Cola (FIFO) implementada con arreglo circular.
 *
 * Se usa para el orden de combate: el enemigo que aparece primero
 * es el primero en ser enfrentado. 
 */
typedef struct s_Cola *Cola;
 
/**
 * @brief Crea una cola vacia con la capacidad indicada.
 * @param capacidad Maximo de elementos (>= 1).
 * @return Cola nueva, o NULL si la capacidad es invalida o no hay memoria.
 */
Cola cola_crear(int capacidad);
 
/** @brief Deja la cola vacia (makeEmpty). No libera memoria. */
void cola_vaciar(Cola c);
 
/**
 * @brief Inserta un enemigo al final de la cola (enqueue).
 * @return 0 si ok, -1 si la cola es NULL o esta llena.
 */
int cola_encolar(Cola c, s_Enemigo enemigo);
 
/**
 * @brief Elimina el enemigo del frente (dequeue).
 * @return 0 si ok, -1 si la cola es NULL o esta vacia.
 */
int cola_desencolar(Cola c);
 
/**
 * @brief Entrega el enemigo del frente  (front).
 * @return Puntero al elemento (para actualizar su vida en combate),
 *         o NULL si la cola es NULL o esta vacia.
 */
s_Enemigo *cola_frente(const Cola c);
 
/** @brief true si la cola esta vacia (o es NULL). */
bool cola_esta_vacia(const Cola c);
 
/** @brief true si la cola esta llena. */
bool cola_esta_llena(const Cola c);
 
/** @brief Cantidad de elementos en la cola (0 si es NULL). */
int cola_tamanho(const Cola c);
 
/** @brief Libera la cola completa. El llamador debe poner su puntero en NULL. */
void cola_destruir(Cola c);
 
/**
 * @brief Avanza un indice de forma circular (vuelve a 0 al llegar a la capacidad).
 * @param c Cola (no NULL).
 * @param valor Indice actual.
 * @return Siguiente indice dentro de [0, capacidad).
 */
int cola_siguiente_indice(const Cola c, int valor);
 
#endif