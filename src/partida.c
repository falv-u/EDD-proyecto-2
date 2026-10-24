#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "partida.h"

#define SEP ';'


/**
 * @brief Copia una cadena a memoria nueva.
 * @return Copia con malloc, o NULL si falla.
 */
static char *duplicar(const char *or)
{
	char *copia;
	size_t n;

	if (or == NULL)
	{
		return NULL;
	}

	n = strlen(or) + 1;          /* +1 para el '\0' */
	copia = malloc(n);
	if (copia == NULL)
	{
		return NULL;
	}

	memcpy(copia, or, n);        /* memcpy en vez de strcpy: misma idea */
	return copia;
}

/**
 * @brief Lee una linea y le quita el '\n' final.
 * @return 0 ok, -1 EOF o linea truncada.
 */
static int leer_linea(FILE *f, char *buf, size_t tam)
{
	size_t n;

	if (fgets(buf, (int)tam, f) == NULL)
	{
		return -1;
	}

	n = strlen(buf);
	if (n == 0 || buf[n - 1] != '\n')
	{
		return -1;
	}

	buf[n - 1] = '\0';           /* recorta el '\n' */
	return 0;
}

/**
 * @brief Saca el siguiente campo de una linea separada por SEP.
 *
 * Modifica la cadena: cambia el ';' por '\0'. Avanza *cursor.
 * Devuelve NULL cuando ya no quedan campos.
 */
static char *campo(char **cursor)
{
	char *ini;
	char *p;

	if (cursor == NULL || *cursor == NULL)
	{
		return NULL;
	}

	ini = *cursor;
	p = strchr(ini, SEP);        /* busca el proximo ';' */

	if (p == NULL)
	{
		*cursor = NULL;
	}
	else
	{
		*p = '\0';
		*cursor = p + 1;
	}

	return ini;
}

/**
 * @brief Nombre textual de un TipoAccion (para guardar).
 */
static const char *tipo_a_texto(TipoAccion t)
{
	switch (t)
	{
		case A_CONSUMIBLE:
			 return "A_CONSUMIBLE";
		case A_REGISTRO:
			 return "A_REGISTRO";
		case A_COMBATE_I:
			 return "A_COMBATE_I";
		case A_COM_TURNO:
			 return "A_COM_TURNO";
		case A_FIN_COMBATE:
			 return "A_FIN_COMBATE";
		case A_DESCARTE:
			 return "A_DESCARTE";
		default:
			 return "A_REGISTRO";
	}
}

/**
 * @brief Convierte texto a TipoAccion.
 * @return TipoAccion o -1 si no coincide.
 */
static int texto_a_tipo(const char *s)
{
	if (s == NULL) 
		return -1;

	if (strcmp(s, "A_CONSUMIBLE")  == 0)
		return A_CONSUMIBLE;
	if (strcmp(s, "A_REGISTRO")    == 0)
		return A_REGISTRO;
	if (strcmp(s, "A_COMBATE_I")   == 0)
		return A_COMBATE_I;
	if (strcmp(s, "A_COM_TURNO")   == 0)
		return A_COM_TURNO;
	if (strcmp(s, "A_FIN_COMBATE") == 0)
		return A_FIN_COMBATE;
	if (strcmp(s, "A_DESCARTE")    == 0)
		return A_DESCARTE;

	return -1;
}


int partida_guardar(const s_GameState *gs, const char *ruta)
{
	FILE *f;
	Posicion p;
	PosHistorial h;
	int cantidad;

	if (gs == NULL)
	{
		return -1;
	}

	if (ruta == NULL)
	{
		ruta = PARTIDA_RUTA;
	}

	f = fopen(ruta, "w");
	if (f == NULL)
	{
		fprintf(stderr, "error: no se pudo abrir '%s'\n", ruta);
		return -1;
	}

	/* Personaje */
	fprintf(f, "PERSONAJE%c%s%c%d%c%d%c%d%c%d\n",
	        SEP, gs->personaje.nombre ? gs->personaje.nombre : "",
	        SEP, gs->personaje.vida,
	        SEP, gs->personaje.vidaMax,
	        SEP, gs->personaje.ataque,
	        SEP, gs->personaje.defensa);

	/* Inventario */
	cantidad = cantidad_cartas(gs->inventario);
	fprintf(f, "INVENTARIO%c%d\n", SEP, cantidad);

	for (p = gs->inventario->next; p != NULL; p = p->next)
	{
		fprintf(f, "CARTA%c%s%c%d%c%d\n",
		        SEP, p->carta.nombre ? p->carta.nombre : "",
		        SEP, (int)p->carta.tipo,
		        SEP, p->carta.valor);
	}

	/* Historial */
	if (gs->Historial == NULL)
	{
		fprintf(f, "HISTORIAL%c0\n", SEP);
	}
	else
	{
		fprintf(f, "HISTORIAL%c%d\n", SEP, gs->Historial->CantidadRegistros);

		for (h = gs->Historial->nodo; h != NULL; h = h->siguiente)
		{
			fprintf(f, "ACCION%c%s%c%s\n",
			        SEP, tipo_a_texto(h->datos.tipo),
			        SEP, h->datos.descripcion ? h->datos.descripcion : "");
		}
	}

	if (fclose(f) != 0)
	{
		return -1;
	}

	return 0;
}

int partida_cargar(s_GameState *gs, const char *ruta)
{
	FILE *f;
	char linea[PARTIDA_LINEA_MAX];
	char *tok;               /* token actual de strtok */
	int i;
	int cantidad;            /* cuantas cartas / acciones leer */
	s_Personaje pj;          /* personaje temporal: solo se aplica si todo ok */
	char *nombre;

	if (gs == NULL || ruta == NULL)
	{
		return -1;
	}

	f = fopen(ruta, "r");
	if (f == NULL)
	{
		return -1;
	}

	/* fgets lee UNA linea (incluye '\n' si cabe). Si falla, EOF. */
	if (fgets(linea, sizeof(linea), f) == NULL)
	{
		fclose(f);
		return -1;
	}

	/*
	 * strcspn(linea, "\n") devuelve la posicion del '\n'.
	 * Al asignarle '\0' ahi, recortamos el salto de linea.
	 */
	linea[strcspn(linea, "\n")] = '\0';

	/*
	 * strtok(linea, ";") devuelve el primer token separado por ';'.
	 * Tambien escribe '\0' donde estaba el ';', asi que linea queda
	 * partida en trozos. Para seguir hay que llamar strtok(NULL, ";").
	 */
	tok = strtok(linea, ";");

	/* El primer token debe ser literalmente "PERSONAJE". */
	if (tok == NULL || strcmp(tok, "PERSONAJE") != 0)
	{
		fclose(f);
		fprintf("error: error de formato en la partida guardada\n");
		return -1;
	}

	/* Siguiente token: el nombre. Lo copiamos a memoria nueva porque
	   el buffer "linea" se va a reutilizar en la siguiente lectura. */
	tok = strtok(NULL, ";");
	if (tok == NULL)
	{
		fclose(f);
		return -1;
	}
	nombre = strdup(tok);           /* reserva + copia */
	if (nombre == NULL)
	{
		fclose(f);
		return -1;
	}

	/*
	 * Los 4 enteros: vida, vidaMax, ataque, defensa.
	 * atoi convierte "123" -> 123. Si el token no es numero, 
	 */
	tok = strtok(NULL, ";");
	if (tok == NULL) { free(nombre); fclose(f); return -1; }
	pj.vida = atoi(tok);

	tok = strtok(NULL, ";");
	if (tok == NULL) { free(nombre); fclose(f); return -1; }
	pj.vidaMax = atoi(tok);

	tok = strtok(NULL, ";");
	if (tok == NULL) { free(nombre); fclose(f); return -1; }
	pj.ataque = atoi(tok);

	tok = strtok(NULL, ";");
	if (tok == NULL) { free(nombre); fclose(f); return -1; }
	pj.defensa = atoi(tok);

	pj.nombre = nombre;

	if (fgets(linea, sizeof(linea), f) == NULL)
	{
		free(nombre);
		fclose(f);
		return -1;
	}

	linea[strcspn(linea, "\n")] = '\0';

	tok = strtok(linea, ";");
	if (tok == NULL || strcmp(tok, "INVENTARIO") != 0)
	{
		free(nombre);
		fclose(f);
		return -1;
	}

	/* Segundo token: cuantas cartas hay. */
	tok = strtok(NULL, ";");
	cantidad = (tok != NULL) ? atoi(tok) : 0;

	/* Empezamos con el inventario limpio: liberamos los nodos actuales
	   pero conservamos la cabecera (vaciar_inventario hace eso). */
	vaciar_inventario(gs->inventario);

	for (i = 0; i < cantidad; i++)
	{
		char nombre_carta[256];  /* copia local del nombre de la carta */
		s_Carta carta;           /* la que devuelva la busqueda */
		char *nom_tok;
		char *tipo_tok;
		char *valor_tok;

		/* Leemos la linea "CARTA;..." */
		if (fgets(linea, sizeof(linea), f) == NULL)
		{
			free(nombre);
			fclose(f);
			return -1;
		}
		linea[strcspn(linea, "\n")] = '\0';

		tok = strtok(linea, ";");
		if (tok == NULL || strcmp(tok, "CARTA") != 0)
		{
			free(nombre);
			fclose(f);
			return -1;
		}

		/* Los 3 campos siguientes, todos a la vez, para validarlos
		   juntos de un vistazo. */
		nom_tok   = strtok(NULL, ";");
		tipo_tok  = strtok(NULL, ";");
		valor_tok = strtok(NULL, ";");

		if (nom_tok == NULL || tipo_tok == NULL || valor_tok == NULL)
		{
			free(nombre);
			fclose(f);
			return -1;
		}

		/* Copiamos el nombre a un buffer fijo. strncpy no garantiza '\0'
		   si el origen es mas largo que el destino, por eso forzamos el
		   ultimo byte a '\0' a mano. */
		strncpy(nombre_carta, nom_tok, sizeof(nombre_carta) - 1);
		nombre_carta[sizeof(nombre_carta) - 1] = '\0';

		/* Buscamos la carta por nombre. Asi reconstruimos el s_Carta
		   con su puntero a literal original (no guardamos punteros). */
		carta = carta_buscar_por_nombre(nombre_carta);
		if (carta.nombre == NULL)
		{
			fprintf(stderr, "aviso: carta desconocida '%s'\n", nombre_carta);
			continue;   /* no abortamos: saltamos esta carta */
		}

		agregar_carta(gs->inventario, carta, gs->inventario);
	}


	if (fgets(linea, sizeof(linea), f) == NULL)
	{
		free(nombre);
		fclose(f);
		return -1;
	}
	linea[strcspn(linea, "\n")] = '\0';

	tok = strtok(linea, ";");
	if (tok == NULL || strcmp(tok, "HISTORIAL") != 0)
	{
		free(nombre);
		fclose(f);
		return -1;
	}

	tok = strtok(NULL, ";");
	cantidad = (tok != NULL) ? atoi(tok) : 0;

	/* Si ya habia un historial, lo destruimos (libera nodos y
	   descripciones). Luego creamos uno vacio o lo dejamos como esta. */
	if (gs->Historial == NULL)
	{
		gs->Historial = historial_make_empty();
	}
	else
	{
		historial_destroy(gs->Historial);
	}
	if (gs->Historial == NULL)
	{
		free(nombre);
		fclose(f);
		return -1;
	}

	for (i = 0; i < cantidad; i++)
	{
		char tipo_txt[64];
		char desc[512];
		int t;
		char *tipo_tok;
		char *desc_tok;

		if (fgets(linea, sizeof(linea), f) == NULL)
		{
			free(nombre);
			fclose(f);
			return -1;
		}
		linea[strcspn(linea, "\n")] = '\0';

		tok = strtok(linea, ";");
		if (tok == NULL || strcmp(tok, "ACCION") != 0)
		{
			free(nombre);
			fclose(f);
			return -1;
		}

		tipo_tok = strtok(NULL, ";");
		desc_tok = strtok(NULL, ";");

		if (tipo_tok == NULL || desc_tok == NULL)
		{
			free(nombre);
			fclose(f);
			return -1;
		}

		strncpy(tipo_txt, tipo_tok, sizeof(tipo_txt) - 1);
		tipo_txt[sizeof(tipo_txt) - 1] = '\0';

		strncpy(desc, desc_tok, sizeof(desc) - 1);
		desc[sizeof(desc) - 1] = '\0';

		/* Mapeo texto -> enum, inline para no saltar a otra funcion. */
		t = -1;
		if (strcmp(tipo_txt, "A_CONSUMIBLE")  == 0)
			t = A_CONSUMIBLE;

		if (strcmp(tipo_txt, "A_REGISTRO")    == 0)
			t = A_REGISTRO;

		if (strcmp(tipo_txt, "A_COMBATE_I")   == 0)
			t = A_COMBATE_I;

		if (strcmp(tipo_txt, "A_COM_TURNO")   == 0)
			t = A_COM_TURNO;

		if (strcmp(tipo_txt, "A_FIN_COMBATE") == 0)
			t = A_FIN_COMBATE;

		if (strcmp(tipo_txt, "A_DESCARTE")    == 0)
			t = A_DESCARTE;

		if (t < 0)
		{
			fprintf(stderr, "aviso: accion desconocida '%s'\n", tipo_txt);
			continue;   /* no abortamos: saltamos esta accion */
		}

		historial_push(gs->Historial, desc, (TipoAccion)t);
	}

	fclose(f);

	/* 
	 * APLICAR AL ESTADO REAL
	 * Solo llegamos aqui si TODO se leyo bien. Asi, si algo fallo
	 * antes, gs->personaje sigue intacto y el llamador puede
	 * arrancar una partida nueva sin datos a medias.
	 */
	gs->personaje = pj;
	gs->ejecutando = true;
	gs->Historial->HayReversionPendiente = false;  /* partida cargada: sin undo */

	return 0;
}
