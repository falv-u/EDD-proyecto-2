#ifndef PARTIDA_H
#define PARTIDA_H

#include "juego.h"

#define PARTIDA_RUTA "partida.csv"

/**
 * @file partida.h
 * @brief Guardado y carga de partida en formato CSV.
 *
 * Formato:
 *   linea 1: PERSONAJE,nombre,vida,vidaMax,ataque,defensa
 *   linea 2: INVENTARIO,cantidad
 *   lineas siguientes: carta,nombre,tipo,valor   (una por carta)
 *   linea siguiente: HISTORIAL,cantidad
 *   lineas siguientes: accion,tipo,descripcion   (una por accion)
 *
 *
 * @author Francisco Alvarado
 */

/**
 * @brief Guarda el estado del juego en un archivo CSV.
 *
 * @param gs    Estado del juego (no NULL).
 * @param ruta  Ruta del archivo (si NULL, usa PARTIDA_RUTA).
 * @return 0 si todo va bien, -1 si hay error de apertura o escritura.
 */
int partida_guardar(const s_GameState *gs, const char *ruta);

/**
 * @brief Carga el estado del juego desde un archivo CSV.
 *
 * Si el archivo no existe o esta corrupto, deja gs sin modificar y
 * devuelve -1. El llamador decide si arrancar una partida nueva.
 *
 * @param gs    Estado del juego destino (no NULL).
 * @param ruta  Ruta del archivo (si NULL, usa PARTIDA_RUTA).
 * @return 0 si todo va bien, -1 en caso de error.
 */
int partida_cargar(s_GameState *gs, const char *ruta);

#endif
