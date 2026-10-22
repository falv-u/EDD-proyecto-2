#ifndef COMUNES_H
#define COMUNES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int leer_entero();
int leer_entero_en_rango(int min, int max);
void validar_rango(int *num, int min, int max);
void imprimir_ingresar_opcion();

#endif