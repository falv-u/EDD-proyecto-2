#include "combate.h"

int calcular_danho(int ataque,int defensa, bool *critico)
{
    int danho = ataque - defensa;
    if(danho < 1)
        danho = 1;

    *critico = (rand()%10) < PROB_CRITICOS;
    if(*critico)
        danho *= 2;
    
    return danho;

}

Cola generar_grupo()
{
    int cantidad = 1 + rand()%MAX_GRUPO;
    Cola grupo = cola_crear(cantidad);

    if(grupo == NULL)
        return NULL;
    
    printf("\n\t Un grupo de enemigos te bloquea el paso!\n\tEnemigos : \n");

    for (int i = 1 ; i<= cantidad ; i++)
    {
        s_Enemigo enemigo = enemigo_aleatorio(i);

        if(cola_encolar(grupo,enemigo) != 0)
        {
            cola_destruir(grupo);
            return NULL;
        }
        printf(" %d. %s (%d PV)\n", enemigo.id, enemigo.nombre, enemigo.vida);
    }
    printf("Siguiente enemigo: %s\n", cola_frente(grupo)->nombre);
    return grupo;
}

void mostrar_estado(const s_GameState *gs, const s_Enemigo *enemigo, int enEspera)
{
    printf("\n===== COMBATE =====\n");
    printf("%s: %d/%d PV | Ataque %d | Defensa %d\n",
           gs->personaje.nombre, gs->personaje.vida, gs->personaje.vidaMax,
           gs->personaje.ataque, gs->personaje.defensa);
    printf("Enemigo actual: %s (%d/%d PV)\n", enemigo->nombre, enemigo->vida, enemigo->vidaMax);
    printf("Enemigos en espera: %d\n", enEspera);
    printf("\n1. Atacar\n2. Ver inventario\n3. Usar carta");
    imprimir_ingresar_opcion();
}

void atacar_enemigo(const s_GameState *gs, s_Enemigo *enemigo)
{
    bool critico;
    int danho = calcular_danho(gs->personaje.ataque, enemigo->defensa, &critico);
 
    enemigo->vida -= danho;
    if(enemigo->vida < 0)
        enemigo->vida = 0;
 
    if(critico)
        printf("!Golpe critico!\n");
    printf("Atacas a %s: %d de danho. %s: %d/%d PV.\n",
           enemigo->nombre, danho, enemigo->nombre, enemigo->vida, enemigo->vidaMax);
}