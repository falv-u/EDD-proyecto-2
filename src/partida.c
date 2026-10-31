/* Necesario antes de los includes para tener strdup() incluso con -std=c99 */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "partida.h"

#define SEP ';'
#define PARTIDA_LINEA_MAX 1024

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

/* Funciones auxiliares para partida_guardar */

/**
 * @brief Escribe la seccion PERSONAJE al archivo.
 */
static void guardar_personaje(FILE *f, const s_Personaje *pj)
{
    fprintf(f, "PERSONAJE%c%s%c%d%c%d%c%d%c%d\n",
            SEP, pj->nombre ? pj->nombre : "",
            SEP, pj->vida,
            SEP, pj->vidaMax,
            SEP, pj->ataque,
            SEP, pj->defensa);
}

/**
 * @brief Escribe la seccion INVENTARIO al archivo.
 */
static void guardar_inventario(FILE *f, const Lista inv)
{
    Posicion p;
    int cantidad = cantidad_cartas(inv);

    fprintf(f, "INVENTARIO%c%d\n", SEP, cantidad);

    for (p = inv->next; p != NULL; p = p->next)
    {
        fprintf(f, "CARTA%c%s%c%d%c%d\n",
                SEP, p->carta.nombre ? p->carta.nombre : "",
                SEP, (int)p->carta.tipo,
                SEP, p->carta.valor);
    }
}

/**
 * @brief Escribe la seccion HISTORIAL al archivo.
 * Guarda las acciones de mas viejo a mas nuevo para que al cargar
 * con push la pila quede con el mas nuevo en el tope.
 */
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

    if (contador > 0)
    {
        /* Reservar array temporal de punteros a nodos para escribir
         * el historial en orden inverso (mas viejo primero en archivo) */
        PosHistorial *nodos = malloc(contador * sizeof(PosHistorial));
        if (nodos)
        {
            PosHistorial actual;
            int idx = 0;

            for (actual = h->nodo; actual != NULL; actual = actual->siguiente)
                nodos[idx++] = actual;

            for (int i = contador - 1; i >= 0; i--)
            {
                PosHistorial nh = nodos[i];
                fprintf(f, "ACCION%c%s%c%s\n",
                        SEP, tipo_a_texto(nh->datos.tipo),
                        SEP, nh->datos.descripcion ? nh->datos.descripcion : "");
            }
            free(nodos);
        }
        else
        {
            /* Respaldo: escribir en orden natural si falla malloc */
            PosHistorial actual;
            for (actual = h->nodo; actual != NULL; actual = actual->siguiente)
            {
                fprintf(f, "ACCION%c%s%c%s\n",
                        SEP, tipo_a_texto(actual->datos.tipo),
                        SEP, actual->datos.descripcion ? actual->datos.descripcion : "");
            }
        }
    }
}

int partida_guardar(const s_GameState *gs, const char *ruta)
{
    FILE *f;

    if (gs == NULL)
    {
        fprintf(stderr, "error: estado de juego invalido (NULL)\n");
        return -1;
    }

    if (ruta == NULL)
    {
        ruta = PARTIDA_RUTA;
    }

    f = fopen(ruta, "w");
    if (f == NULL)
    {
        fprintf(stderr, "error: no se pudo abrir archivo '%s' para escritura\n", ruta);
        return -1;
    }

    guardar_personaje(f, &gs->personaje);
    guardar_inventario(f, gs->inventario);
    guardar_historial(f, gs->Historial);

    if (fclose(f) != 0)
    {
        fprintf(stderr, "error: fallo al cerrar archivo '%s'\n", ruta);
        return -1;
    }

    return 0;
}

/* Funciones auxiliares privadas para partida_cargar */

/**
 * @brief Lee una linea del archivo, elimina el salto de linea.
 * @param f archivo abierto
 * @param buf buffer donde guardar la linea
 * @param tam tamano del buffer
 * @return 1 si ok, 0 si EOF o error de lectura
 */
static int leer_linea(FILE *f, char *buf, int tam)
{
    if (fgets(buf, tam, f) == NULL)
    {
        return 0;
    }
    buf[strcspn(buf, "\n")] = '\0';
    return 1;
}

/**
 * @brief Verifica que el primer token de la linea sea el esperado.
 * @param linea linea a verificar (se modifica por strtok)
 * @param esperado token que se espera encontrar
 * @return 1 si coincide, 0 si no coincide o linea vacia
 */
static int verificar_token(const char *linea, const char *esperado)
{
    char *tok = strtok((char *)linea, ";");
    if (tok == NULL)
    {
        return 0;
    }
    if (strcmp(tok, esperado) != 0)
    {
        return 0;
    }
    return 1;
}

/**
 * @brief Obtiene el siguiente token de la linea actual.
 * @return puntero al token o NULL si no hay mas.
 */
static char *siguiente_token(void)
{
    return strtok(NULL, ";");
}

/**
 * @brief Lee y valida la seccion PERSONAJE.
 * Formato esperado: PERSONAJE;nombre;vida;vidaMax;ataque;defensa
 * @param f archivo abierto
 * @param pj estructura donde guardar los datos (solo si todo ok)
 * @param nombre_salida puntero donde guardar el nombre duplicado (debe liberarse)
 * @return 0 si ok, -1 si error.
 */
static int cargar_personaje(FILE *f, s_Personaje *pj, char **nombre_salida)
{
    char linea[PARTIDA_LINEA_MAX];
    char *tok;
    char *nombre;

    if (!leer_linea(f, linea, sizeof(linea)))
    {
        fprintf(stderr, "error: EOF inesperado leyendo seccion PERSONAJE\n");
        return -1;
    }

    if (!verificar_token(linea, "PERSONAJE"))
    {
        fprintf(stderr, "error: formato invalido, se esperaba 'PERSONAJE' al inicio\n");
        return -1;
    }

    tok = siguiente_token();
    if (tok == NULL)
    {
        fprintf(stderr, "error: falta nombre en seccion PERSONAJE\n");
        return -1;
    }

    /* strdup reserva memoria en heap para copiar el nombre del personaje.
     * Esta memoria pasa a ser propiedad de pj->nombre y luego de gs->personaje.nombre.
     * Se liberara en partida_cargar al cargar una nueva partida o al salir. */
    nombre = strdup(tok);
    if (nombre == NULL)
    {
        fprintf(stderr, "error: sin memoria para copiar nombre\n");
        return -1;
    }

    tok = siguiente_token();
    if (tok == NULL)
    {
        fprintf(stderr, "error: falta vida en seccion PERSONAJE\n");
        free(nombre);
        return -1;
    }
    pj->vida = atoi(tok);

    tok = siguiente_token();
    if (tok == NULL)
    {
        fprintf(stderr, "error: falta vidaMax en seccion PERSONAJE\n");
        free(nombre);
        return -1;
    }
    pj->vidaMax = atoi(tok);

    tok = siguiente_token();
    if (tok == NULL)
    {
        fprintf(stderr, "error: falta ataque en seccion PERSONAJE\n");
        free(nombre);
        return -1;
    }
    pj->ataque = atoi(tok);

    tok = siguiente_token();
    if (tok == NULL)
    {
        fprintf(stderr, "error: falta defensa en seccion PERSONAJE\n");
        free(nombre);
        return -1;
    }
    pj->defensa = atoi(tok);

    pj->nombre = nombre;
    *nombre_salida = nombre;
    return 0;
}

/**
 * @brief Lee y valida la seccion INVENTARIO.
 * Formato esperado: INVENTARIO;cantidad
 * Seguido de lineas: CARTA;nombre;tipo;valor
 *
 * Las cartas se cargan en una lista aparte: si algo falla, el inventario
 * de gs queda sin modificar.
 *
 * @param f archivo abierto
 * @param inv puntero donde se deja la lista con las cartas cargadas
 * @param nombre_personaje nombre del personaje (para liberar en caso de error)
 * @return 0 si ok, -1 si error.
 */
static int cargar_inventario(FILE *f, Lista *inv, char *nombre_personaje)
{
    char linea[PARTIDA_LINEA_MAX];
    char *tok;
    int cantidad;
    int i;

    if (!leer_linea(f, linea, sizeof(linea)))
    {
        fprintf(stderr, "error: EOF inesperado leyendo seccion INVENTARIO\n");
        free(nombre_personaje);
        return -1;
    }

    if (!verificar_token(linea, "INVENTARIO"))
    {
        fprintf(stderr, "error: formato invalido, se esperaba 'INVENTARIO'\n");
        free(nombre_personaje);
        return -1;
    }

    tok = siguiente_token();
    if (tok == NULL)
    {
        fprintf(stderr, "error: falta cantidad en seccion INVENTARIO\n");
        free(nombre_personaje);
        return -1;
    }
    cantidad = atoi(tok);

    /* Las cartas se cargan en una lista aparte: si algo falla, el inventario
     * de gs queda sin modificar. */
    *inv = crear_inventario(NULL);

    if (*inv == NULL)
    {
        fprintf(stderr, "error: no se pudo crear el inventario\n");
        free(nombre_personaje);
        return -1;
    }

    for (i = 0; i < cantidad; i++)
    {
        char nombre_carta[256];
        s_Carta carta = {0};
        char *nom_tok;
        char *tipo_tok;
        char *valor_tok;

        if (!leer_linea(f, linea, sizeof(linea)))
        {
            fprintf(stderr, "error: EOF inesperado leyendo carta %d de %d\n", i + 1, cantidad);
            free(nombre_personaje);
            vaciar_inventario(*inv);
            free(*inv);
            *inv = NULL;
            return -1;
        }

        if (!verificar_token(linea, "CARTA"))
        {
            fprintf(stderr, "error: formato invalido, se esperaba 'CARTA' en linea %d\n", i + 1);
            free(nombre_personaje);
            vaciar_inventario(*inv);
            free(*inv);
            *inv = NULL;
            return -1;
        }

        nom_tok = siguiente_token();
        tipo_tok = siguiente_token();
        valor_tok = siguiente_token();

        if (nom_tok == NULL || tipo_tok == NULL || valor_tok == NULL)
        {
            fprintf(stderr, "error: campos incompletos en carta %d (nombre;tipo;valor)\n", i + 1);
            free(nombre_personaje);
            vaciar_inventario(*inv);
            free(*inv);
            *inv = NULL;
            return -1;
        }

        strncpy(nombre_carta, nom_tok, sizeof(nombre_carta) - 1);
        nombre_carta[sizeof(nombre_carta) - 1] = '\0';

        carta.nombre = nombre_carta;   /* prestado; agregar_carta copia */
        carta.tipo   = (e_TipoCarta)atoi(tipo_tok);
        carta.valor  = atoi(valor_tok);

        /* agregar_carta() reserva memoria para el nodo (s_Inventario) y
         * duplica el nombre: el nodo es dueno de la copia. */
        agregar_carta(*inv, carta, *inv);
    }

    return 0;
}

/**
 * @brief Convierte texto de tipo de accion a enum TipoAccion.
 * @return valor del enum o -1 si desconocido.
 */
static int tipo_texto_a_enum(const char *texto)
{
    if (strcmp(texto, "A_CONSUMIBLE") == 0)
    {
        return A_CONSUMIBLE;
    }

    if (strcmp(texto, "A_REGISTRO") == 0)
    {
        return A_REGISTRO;
    }

    if (strcmp(texto, "A_COMBATE_I") == 0)
    {
        return A_COMBATE_I;
    }

    if (strcmp(texto, "A_COM_TURNO") == 0)
    {
        return A_COM_TURNO;
    }

    if (strcmp(texto, "A_FIN_COMBATE") == 0)
    {
        return A_FIN_COMBATE;
    }

    if (strcmp(texto, "A_DESCARTE") == 0)
    {
        return A_DESCARTE;
    }

    fprintf(stderr, "error: tipo de accion desconocido '%s'\n", texto);
    return -1;
}

/**
 * @brief Lee y valida la seccion HISTORIAL.
 * Formato esperado: HISTORIAL;cantidad
 * Seguido de lineas: ACCION;tipo;descripcion
 * Las acciones se guardan de mas viejo a mas nuevo en el archivo,
 * asi que al hacer push secuencial la pila queda correcta (mas nuevo en tope).
 * @param f archivo abierto
 * @param gs estado del juego (para acceder/crear historial)
 * @param nombre_personaje nombre del personaje (para liberar en caso de error)
 * @return 0 si ok, -1 si error.
 */
static int cargar_historial(FILE *f, s_GameState *gs, char *nombre_personaje)
{
    char linea[PARTIDA_LINEA_MAX];
    char *tok;
    int cantidad;
    int i;
    historial hist;

    if (!leer_linea(f, linea, sizeof(linea)))
    {
        fprintf(stderr, "error: EOF inesperado leyendo seccion HISTORIAL\n");
        free(nombre_personaje);
        return -1;
    }

    if (!verificar_token(linea, "HISTORIAL"))
    {
        fprintf(stderr, "error: formato invalido, se esperaba 'HISTORIAL'\n");
        free(nombre_personaje);
        return -1;
    }

    tok = siguiente_token();
    if (tok == NULL)
    {
        fprintf(stderr, "error: falta cantidad en seccion HISTORIAL\n");
        free(nombre_personaje);
        return -1;
    }
    cantidad = atoi(tok);

    /* El historial se arma aparte y solo se instala en gs cuando toda la
     * seccion se cargo bien: si algo falla, gs queda sin modificar. */
    hist = historial_make_empty();

    if (hist == NULL)
    {
        fprintf(stderr, "error: no se pudo crear historial\n");
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
            fprintf(stderr, "error: EOF inesperado leyendo accion %d de %d\n", i + 1, cantidad);
            free(nombre_personaje);
            historial_free(&hist);
            return -1;
        }

        if (!verificar_token(linea, "ACCION"))
        {
            fprintf(stderr, "error: formato invalido, se esperaba 'ACCION' en linea %d\n", i + 1);
            free(nombre_personaje);
            historial_free(&hist);
            return -1;
        }

        tipo_tok = siguiente_token();
        desc_tok = siguiente_token();

        if (tipo_tok == NULL || desc_tok == NULL)
        {
            fprintf(stderr, "error: campos incompletos en accion %d (tipo;descripcion)\n", i + 1);
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
            fprintf(stderr, "aviso: tipo de accion desconocido '%s', se omite\n", tipo_txt);
            continue;
        }

        /* historial_push() reserva memoria para un nuevo nodo (s_NodoHistorial)
         * y para copiar la descripcion (strdup). */
        if (historial_push(hist, desc, (TipoAccion)t) != 0)
        {
            fprintf(stderr, "error: fallo al agregar accion al historial\n");
            free(nombre_personaje);
            historial_free(&hist);
            return -1;
        }
    }

    /* Todo salio bien: se reemplaza el historial de gs por el recien cargado */
    historial_free(&gs->Historial);
    gs->Historial = hist;

    return 0;
}

/* Funcion principal de carga */

/**
 * @brief Carga el estado del juego desde un archivo CSV.
 * Si el archivo no existe o esta corrupto, deja gs sin modificar y
 * devuelve -1. El llamador decide si arrancar una partida nueva.
 *
 * Formato del archivo:
 *   linea 1: PERSONAJE;nombre;vida;vidaMax;ataque;defensa
 *   linea 2: INVENTARIO;cantidad
 *   lineas siguientes: CARTA;nombre;tipo;valor   (una por carta)
 *   linea siguiente: HISTORIAL;cantidad
 *   lineas siguientes: ACCION;tipo;descripcion   (una por accion)
 *
 * @param gs    Estado del juego destino (no NULL).
 * @param ruta  Ruta del archivo (si NULL, usa PARTIDA_RUTA).
 * @return 0 si todo va bien, -1 en caso de error.
 */
int partida_cargar(s_GameState *gs, const char *ruta)
{
    FILE *f;
    s_Personaje pj = {0};
    char *nombre_personaje = NULL;
    Lista inv = NULL;

    if (gs == NULL)
    {
        fprintf(stderr, "error: estado de juego invalido (NULL)\n");
        return -1;
    }

    if (ruta == NULL)
    {
        ruta = PARTIDA_RUTA;
    }

    f = fopen(ruta, "r");
    if (f == NULL)
    {
        fprintf(stderr, "error: no se pudo abrir archivo '%s' para lectura\n", ruta);
        return -1;
    }

    /* Cargar cada seccion secuencialmente. Si alguna falla, liberamos
     * lo reservado y devolvemos error sin modificar gs. */

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

    /* Todo salio bien: se aplica el inventario cargado al estado real */
    if (gs->inventario != NULL)
    {
        vaciar_inventario(gs->inventario);
        free(gs->inventario);
    }

    gs->inventario = inv;

    /* Liberar nombre anterior del personaje si existe.
     * Este nombre fue reservado con strdup() en una carga anterior
     * y pertenece a gs->personaje.nombre. */
    if (gs->personaje.nombre != NULL)
    {
        char *tmp = (char *)gs->personaje.nombre;
        free(tmp);
    }

    /* Todo salio bien: aplicar al estado real */
    gs->personaje = pj;
    gs->ejecutando = true;
    if (gs->Historial != NULL)
    {
        gs->Historial->HayReversionPendiente = false;  /* partida cargada: sin undo */
    }

    /* nombre_personaje NO se libera aqui: ahora es propiedad de gs->personaje.nombre
     * Se liberara cuando se destruya el personaje o se cargue otra partida */
    return 0;
}