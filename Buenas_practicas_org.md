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

```c
#include <stdlib.h>
#include <stdio.h>

#include "declaraciones.h"

#define MAX 67

void LimpiarPantalla(void)
{
	system(clear);
}

int main(void)
{
	/* codigo anterior */

	return 0;
}
```