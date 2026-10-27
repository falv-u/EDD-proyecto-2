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
	h->CantidadRegistros = 0;
	h->HayReversionPendiente = false;

	memset(&h->reversion, 0, sizeof(h->reversion));
	return h;
}

int historial_push(historial h, const char *desc, TipoAccion t )
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
		fprintf(stderr, "no quisiste poner ''???\n");
		return -1;
	}
	copia_desc = copiar_cadena(desc);

	if (copia_desc == NULL)
		return -1;

	nuevo = malloc(sizeof(struct s_NodoHistorial));
	if (nuevo == NULL)
	{
		fprintf(stderr, "error: sin memoria para nodo\n");
		free(copia_desc);
		return -1;
	}

	nuevo->datos.descripcion = copia_desc;
	nuevo->datos.tipo        = t;
	nuevo->siguiente         = h->nodo;
	h->nodo = nuevo;
	h->CantidadRegistros++;
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
	h->CantidadRegistros--;
	h->HayReversionPendiente = false;

	h->reversion = (s_Reversion){0};
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
	if (h == NULL)
	{
		fprintf(stderr, "error: historial NULL\n");
		return -1;
	}

	if (h->CantidadRegistros == 0 && h->nodo == NULL)
	{
		return 1;
	}

	if (h->CantidadRegistros > 0 && h->nodo != NULL)
	{
		return 0;
	}

	fprintf(stderr, "error: estado incongruente (registros=%d, nodo=%p)\n",
	        h->CantidadRegistros, (void *)h->nodo);
	return -1;
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
	h->HayReversionPendiente = false;
	h->nodo = NULL;
	h->CantidadRegistros = 0;
}
int historial_registrar(historial h, const char *desc, TipoAccion t, const s_Reversion *rev)
{
	if (h == NULL)
	{
		fprintf(stderr, "error: historial apunta a NULL\n");
		return -1;
	}

	if (desc == NULL)
	{
		fprintf(stderr, "error: descripcion NULL\n");
		return -1;
	}


	if (historial_push(h, desc, t) != 0)
	{
		fprintf(stderr, "push fallo");
		return -1;
	}

	/* Cualquier accion nueva invalida la ventana de reversion anterior. */
	h->HayReversionPendiente = false;

	if (t == A_DESCARTE && rev != NULL)
	{
		h->reversion = *rev;
		h->HayReversionPendiente = true;
	}

	return 0;
}

bool historial_puede_deshacer(const historial h)
{
	if (h == NULL)
	{
		return false;
	}

	if (!h->HayReversionPendiente)
	{
		return false;
	}

	if (h->nodo == NULL || h->nodo->datos.tipo != A_DESCARTE)
	{
		return false;
	}

	return true;
}

int historial_obtener_reversion(const historial h, s_Reversion *out)
{
	if (out == NULL)
	{
		fprintf(stderr, "error: destino de reversion NULL\n");
		return -1;
	}

	if (!historial_puede_deshacer(h))
	{
		return -1;
	}

	*out = h->reversion;
	return 0;
}

int historial_deshacer(historial h)
{
	if (!historial_puede_deshacer(h))
	{
		return -1;
	}

	return historial_pop(h);
}

static const char *tipo_accion_a_string(TipoAccion t)
{
	switch (t)
	{
		case A_CONSUMIBLE: 
			return "CONSUMIBLE";
		case A_REGISTRO:
			return "REGISTRO";
		case A_COMBATE_I:
			 return "COMBATE_INICIO";
		case A_COM_TURNO:
			 return "COMBATE_TURNO";
		case A_FIN_COMBATE:
			 return "FIN_COMBATE";
		case A_DESCARTE:
			 return "DESCARTE";
		default:
			 return "DESCONOCIDO";
	}
}

void historial_imprimir(const historial h)
{
	PosHistorial actual;
	int i;

	if (h == NULL)
	{
		fprintf(stderr, "error: historial NULL\n");
		return;
	}

	if (h->nodo == NULL)
	{
		printf("Historial vacio\n");
		return;
	}

	printf("=== HISTORIAL (%d registros) ===\n", h->CantidadRegistros);
	actual = h->nodo;
	i = 1;
	while (actual != NULL)
	{
		printf("%d. [%s] %s\n", i, tipo_accion_a_string(actual->datos.tipo), actual->datos.descripcion);
		actual = actual->siguiente;
		i++;
	}
	printf("================================\n");
}
