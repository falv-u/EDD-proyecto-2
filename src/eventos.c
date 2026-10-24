#include "juego.h"


int definir_evento()
{
    int prob = rand()%100;

    if(prob < 40)
        return 1;
    else if(prob<60)
        return 2;
    else if(prob<70)
        return 3;
    else 
        return 0;

}


