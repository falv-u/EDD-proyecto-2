#include "juego.h"

void inventario(s_GameState *gs)
{
    int opcion = -1;
    Posicion puntCarta;

    if(gs == NULL || gs->inventario == NULL)
        return;

    while(opcion != 5)
    {
        printf("\n========== INVENTARIO ==========\n\n"
               "1. Mostrar cartas\n"
               "2. Buscar carta por nombre\n"
               "3. Usar carta\n"
               "4. Descartar carta\n"
               "5. Volver\n");

        imprimir_ingresar_opcion();
        opcion = leer_entero_en_rango(1, 5);

        switch(opcion)
        {
            case 1:
                imprimir_cartas(gs->inventario);
                break;

            case 2:
            {
                char nombre[100];

                printf("\nIngrese el nombre de la carta: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = '\0';

                puntCarta = buscar_carta_nombre(gs->inventario, nombre);

                if(puntCarta == NULL)
                {
                    printf("\nCarta no encontrada en el inventario\n");
                }
                else
                {
                    printf("\nCarta encontrada:\n");
                    imprimir_carta_sola(puntCarta->carta);
                }
                break;
            }

            case 3:
                puntCarta = seleccionar_carta(gs->inventario);

                if(puntCarta != NULL)
                    usar_carta(gs, puntCarta);

                break;

            case 4:
                puntCarta = seleccionar_carta(gs->inventario);

                if(puntCarta != NULL)
                    descartar_carta(gs, puntCarta);

                break;

            case 5:
                printf("\nVolviendo al menu principal...\n");
                break;
        }
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
        printf("\nError: No se pudo crear el inventario\n");
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
        printf("\nError: No se pudo agregar la carta\n");
        return;
    }

    aux->carta = carta;
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
    int cant = 0;
    Posicion aux = L->next;

    printf("\n");
    while(aux != NULL)
    {
        printf("%d. %s\t", cant + 1, aux->carta.nombre);

        if((++cant % 3) == 0) 
            printf("\n");
        
        aux = aux->next;
    }
    printf("\n");
}

int cantidad_cartas(Lista L)
{
    if(L == NULL)
    {
        printf("\nLa lista esta vacia\n");
        return 0;
    }

    int cantidad = 0;
    Posicion aux = L->next;

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
        printf("\nEl inventario esta vacio\n");
        return NULL;
    }

    int num;
    Posicion puntCarta;

    imprimir_cartas(I);

    printf("\nIngrese el numero de la carta: ");
    num = leer_entero();

    puntCarta = encontrar_carta(I, num);

    if(puntCarta == NULL)
        printf("\nCarta no encontrada en el inventario\n");

    return puntCarta;
}

void descartar_carta(s_GameState *gs, Posicion puntCarta)
{
    if(gs == NULL || gs->inventario == NULL || puntCarta == NULL)
        return;

    if(gs->Historial == NULL)
    {
        printf("\nError: historial es NULL\n");
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
    rev.posicion = numero;

    if(historial_registrar(gs->Historial, "Carta descartada", A_DESCARTE, &rev) != 0)
    {
        printf("\nError al registrar el descarte\n");
        return;
    }

    printf("\nHas descartado %s\n", puntCarta->carta.nombre);

    eliminar_carta(gs->inventario, puntCarta);
}

bool usar_carta(s_GameState *gs, Posicion puntCarta)
{
    if(gs == NULL || puntCarta == NULL)
        return;

    s_Carta carta = puntCarta->carta;
    s_Personaje *pj = &gs->personaje;

    switch(carta.tipo)
    {
        case ATAQUE:
            pj->ataque += carta.valor;
            printf("\n+%d Ataque\n", carta.valor);
            break;

        case DEFENSA:
            pj->defensa += carta.valor;
            printf("\n+%d Defensa\n", carta.valor);
            break;

        case CURACION:
            pj->vida += carta.valor;

            if(pj->vida > pj->vidaMax)
                pj->vida = pj->vidaMax;

            printf("\nVida actual: %d/%d\n", pj->vida, pj->vidaMax);
            break;

        case VIDAMAXIMA:
            pj->vidaMax += carta.valor;
            pj->vida = pj->vidaMax;

            printf("\nVida maxima aumentada a %d\n", pj->vidaMax);
            break;

        case ESPECIAL:
            // falta agregar
            return;
    }

    printf("\nSe ha utilizado %s\n", carta.nombre);

    eliminar_carta(gs->inventario, puntCarta);
}