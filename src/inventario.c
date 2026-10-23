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

int imprimir_cartas(Lista L) //Recorrer la lista e imprimir las cartas mientras aumenta cant para obtener la posicion a elejir
{
    int cantidad = 0;

    return cantidad;
}