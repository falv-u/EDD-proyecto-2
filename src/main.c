#include "juego.h"
#include "historial.h"

/**
 * @brief Libera toda la memoria dinamica del estado del juego
 * @param gs puntero al estado del juego
 */
static void liberar_memoria(s_GameState *gs)
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
 * @brief Muestra el menu del juego
 * @param gs
 */
static void mostrar_menu(const s_GameState *gs)
{
    printf("\n===== GREED ISLAND =====\n");
    printf("Jugador: %s\n", gs->personaje.nombre);
    printf("1. Ver personaje\n2. Explorar Antokiba\n3. Ver inventario\n4. Salir\n");
    imprimir_ingresar_opcion();
}

int main(void)
{
    srand(time(NULL));
    int pasos = 0;
    s_GameState gs = {0};

    gs.inventario = crear_inventario(NULL);
    iniciar_juego(&gs);
    imprimir_cartas(gs.inventario);

    while(gs.ejecutando)
    {
        mostrar_menu(&gs);

        switch(leer_entero_en_rango(1, 4))
        {
            case 1: 
                imprimir_personaje(&gs.personaje);
                break;
            case 2: 
                exploracion(&gs, 0); 
                break;
            case 3: 
                imprimir_cartas(gs.inventario); 
                    break;
            case 4:
                gs.ejecutando = false;
                break;
        }
    }

    /* Liberar toda la memoria antes de salir */
    liberar_memoria(&gs);

    return 0;
}