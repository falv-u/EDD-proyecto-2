## Buenas Practicas

### Convención de nombres
- **Funciones:** snake_case
- **Variables:** camelCase
- **Macros:** siempre en mayusculas  
- **Estructuras**: PascalCase con prefijo s_
- **Enums**: PascalCase con prefijo e_

### estilo de codigo

el orden del codigo es:
1. **includes de la libc** usando corchetes angulares (<>).
2. **includes propios** usando comillas dobles ("").
3. **macros/defines**

Este orden refleja la jerarquía de dependencias (de lo mas externo a lo mas interno)
y facilita detectar de un vistazo que necesita el archivo del sistema y que del propio proyecto.

para el proyecto se usara el estilo de llaves allman.

### uso de memoria dinamica y descriptores

Por cada malloc o descriptor de archivo abierto se liberara la memoria o cerrara el descriptor
segun se requiera sea al final de la funcion o en una funcion auxiliar dedicada a eso.

### codigo fuente

El codigo fuente es diferenciado en varios archivos de la forma "un archivo, un proposito",
proporcionando modularidad al proyecto y teniendo nombres cortos en español que describan
de forma muy generalizada su proposito, por ejemplo '_interfaz.c_' con su respectivo '_interfaz.h_'.

### arbol basico del proyecto
```sh
.
├── .gitignore
├── Buenas_practicas_org.md
├── Makefile
├── README.md
├── Otros-archivos-relacionados
├── docs
├── include
│   └── *.h
└── src
    └── *.c


```
_ejemplo sencillo:_
```c
#include <stdlib.h>
#include <stdio.h>

#include "declaraciones.h"

#define MAX 67

void limpiar_pantalla(void)
{
	system(clear);
}

int main(void)
{
	codigoImprimir[MAX];
	/*...*/
	for (int i = 0; i < MAX; i++)
	{
		printf("%s\n", codigoImprimir[i]);
	}
	limpiar_pantalla();
	return 0;
}
```