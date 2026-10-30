#include "juego.h"
#include "historial.h"
 
/* Prototipos de las funciones de este archivo */
void liberar_memoria(s_GameState *gs);
void mostrar_menu(const s_GameState *gs);
bool inicializar_juego(s_GameState *gs);
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
    ui_opcion_menu(4, "Salir");
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
 * @brief Ejecuta la opcion elegida en el menu principal.
 *
 * @param gs     Estado del juego (no NULL).
 * @param opcion Opcion elegida (1 a 4).
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
 
    if (!inicializar_juego(&gs))
    {
        liberar_memoria(&gs);
        return 1;
    }
 
    while (gs.ejecutando)
    {
        mostrar_menu(&gs);
        ejecutar_opcion_menu(&gs, leer_entero_en_rango(1, 4));
    }
 
    /* Liberar toda la memoria antes de salir */
    liberar_memoria(&gs);
 
    return 0;
}
 