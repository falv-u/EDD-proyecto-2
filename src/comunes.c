#include "comunes.h"
#include "ui.h"

int leer_entero()
{
    int var; // Variable a retornar
    int c; // Sirve para limpiar el buffer

    while(scanf("%d", &var) != 1) // Si se lee un numero entero devuelve 1, si no repite
    {
        if(feof(stdin)) //Pregunta si llego al final de la entrada
            exit(1);

        while ((c = getchar()) != '\n' && c != EOF); // Limpia el buffer
        printf("\033[1;31m\nERROR:\033[0m \033[31mDebe ingresar un numero entero\033[0m\n\nIntente nuevamente: ");
    }

    while ((c = getchar()) != '\n' && c != EOF); // Limpia el buffer

    return var; 
}

void validar_rango(int *num, int min, int max)
{
    while(*num > max || *num < min)
    {
        printf("Ingrese un numero entre %d y %d: ", min, max);
        *num = leer_entero();
    }  
}

int leer_entero_en_rango(int min, int max)
{
    int var = leer_entero();
    validar_rango(&var, min, max);

    return var;
}

bool leer_si_no()
{
    int c;
    int extra;

    while(1)
    {
        printf("(s/n) > ");
        c = getchar();

        if(c == EOF)
            exit(1);

        if(c != '\n')
            while((extra = getchar()) != '\n' && extra != EOF); // Limpia el buffer

        if(c == 's' || c == 'S')
            return true;
        if(c == 'n' || c == 'N')
            return false;

        printf("\033[1;31mERROR:\033[0m Responde con 's' o 'n'\n");
    }
}

void imprimir_ingresar_opcion()
{
    printf("\n%sIngresa una opcion%s > ", COLOR_CIAN, COLOR_RESET);
}