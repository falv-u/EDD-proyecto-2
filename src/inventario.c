#include "juego.h"

/**
 * @brief Reserva memoria y copia una cadena terminada en '\0'.
 *
 * Se usa para que cada nodo del inventario tenga su propia copia
 * del nombre de la carta.
 *
 * @param or Cadena origen (no NULL).
 * @return Puntero a la copia en heap, o NULL si or es NULL o
 *         falla malloc.
 */
static char *copiar_cadena(const char *or)
{
    if(or == NULL)
        return NULL;

    size_t tam = strlen(or) + 1;
    char *copia = malloc(tam);

    if(copia == NULL)
        return NULL;

    memcpy(copia, or, tam);
    return copia;
}

void inventario(s_GameState *gs)
{
    int opcion = -1;
    Posicion puntCarta;

    if(gs == NULL || gs->inventario == NULL)
        return;

    while(opcion != 6)
    {
        mostrar_menu_inventario(cantidad_cartas(gs->inventario));
        opcion = leer_entero_en_rango(1, 6);

        switch(opcion)
        {
            case 1:
                imprimir_cartas(gs->inventario);
                break;

            case 2:
                buscar_carta_interactivo(gs->inventario);
                break;

            case 3:
                usar_carta_en_combate(gs);
                break;

            case 4:
                puntCarta = seleccionar_carta(gs->inventario);

                if(puntCarta != NULL)
                    descartar_carta(gs, puntCarta);

                break;

            case 5:
                restaurar_carta(gs);
                break;

            case 6:
                ui_mensaje_info("Volviendo al menu principal...");
                ui_pausa_ms(PAUSA_CORTA);
                break;
        }

        if(opcion != 6)
            ui_esperar_enter();
    }
}

Lista crear_inventario(Lista I)
{
    if(I != NULL)
    {
        vaciar_inventario(I);
        return I;
    }

    I = malloc(sizeof(s_Inventario));

    if(I == NULL)
    {
        ui_mensaje_error("No se pudo crear el inventario");
        return NULL;
    }

    I->next = NULL;
    I->prev = NULL;

    return I;
}

void vaciar_inventario(Lista I)
{
    if(I == NULL)
        return;

    Posicion aux = I->next;
    Posicion siguiente;

    while(aux != NULL)
    {
        siguiente = aux->next;
        free(aux->carta.nombre);
        free(aux);
        aux = siguiente;
    }

    I->next = NULL;
    I->prev = NULL;
}

void agregar_carta(Lista I, s_Carta carta, Posicion P)
{
    if(I == NULL || P == NULL) 
        return; 

    Posicion aux;
    
    aux = malloc(sizeof(s_Inventario));

    if(aux == NULL)
    {
        ui_mensaje_error("No se pudo agregar la carta (sin memoria)");
        return;
    }

    aux->carta = carta;

    /* El nodo es dueno de su propia copia del nombre; se libera
     * en vaciar_inventario()/eliminar_carta(). */
    aux->carta.nombre = copiar_cadena(carta.nombre);

    if(aux->carta.nombre == NULL)
    {
        free(aux);
        ui_mensaje_error("No se pudo copiar el nombre de la carta");
        return;
    }

    aux->next = P->next;
    aux->prev = P;

    if(P->next != NULL)
        P->next->prev = aux;

    P->next = aux;
}

void eliminar_carta(Lista I, Posicion P)
{
    if(P == NULL || P == I)
        return;

    P->prev->next = P->next;

    if(P->next != NULL)
        P->next->prev = P->prev;

    free(P->carta.nombre);
    free(P);
}

Posicion encontrar_carta(Lista I, int posicion)
{
     if(I == NULL || posicion < 1)
        return NULL;

    Posicion aux = I->next;
    int cont = 1;

    while(aux != NULL && cont < posicion)
    {
        aux = aux->next;
        cont++;
    }

    return aux;
}

void imprimir_cartas(Lista L) 
{
    int indice = 1;
    Posicion aux;

    if(L == NULL)
    {
        ui_mensaje_error("El inventario no esta inicializado");
        return;
    }

    printf("\n");
    ui_linea('-', COLOR_CIAN);

    if(L->next == NULL)
    {
        ui_mensaje_aviso("El inventario esta vacio.");
        return;
    }

    aux = L->next;
    while(aux != NULL)
    {
        printf(" %2d. ", indice);
        imprimir_carta_sola(aux->carta);
        indice++;
        aux = aux->next;
    }
    ui_linea('-', COLOR_CIAN);
}

int cantidad_cartas(Lista I)
{
    if(I == NULL)
    {
        printf("\nLa lista esta vacia\n");
        return 0;
    }

    int cantidad = 0;
    Posicion aux = I->next;

    while(aux != NULL)
    {
        cantidad++;
        aux = aux->next;
    }

    return cantidad;
}

Posicion buscar_carta_nombre(Lista I, const char *nombre)
{
    if(I == NULL || nombre == NULL)
        return NULL;

    Posicion aux = I->next;

    while(aux != NULL)
    {
        if(strcmp(aux->carta.nombre, nombre) == 0)
            return aux;

        aux = aux->next;
    }

    return NULL;
}

Posicion seleccionar_carta(Lista I)
{
    if(I == NULL || I->next == NULL)
    {
        ui_mensaje_aviso("El inventario esta vacio");
        return NULL;
    }

    int num;
    Posicion puntCarta;

    imprimir_cartas(I);

    printf("\nIngrese el numero de la carta: ");
    num = leer_entero();

    puntCarta = encontrar_carta(I, num);

    if(puntCarta == NULL)
        ui_mensaje_aviso("Carta no encontrada en el inventario");

    return puntCarta;
}

void descartar_carta(s_GameState *gs, Posicion puntCarta)
{
    if(gs == NULL || gs->inventario == NULL || puntCarta == NULL)
        return;

    if(gs->Historial == NULL)
    {
        ui_mensaje_error("El historial no esta inicializado");
        return;
    }

    int numero = 1;
    Posicion aux = gs->inventario->next;

    while(aux != NULL && aux != puntCarta)
    {
        aux = aux->next;
        numero++;
    }

    if(aux == NULL)
        return;

    s_Reversion rev;

    rev.carta = puntCarta->carta;

    /* La carta se elimina del inventario y su nombre se libera, por eso la
     * reversion guarda una copia propia del nombre para poder restaurarla. */
    rev.carta.nombre = copiar_cadena(puntCarta->carta.nombre);
    rev.posicion = numero;

    if (rev.carta.nombre == NULL)
    {
        ui_mensaje_error("No se pudo copiar el nombre de la carta");
        return;
    }

    if(historial_registrar(gs->Historial, "Carta descartada", A_DESCARTE, &rev) != 0)
    {
        ui_mensaje_error("Error al registrar el descarte");
        free(rev.carta.nombre);
        return;
    }

    /* El historial toma dueno del nombre copiado */
    rev.carta.nombre = NULL;

    ui_mensaje_exito("Has descartado %s", puntCarta->carta.nombre);

    eliminar_carta(gs->inventario, puntCarta);
}

/**
 * @brief Calcula la posicion del inventario donde debe insertarse una carta
 *        para devolverla a su lugar original.
 *
 * La posicion es 1-based como en el inventario. Si la posicion es 1 la carta
 * va al principio (justo despues de la cabecera). Si no hay cartas suficientes
 * la carta se agrega al final de la lista.
 *
 * @param I        Lista del inventario (no NULL).
 * @param posicion Posicion original de la carta (1-based).
 * @return Posicion donde insertar la carta, o NULL si la lista es NULL.
 */
static Posicion posicion_para_restaurar(Lista I, int posicion)
{
    Posicion anterior;
    Posicion aux;

    if (I == NULL)
        return NULL;

    /* Posicion 1: la carta va al principio, despues de la cabecera */
    if (posicion <= 1)
        return I;

    /* Al quitar la carta, la que estaba despues ocupa su lugar */
    anterior = encontrar_carta(I, posicion - 1);
    if (anterior != NULL)
        return anterior;

    /* No hay cartas suficientes: se inserta al final de la lista */
    aux = I->next;
    if (aux == NULL)
        return I;

    while (aux->next != NULL)
        aux = aux->next;

    return aux;
}

/**
 * @brief Deshace el descarte de la ultima carta: la reinserta en el
 *        inventario en la posicion original y saca la accion del historial.
 *
 * Solo se puede deshacer si la ultima accion registrada fue descartar una
 * carta. En cualquier otro caso se avisa al jugador y no se modifica nada.
 *
 * @param gs Puntero al estado del juego.
 * @return true si se deshizo el descarte, false si no era posible.
 * @author Francisco Alvarado
 */
bool restaurar_carta(s_GameState *gs)
{
    s_Reversion rev;
    Posicion insercion;
    int cartas_antes;

    if (gs == NULL || gs->inventario == NULL || gs->Historial == NULL)
    {
        ui_mensaje_error("No se puede deshacer: el estado del juego no esta listo");
        return false;
    }

    if (!historial_puede_deshacer(gs->Historial))
    {
        ui_mensaje_aviso("No se puede deshacer: la ultima accion no fue descartar una carta");
        return false;
    }

    if (historial_obtener_reversion(gs->Historial, &rev) != 0)
    {
        ui_mensaje_error("No se pudo obtener la accion a deshacer");
        return false;
    }

    insercion = posicion_para_restaurar(gs->inventario, rev.posicion);
    if (insercion == NULL)
    {
        ui_mensaje_error("No se pudo restaurar la carta: el inventario no esta disponible");
        return false;
    }

    cartas_antes = cantidad_cartas(gs->inventario);

    /* El nombre sigue siendo del historial: agregar_carta hace su propia copia */
    agregar_carta(gs->inventario, rev.carta, insercion);

    if (cantidad_cartas(gs->inventario) == cartas_antes)
    {
        ui_mensaje_error("No se pudo restaurar la carta: no hay memoria suficiente");
        return false;
    }

    ui_mensaje_exito("Has restaurado %s", rev.carta.nombre ? rev.carta.nombre : "la carta");

    if (historial_deshacer(gs->Historial) != 0)
    {
        ui_mensaje_error("No se pudo deshacer la accion en el historial");
        return false;
    }

    return true;
}

bool confirmar_uso_carta(Posicion puntCarta)
{
    if(puntCarta == NULL)
        return false;

    mostrar_descripcion_carta(puntCarta->carta);

    printf("\nEstas seguro de utilizar esta carta? ");
    return leer_si_no();
}


void buscar_carta_interactivo(Lista I)
{
    char nombre[100];
    Posicion puntCarta;

    printf("\nIngrese el nombre de la carta: ");

    if(fgets(nombre, sizeof(nombre), stdin) == NULL)
        return;
    nombre[strcspn(nombre, "\n")] = '\0';

    puntCarta = buscar_carta_nombre(I, nombre);

    if(puntCarta == NULL)
    {
        ui_mensaje_aviso("Carta no encontrada en el inventario");
    }
    else
    {
        ui_mensaje_exito("Carta encontrada:");
        imprimir_carta_sola(puntCarta->carta);
    }
}


void mostrar_menu_inventario(int cantidad)
{
    ui_limpiar_pantalla();
    ui_titulo("INVENTARIO", COLOR_CIAN);
    printf("Cartas guardadas: %s%d%s\n\n", COLOR_AMARILLO, cantidad, COLOR_RESET);
    ui_opcion_menu(1, "Mostrar cartas");
    ui_opcion_menu(2, "Buscar carta por nombre");
    ui_opcion_menu(3, "Usar carta");
    ui_opcion_menu(4, "Descartar carta");
    ui_opcion_menu(5, "Deshacer descarte");
    ui_opcion_menu(6, "Volver");
    imprimir_ingresar_opcion();
}