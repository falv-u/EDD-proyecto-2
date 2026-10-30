#ifndef COMUNES_H
#define COMUNES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Lee un número entero ingresado por el usuario y valida la entrada.
 * 
 * @return int Numero entero ingresado por el usuario
 */
int leer_entero();

/**
 * @brief Lee un número entero y comprueba que se este dentro del rango establecido.
 * 
 * @param min Valor minimo permitido.
 * @param max Valor maximo permitido
 * @return Numero entero dentro del rango establecido
 */
int leer_entero_en_rango(int min, int max);

/**
 * @brief Valida que el numero ingresado este en el rango establecido, se repite hasta que se cumplan las condiciones
 * 
 * @param num Puntero al numero a validar el rango
 * @param min Valor minimo permitido
 * @param max Valor maximo permitido
 */
void validar_rango(int *num, int min, int max);

/**
 * @brief Solicita una respuesta de sí o no y devuelve su valor.
 * 
 * @return true Si responde si
 * @return false Si responde no
 */
bool leer_si_no(void);


void imprimir_ingresar_opcion();
#endif