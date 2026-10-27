#include <stdio.h>
#include <stdlib.h>
 
#include "cola.h"
 
struct s_Cola
{
    int capacidad;
    int frente;      /* indice del elemento que sale primero */
    int final;       /* indice del ultimo elemento insertado */
    int tamanho;      /* cantidad de elementos */
    s_Enemigo *arreglo;
};
 
/** Avanza un indice de forma circular (vuelve a 0 al llegar al final). */
int cola_siguiente_indice(const Cola c, int valor)
{
    valor++;
    if(valor == c->capacidad)
        valor = 0;
    return valor;
}
 
Cola cola_crear(int capacidad)
{
    Cola c;
 
    if(capacidad < 1)
    {
        fprintf(stderr, "error: capacidad de cola invalida\n");
        return NULL;
    }
 
    c = malloc(sizeof(struct s_Cola));
    if(c == NULL)
    {
        fprintf(stderr, "error: sin memoria para crear la cola\n");
        return NULL;
    }
 
    c->arreglo = malloc(sizeof(s_Enemigo) * capacidad);
    if(c->arreglo == NULL)
    {
        fprintf(stderr, "error: sin memoria para el arreglo de la cola\n");
        free(c);
        return NULL;
    }
 
    c->capacidad = capacidad;
    cola_vaciar(c);
    return c;
}
 
void cola_vaciar(Cola c)
{
    if(c == NULL)
        return;
 
    c->tamanho = 0;
    c->frente = 1;
    c->final = 0; 
}
 
bool cola_esta_vacia(const Cola c)
{
    if(c == NULL)
        return true;
    else if(c->tamanho == 0)
        return true;
    else
        return false;

}
 
bool cola_esta_llena(const Cola c)
{
    if(c == NULL)
        return true;
    else if(c->tamanho == c->capacidad)
        return true;
    else
        return false;

}
 
int cola_tamanho(const Cola c)
{
    if(c == NULL)
        return 0;
    
    return c->tamanho;
}
 
int cola_encolar(Cola c, s_Enemigo enemigo)
{
    if(c == NULL)
        return -1;
 
    if(cola_esta_llena(c))
    {
        fprintf(stderr, "error: cola llena\n");
        return -1;
    }
 
    c->tamanho++;
    c->final = cola_siguiente_indice(c, c->final);
    c->arreglo[c->final] = enemigo;
    return 0;
}
 
int cola_desencolar(Cola c)
{
    if(c == NULL)
        return -1;
 
    if(cola_esta_vacia(c))
    {
        fprintf(stderr, "error: cola vacia\n");
        return -1;
    }
 
    c->tamanho--;
    c->frente = cola_siguiente_indice(c, c->frente);
    return 0;
}
 
s_Enemigo *cola_frente(const Cola c)
{
    if(cola_esta_vacia(c))
        return NULL;
 
    return &c->arreglo[c->frente];
}
 
void cola_destruir(Cola c)
{
    if(c == NULL)
        return;
 
    free(c->arreglo);
    free(c);
}