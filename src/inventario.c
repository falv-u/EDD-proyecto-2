#include "juego.h"

void inventario(s_GameState *gs)
{

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
    
}