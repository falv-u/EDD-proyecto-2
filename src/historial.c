#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "historial.h"

static char *copiar_cadena(const char *or)
{
	char *copia;
	unsigned int tam;
	if ( or == NULL )
	{
		fprintf(stderr, "error: cadena original NULL\n");
		return NULL;
	}
	tam = strlen(or) + 1;
	copia = malloc(tam);
	if (copia == NULL)
	{
		fprintf(stderr, "error: fallo en asignacion para copiar cadena de texto\n");
		return NULL;
	}
	strcpy(copia, or);
	return copia;

}
historial historial_make_empty(void)
{
	historial h = malloc(sizeof(struct s_historial));
	if (h == NULL)
	{
		fprintf(stderr, "error: falla al asignar memoria para la creacion del historial");
		return NULL;
	}
	h->nodo = NULL;
	h->cantidad_registros = 0;
	return h;
}

int historial_push(historial h, const char **desc, tipo_accion t )
{
	PosHistorial nuevo;
	char *copia_desc;
	if (h == NULL)
	{
		fprintf(stderr, "error: historial apunta a NULL\n");
		return -1;
	}
	if (desc == NULL)
	{
		fprintf(stderr, "error: cadena descripcion NULL\n");
		fprintf(stderr, "no quisiste poner '\0'???\n");
		return -1;
	}
	copia_desc = copiar_cadena(desc);

	if (copia_desc == NULL)
		return -1;

	nuevo = malloc(sizeof(struct s_NodoHistorial));
	if (nuevo == NULL) {
		fprintf(stderr, "error: sin memoria para nodo\n");
		free(copia_desc);
		return -1;
	}

	nuevo->datos.acc         = acc;
	nuevo->datos.descripcion = copia_desc;
	nuevo->datos.tipo        = t;
	nuevo->siguiente         = h->nodo;
	h->nodo = nuevo;
	h->cantidad_registros++;
	return 0;
}

int historial_pop(historial h)
{
	PosHistorial popear;
	if (h == NULL)
	{
		fprintf(stderr, "error: historial apunta a NULL\n");
		return -1;
	}
	if (h->nodo == NULL)
	{
		fprintf(stderr, "error: pop sobre historial vacio\n");
		return -1;
	}
	popear = h->nodo;
	h->nodo = popear->siguiente;
	h->cantidad_registros--;
	
	free(popear->datos.descripcion);
	free(popear);
	return 0;
}

PosHistorial historial_top(const historial h)
{
	if ( h == NULL )
	{
		fprintf(stderr, "error: historial invalido, apunta a NULL\n");
		return NULL;
	}
	if ( h->nodo == NULL)
	{
		fprintf(stderr, "error: historial invalido, el top es NULL\n");
		return NULL;
	}

 	return h->nodo;
}

PosHistorial historial_siguiente(const PosHistorial pos)
{
	if (pos == NULL)
	{
		fprintf(stderr, "error: posicion invalida, es NULL\n");
		return NULL;
	}
	return pos->siguiente;
}

int historial_is_empty(const historial h)
{
	if ( h == NULL)
	{
		fprintf(stderr, "error: historial NULL\n");
		return -1;
	}
	if (h->cantidad_registros == 0 && h->nodo == NULL)
        	return 1;   /* vacía */
	if (h.cantidad_registros != 0 || h.cantidad_registros < 0 && h->siguiente == NULL)
	{
		fprintf(stderr, "error: valores incongruentes, registros distintos de 0  pero apunta a NULL\n");
		return -1;
	}
	return 0;
}


void historial_destroy(historial h)
{
	PosHistorial actual, siguiente;
	if (h == NULL)
	{
		fprintf(stderr, "error: historia ya NULL\n");
		return;
	} 
	actual = h->nodo;
	while (actual != NULL)
	{
		siguiente = actual->siguiente;
		free(actual->datos.descripcion);
		free(actual);
		actual = siguiente;
	}
	h->nodo = NULL;
	h->cantidad_registros = 0;
}
