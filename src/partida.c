/*
 * partida.c - guardar/cargar partida en CSV
 * Nota: strtok_r porque strtok es global y rompe el const.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "partida.h"

#define SEP ';'
#define PARTIDA_LINEA_MAX 1024
#define CARGA_MAX_ITEMS 1000

// copia un string a heap, devuelve NULL si falla
static char *duplicar_cadena(const char *s)
{
    char *c;

    if (s == NULL)
        return NULL;

    c = malloc(strlen(s) + 1);
    if (c == NULL)
        return NULL;

    strcpy(c, s);
    return c;
}

// nombre del tipo de accion para el archivo
static const char *tipo_a_texto(TipoAccion t)
{
    switch (t)
    {
        case A_CONSUMIBLE:  return "A_CONSUMIBLE";
        case A_REGISTRO:    return "A_REGISTRO";
        case A_COMBATE_I:   return "A_COMBATE_I";
        case A_COM_TURNO:   return "A_COM_TURNO";
        case A_FIN_COMBATE: return "A_FIN_COMBATE";
        case A_DESCARTE:    return "A_DESCARTE";
        default:            return "A_REGISTRO";
    }
}

// escribe la seccion PERSONAJE
static void guardar_personaje(FILE *f, const s_Personaje *pj)
{
    fprintf(f, "PERSONAJE%c%s%c%d%c%d%c%d%c%d\n",
            SEP, pj->nombre ? pj->nombre : "",
            SEP, pj->vida,
            SEP, pj->vidaMax,
            SEP, pj->ataque,
            SEP, pj->defensa);
}

// escribe la seccion INVENTARIO con sus cartas
static void guardar_inventario(FILE *f, const Lista inv)
{
    Posicion p;
    if (inv == NULL)
    {
        fprintf(f, "INVENTARIO%c0\n", SEP);
        return;
    }
    int cantidad = cantidad_cartas(inv);

    fprintf(f, "INVENTARIO%c%d\n", SEP, cantidad);

    for (p = inv->next; p != NULL; p = p->next)
    {
        fprintf(f, "CARTA%c%s%c%d%c%d%c%d\n",
                SEP, p->carta.nombre ? p->carta.nombre : "",
                SEP, (int)p->carta.tipo,
                SEP, p->carta.valor,
                SEP, p->carta.id);
    }
}

// recursa hasta el final y escribe al volver (mas viejo primero en archivo)
static void escribir_historial_rec(FILE *f, PosHistorial n)
{
    if (n == NULL)
        return;

    escribir_historial_rec(f, n->siguiente);

    fprintf(f, "ACCION%c%s%c%s\n",
            SEP, tipo_a_texto(n->datos.tipo),
            SEP, n->datos.descripcion ? n->datos.descripcion : "");
}

// escribe la seccion HISTORIAL
static void guardar_historial(FILE *f, const historial h)
{
    int contador;

    if (h == NULL)
    {
        fprintf(f, "HISTORIAL%c0\n", SEP);
        return;
    }

    contador = h->CantidadRegistros;
    fprintf(f, "HISTORIAL%c%d\n", SEP, contador);

    escribir_historial_rec(f, h->nodo);
}

int partida_guardar(const s_GameState *gs, const char *ruta)
{
    FILE *f;

    if (gs == NULL)
    {
        fprintf(stderr, "error: estado NULL\n");
        return -1;
    }

    if (ruta == NULL)
        ruta = PARTIDA_RUTA;

    f = fopen(ruta, "w");
    if (f == NULL)
    {
        fprintf(stderr, "error: no se pudo abrir '%s'\n", ruta);
        return -1;
    }

    guardar_personaje(f, &gs->personaje);
    guardar_inventario(f, gs->inventario);
    guardar_historial(f, gs->Historial);

    if (fclose(f) != 0)
    {
        fprintf(stderr, "error: fallo al cerrar\n");
        return -1;
    }

    return 0;
}

// lee una linea y le quita el \n
static int leer_linea(FILE *f, char *buf, int tam)
{
    if (fgets(buf, tam, f) == NULL)
        return 0;

    buf[strcspn(buf, "\n")] = '\0';
    return 1;
}

// lee seccion PERSONAJE, deja todo en pj si todo ok
static int cargar_personaje(FILE *f, s_Personaje *pj, char **nombre_salida)
{
    char linea[1024];
    char copia[PARTIDA_LINEA_MAX];
    char *saveptr;
    char *tok;
    char *nombre;

    if (!leer_linea(f, linea, sizeof(linea)))
    {
        fprintf(stderr, "error: falta PERSONAJE\n");
        return -1;
    }

    strncpy(copia, linea, sizeof(copia) - 1);
    copia[sizeof(copia) - 1] = '\0';

    tok = strtok_r(copia, ";", &saveptr);
    if (tok == NULL || strcmp(tok, "PERSONAJE") != 0)
    {
        fprintf(stderr, "error: formato invalido\n");
        return -1;
    }

    tok = strtok_r(NULL, ";", &saveptr);
    if (tok == NULL)
    {
        fprintf(stderr, "error: falta nombre\n");
        return -1;
    }

    nombre = duplicar_cadena(tok);
    if (nombre == NULL)
    {
        fprintf(stderr, "error: sin memoria\n");
        return -1;
    }

    tok = strtok_r(NULL, ";", &saveptr);
    if (tok == NULL)
    {
        fprintf(stderr, "error: falta vida\n");
        free(nombre);
        return -1;
    }
    pj->vida = atoi(tok);

    tok = strtok_r(NULL, ";", &saveptr);
    if (tok == NULL)
    {
        fprintf(stderr, "error: falta vidaMax\n");
        free(nombre);
        return -1;
    }
    pj->vidaMax = atoi(tok);

    if (pj->vida < 0 || pj->vidaMax <= 0 || pj->vida > pj->vidaMax)
    {
        fprintf(stderr, "error: vida invalida\n");
        free(nombre);
        return -1;
    }

    tok = strtok_r(NULL, ";", &saveptr);
    if (tok == NULL)
    {
        fprintf(stderr, "error: falta ataque\n");
        free(nombre);
        return -1;
    }
    pj->ataque = atoi(tok);

    tok = strtok_r(NULL, ";", &saveptr);
    if (tok == NULL)
    {
        fprintf(stderr, "error: falta defensa\n");
        free(nombre);
        return -1;
    }
    pj->defensa = atoi(tok);

    pj->nombre = nombre;
    *nombre_salida = nombre;
    return 0;
}

// lee seccion INVENTARIO, deja las cartas en *inv si todo ok
static int cargar_inventario(FILE *f, Lista *inv, char *nombre_personaje)
{
    char linea[PARTIDA_LINEA_MAX];
    char copia[PARTIDA_LINEA_MAX];
    char *saveptr;
    char *tok;
    int cantidad;
    int i;
    int max_id = 0;
    Posicion ultimo;

    if (!leer_linea(f, linea, sizeof(linea)))
    {
        fprintf(stderr, "error: falta INVENTARIO\n");
        free(nombre_personaje);
        return -1;
    }

    strncpy(copia, linea, sizeof(copia) - 1);
    copia[sizeof(copia) - 1] = '\0';

    tok = strtok_r(copia, ";", &saveptr);
    if (tok == NULL || strcmp(tok, "INVENTARIO") != 0)
    {
        fprintf(stderr, "error: formato invalido\n");
        free(nombre_personaje);
        return -1;
    }

    tok = strtok_r(NULL, ";", &saveptr);
    if (tok == NULL)
    {
        fprintf(stderr, "error: falta cantidad\n");
        free(nombre_personaje);
        return -1;
    }
    cantidad = atoi(tok);

    if (cantidad < 0 || cantidad > CARGA_MAX_ITEMS)
    {
        fprintf(stderr, "error: cantidad invalida\n");
        free(nombre_personaje);
        return -1;
    }

    // cargo las cartas aparte: si algo falla, gs no se toca
    *inv = crear_inventario(NULL);
    if (*inv == NULL)
    {
        fprintf(stderr, "error: sin memoria\n");
        free(nombre_personaje);
        return -1;
    }

    ultimo = *inv;

    for (i = 0; i < cantidad; i++)
    {
        char nombre_carta[256];
        s_Carta carta = {0};
        char *nom_tok;
        char *tipo_tok;
        char *valor_tok;
        char *id_tok;

        if (!leer_linea(f, linea, sizeof(linea)))
        {
            fprintf(stderr, "error: falta carta %d\n", i + 1);
            free(nombre_personaje);
            vaciar_inventario(*inv);
            free(*inv);
            *inv = NULL;
            return -1;
        }

        strncpy(copia, linea, sizeof(copia) - 1);
        copia[sizeof(copia) - 1] = '\0';

        tok = strtok_r(copia, ";", &saveptr);
        if (tok == NULL || strcmp(tok, "CARTA") != 0)
        {
            fprintf(stderr, "error: formato invalido\n");
            free(nombre_personaje);
            vaciar_inventario(*inv);
            free(*inv);
            *inv = NULL;
            return -1;
        }

        nom_tok   = strtok_r(NULL, ";", &saveptr);
        tipo_tok  = strtok_r(NULL, ";", &saveptr);
        valor_tok = strtok_r(NULL, ";", &saveptr);
        id_tok    = strtok_r(NULL, ";", &saveptr);

        if (nom_tok == NULL || tipo_tok == NULL ||
            valor_tok == NULL || id_tok == NULL)
        {
            fprintf(stderr, "error: carta incompleta\n");
            free(nombre_personaje);
            vaciar_inventario(*inv);
            free(*inv);
            *inv = NULL;
            return -1;
        }

        strncpy(nombre_carta, nom_tok, sizeof(nombre_carta) - 1);
        nombre_carta[sizeof(nombre_carta) - 1] = '\0';

        carta.nombre = nombre_carta;
        carta.tipo   = (e_TipoCarta)atoi(tipo_tok);
        carta.valor  = atoi(valor_tok);
        carta.id     = atoi(id_tok);

        if (carta.id <= 0)
        {
            fprintf(stderr, "error: id invalido\n");
            free(nombre_personaje);
            vaciar_inventario(*inv);
            free(*inv);
            *inv = NULL;
            return -1;
        }

        if (carta.id > max_id)
            max_id = carta.id;

        agregar_carta(*inv, carta, ultimo);
        ultimo = ultimo->next;

	if (ultimo == NULL)
	{
	    fprintf(stderr, "error: sin memoria al agregar carta\n");
	    free(nombre_personaje);
	    vaciar_inventario(*inv);
	    free(*inv);
	    *inv = NULL;
	    return -1;
	}
    }

    carta_fijar_ultimo_id(max_id);
    return 0;
}

// TODO: validar tipos
// convierte texto a enum, -1 si no reconoce
static int tipo_texto_a_enum(const char *texto)
{
    if (strcmp(texto, "A_CONSUMIBLE") == 0)  return A_CONSUMIBLE;
    if (strcmp(texto, "A_REGISTRO") == 0)    return A_REGISTRO;
    if (strcmp(texto, "A_COMBATE_I") == 0)   return A_COMBATE_I;
    if (strcmp(texto, "A_COM_TURNO") == 0)   return A_COM_TURNO;
    if (strcmp(texto, "A_FIN_COMBATE") == 0) return A_FIN_COMBATE;
    if (strcmp(texto, "A_DESCARTE") == 0)    return A_DESCARTE;

    return -1;
}

// lee seccion HISTORIAL, deja la pila en gs si todo ok
static int cargar_historial(FILE *f, s_GameState *gs, char *nombre_personaje)
{
    char linea[PARTIDA_LINEA_MAX];
    char copia[PARTIDA_LINEA_MAX];
    char *saveptr;
    char *tok;
    int cantidad;
    int i;
    historial hist;

    if (!leer_linea(f, linea, sizeof(linea)))
    {
        fprintf(stderr, "error: falta HISTORIAL\n");
        free(nombre_personaje);
        return -1;
    }

    strncpy(copia, linea, sizeof(copia) - 1);
    copia[sizeof(copia) - 1] = '\0';

    tok = strtok_r(copia, ";", &saveptr);
    if (tok == NULL || strcmp(tok, "HISTORIAL") != 0)
    {
        fprintf(stderr, "error: formato invalido\n");
        free(nombre_personaje);
        return -1;
    }

    tok = strtok_r(NULL, ";", &saveptr);
    if (tok == NULL)
    {
        fprintf(stderr, "error: falta cantidad\n");
        free(nombre_personaje);
        return -1;
    }
    cantidad = atoi(tok);

    if (cantidad < 0 || cantidad > CARGA_MAX_ITEMS)
    {
        fprintf(stderr, "error: cantidad invalida\n");
        free(nombre_personaje);
        return -1;
    }

    // armo el historial aparte igual que el inventario
    hist = historial_make_empty();
    if (hist == NULL)
    {
        fprintf(stderr, "error: sin memoria\n");
        free(nombre_personaje);
        return -1;
    }

    for (i = 0; i < cantidad; i++)
    {
        char tipo_txt[64];
        char desc[512];
        int t;
        char *tipo_tok;
        char *desc_tok;

        if (!leer_linea(f, linea, sizeof(linea)))
        {
            fprintf(stderr, "error: falta accion %d\n", i + 1);
            free(nombre_personaje);
            historial_free(&hist);
            return -1;
        }

        strncpy(copia, linea, sizeof(copia) - 1);
        copia[sizeof(copia) - 1] = '\0';

        tok = strtok_r(copia, ";", &saveptr);
        if (tok == NULL || strcmp(tok, "ACCION") != 0)
        {
            fprintf(stderr, "error: formato invalido\n");
            free(nombre_personaje);
            historial_free(&hist);
            return -1;
        }

        tipo_tok = strtok_r(NULL, ";", &saveptr);
        desc_tok = strtok_r(NULL, ";", &saveptr);

        if (tipo_tok == NULL || desc_tok == NULL)
        {
            fprintf(stderr, "error: accion incompleta\n");
            free(nombre_personaje);
            historial_free(&hist);
            return -1;
        }

        strncpy(tipo_txt, tipo_tok, sizeof(tipo_txt) - 1);
        tipo_txt[sizeof(tipo_txt) - 1] = '\0';

        strncpy(desc, desc_tok, sizeof(desc) - 1);
        desc[sizeof(desc) - 1] = '\0';

        t = tipo_texto_a_enum(tipo_txt);
        if (t < 0)
        {
            fprintf(stderr, "aviso: tipo desconocido, se omite\n");
            continue;
        }

        if (historial_push(hist, desc, (TipoAccion)t) != 0)
        {
            fprintf(stderr, "error: fallo push\n");
            free(nombre_personaje);
            historial_free(&hist);
            return -1;
        }
    }

    historial_free(&gs->Historial);
    gs->Historial = hist;
    return 0;
}

/**
 * @brief Carga el estado del juego desde un archivo CSV.
 * @author [NOMBRE]
 */
int partida_cargar(s_GameState *gs, const char *ruta)
{
    FILE *f;
    s_Personaje pj = {0};
    char *nombre_personaje = NULL;
    Lista inv = NULL;

    if (gs == NULL)
    {
        fprintf(stderr, "error: estado NULL\n");
        return -1;
    }

    if (ruta == NULL)
        ruta = PARTIDA_RUTA;

    f = fopen(ruta, "r");
    if (f == NULL)
    {
        fprintf(stderr, "error: no se pudo abrir '%s'\n", ruta);
        return -1;
    }

    if (cargar_personaje(f, &pj, &nombre_personaje) != 0)
    {
        fclose(f);
        return -1;
    }

    if (cargar_inventario(f, &inv, nombre_personaje) != 0)
    {
        fclose(f);
        return -1;
    }

    if (cargar_historial(f, gs, nombre_personaje) != 0)
    {
        fclose(f);
        vaciar_inventario(inv);
        free(inv);
        return -1;
    }

    fclose(f);

    if (gs->inventario != NULL)
    {
        vaciar_inventario(gs->inventario);
        free(gs->inventario);
    }
    gs->inventario = inv;

    if (gs->personaje.nombre != NULL)
    {
        char *tmp = (char *)gs->personaje.nombre;
        free(tmp);
    }

    gs->personaje = pj;
    gs->ejecutando = true;

    if (gs->Historial != NULL)
        gs->Historial->HayReversionPendiente = false;

    return 0;
}