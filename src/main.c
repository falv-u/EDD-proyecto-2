#include "juego.h"
#include "historial.h"
 
/* Prototipos de las funciones de este archivo */
void liberar_memoria(s_GameState *gs);
void mostrar_menu(const s_GameState *gs);
void mostrar_menu_inicio(void);
bool inicializar_juego(s_GameState *gs);
bool cargar_partida(s_GameState *gs);
void guardar_partida(const s_GameState *gs);
void ejecutar_opcion_menu(s_GameState *gs, int opcion);
 
/**
 * @brief Libera toda la memoria dinamica del estado del juego
 * @param gs puntero al estado del juego
 */
void liberar_memoria(s_GameState *gs)
{
    if (gs == NULL)
    {
        return;
    }
 
    /* Liberar nombre del personaje si existe */
    if (gs->personaje.nombre != NULL)
    {
        char *tmp = (char *)gs->personaje.nombre;
        free(tmp);
        gs->personaje.nombre = NULL;
    }
 
    /* Liberar inventario: vaciar nodos y liberar nodo cabeza */
    if (gs->inventario != NULL)
    {
        vaciar_inventario(gs->inventario);
        free(gs->inventario);
        gs->inventario = NULL;
    }
 
    /* Liberar historial completamente (nodos + struct) */
    if (gs->Historial != NULL)
    {
        historial_free(&gs->Historial);
    }
}
 
/**
 * @brief Limpia la pantalla y muestra el menu principal con el estado del jugador.
 * @param gs Estado del juego (no NULL).
 */
void mostrar_menu(const s_GameState *gs)
{   
    ui_limpiar_pantalla();
    ui_titulo("GREED ISLAND", COLOR_AMARILLO);
    printf("Jugador: %s%s%s\n", COLOR_VERDE, gs->personaje.nombre, COLOR_RESET);
    printf("Vida:    ");
    ui_barra_vida(gs->personaje.vida, gs->personaje.vidaMax, ANCHO_BARRA);
    printf("\n\n");
    ui_opcion_menu(1, "Ver personaje");
    ui_opcion_menu(2, "Explorar Antokiba");
    ui_opcion_menu(3, "Ver inventario");
    ui_opcion_menu(4, "Guardar partida");
    ui_opcion_menu(5, "Salir");
    imprimir_ingresar_opcion();
}

/**
 * @brief Muestra el menu inicial donde se elige empezar una partida nueva o cargar una guardada.
 */
void mostrar_menu_inicio(void)
{
    ui_limpiar_pantalla();
    ui_titulo("GREED ISLAND", COLOR_AMARILLO);
    printf("\n");
    ui_opcion_menu(1, "Nueva partida");
    ui_opcion_menu(2, "Cargar partida");
    imprimir_ingresar_opcion();
}
 
/**
 * @brief Prepara el juego: inventario, historial, personaje y cartas iniciales.
 *
 * @param gs Estado del juego (no NULL).
 * @return true si todo se inicializo bien, false si falto memoria.
 */
bool inicializar_juego(s_GameState *gs)
{
    int i;
 
    gs->inventario = crear_inventario(NULL);
    gs->Historial = historial_make_empty();
    gs->ejecutando = true;
 
    if (gs->inventario == NULL || gs->Historial == NULL)
    {
        ui_mensaje_error("No se pudo reservar memoria para iniciar el juego");
        return false;
    }
 
    ui_pantalla_titulo();
    asignar_personaje(&gs->personaje);
 
    if (gs->personaje.nombre == NULL)
    {
        return false;
    }
 
    ui_limpiar_pantalla();
    imprimir_personaje(&gs->personaje);
    printf("\n");
    ui_mensaje_info("Recibes 3 cartas iniciales:");
    ui_pausa_ms(PAUSA_MEDIA);
 
    for (i = 0; i < 3; i++)
    {
        agregar_carta(gs->inventario, carta_aleatoria(), gs->inventario);
    }
 
    imprimir_cartas(gs->inventario);
    ui_esperar_enter();
    return true;
}

/**
 * @brief Carga una partida guardada en el estado del juego.
 *
 * La carga se hace sobre un estado auxiliar para no romper el estado actual
 * si el archivo no existe o esta corrupto. Solo si la carga termina bien se
 * reemplaza el contenido de gs, liberando antes lo que ya estuviera reservado.
 *
 * @param gs Estado del juego (no NULL).
 * @return true si la partida se cargo correctamente, false si fallo la carga.
 * @author Francisco Alvarado
 */
bool cargar_partida(s_GameState *gs)
{
    s_GameState aux = {0};

    /* El inventario debe existir para que la carga lo vacie y llene.
     * El historial se deja en NULL: partida_cargar lo crea al leer su seccion. */
    aux.inventario = crear_inventario(NULL);

    if (aux.inventario == NULL)
    {
        ui_mensaje_error("No se pudo reservar memoria para cargar la partida");
        liberar_memoria(&aux);
        return false;
    }

    if (partida_cargar(&aux, NULL) != 0)
    {
        ui_mensaje_error("No se pudo cargar la partida: archivo inexistente o corrupto");
        liberar_memoria(&aux);
        return false;
    }

    if (aux.Historial == NULL)
    {
        ui_mensaje_error("No se pudo crear el historial de la partida cargada");
        liberar_memoria(&aux);
        return false;
    }

    /* Liberar lo que hubiera reservado en gs para no duplicar memoria */
    liberar_memoria(gs);

    gs->personaje = aux.personaje;
    gs->inventario = aux.inventario;
    gs->Historial = aux.Historial;
    gs->ejecutando = true;

    /* aux ya no es dueño de estos recursos, evita doble liberacion */
    aux.personaje.nombre = NULL;
    aux.inventario = NULL;
    aux.Historial = NULL;

    ui_mensaje_exito("Partida cargada: %s", gs->personaje.nombre);
    ui_esperar_enter();
    return true;
}

/**
 * @brief Guarda el estado actual del juego en el archivo de partida.
 *
 * @param gs Estado del juego (no NULL).
 * @author Francisco Alvarado
 */
void guardar_partida(const s_GameState *gs)
{
    if (partida_guardar(gs, NULL) == 0)
    {
        ui_mensaje_exito("Partida guardada en '%s'", PARTIDA_RUTA);
    }
    else
    {
        ui_mensaje_error("No se pudo guardar la partida");
    }

    ui_esperar_enter();
}
 
/**
 * @brief Ejecuta la opcion elegida en el menu principal.
 *
 * @param gs     Estado del juego (no NULL).
 * @param opcion Opcion elegida (1 a 5).
 */
void ejecutar_opcion_menu(s_GameState *gs, int opcion)
{
    switch (opcion)
    {
        case 1:
            ui_limpiar_pantalla();
            imprimir_personaje(&gs->personaje);
            ui_esperar_enter();
            break;
        case 2:
            exploracion(gs, 0);
            break;
        case 3:
            inventario(gs);
            break;
        case 4:
            ui_limpiar_pantalla();
            guardar_partida(gs);
            break;
        case 5:
            printf("\n");
            ui_mensaje_info("Gracias por jugar. Hasta pronto!");
            ui_pausa_ms(PAUSA_MEDIA);
            gs->ejecutando = false;
            break;
    }
}
 
int main(void)
{
    srand(time(NULL));
    s_GameState gs = {0};
    bool juego_iniciado = false;

    /* Menu inicial: nueva partida o cargar una guardada */
    while (!juego_iniciado)
    {
        mostrar_menu_inicio();

        switch (leer_entero_en_rango(1, 2))
        {
            case 1:
                if (!inicializar_juego(&gs))
                {
                    liberar_memoria(&gs);
                    return 1;
                }
                juego_iniciado = true;
                break;
            case 2:
                /* Si la carga falla se vuelve a ofrecer el menu inicial */
                juego_iniciado = cargar_partida(&gs);
                break;
        }
    }

    while (gs.ejecutando)
    {
        mostrar_menu(&gs);
        ejecutar_opcion_menu(&gs, leer_entero_en_rango(1, 5));
    }
 
    /* Liberar toda la memoria antes de salir */
    liberar_memoria(&gs);
 
    return 0;
}
 