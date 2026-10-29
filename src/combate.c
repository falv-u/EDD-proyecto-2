#include "combate.h"
#include "inventario.h"
#include "historial.h"
#include "comunes.h"
#include "enemigo.h"
#include "cola.h"

/**
 * @brief Calcula el dano base restando defensa al ataque.
 * Aplica probabilidad de critico (PROB_CRITICOS%).
 *
 * @param ataque  Valor de ataque del atacante.
 * @param defensa Valor de defensa del defensor.
 * @param critico Puntero a bool donde se almacena si fue critico.
 * @return Dano final (minimo 1, doble si critico).
 */
int calcular_danho(int ataque, int defensa, bool *critico)
{
    int danho = ataque - defensa;
    if (danho < 1)
        danho = 1;

    *critico = (rand() % 100) < PROB_CRITICOS;
    if (*critico)
        danho *= 2;

    return danho;
}

/**
 * @brief Crea una cola con 1 a MAX_GRUPO enemigos aleatorios.
 * Cada enemigo recibe un ID incremental para definir el orden de combate.
 *
 * @return Cola con los enemigos encolados, o NULL si falla la creacion.
 */
Cola generar_grupo(void)
{
    int cantidad = 1 + rand() % MAX_GRUPO;
    Cola grupo = cola_crear(cantidad);

    if (grupo == NULL)
        return NULL;

    printf("\n\t Un grupo de enemigos te bloquea el paso!\n\tEnemigos : \n");

    for (int i = 1; i <= cantidad; i++)
    {
        s_Enemigo enemigo = enemigo_aleatorio(i);

        if (cola_encolar(grupo, enemigo) != 0)
        {
            cola_destruir(grupo);
            return NULL;
        }
        printf(" %d. %s (%d PV)\n", enemigo.id, enemigo.nombre, enemigo.vida);
    }
    printf("Siguiente enemigo: %s\n", cola_frente(grupo)->nombre);
    return grupo;
}

/**
 * @brief Muestra el estado actual del combate.
 * Incluye stats del personaje, enemigo actual y enemigos en espera.
 *
 * @param gs       Puntero al estado del juego (no NULL).
 * @param enemigo  Puntero al enemigo actual (no NULL).
 * @param enEspera Numero de enemigos restantes en la cola (sin contar el actual).
 */
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

/**
 * @brief El personaje ataca al enemigo actual.
 * Usa calcular_danho con la defensa del enemigo.
 *
 * @param gs      Puntero al estado del juego (no NULL).
 * @param enemigo Puntero al enemigo objetivo (no NULL).
 */
void atacar_enemigo(const s_GameState *gs, s_Enemigo *enemigo)
{
    bool critico;
    int danho = calcular_danho(gs->personaje.ataque, enemigo->defensa, &critico);

    enemigo->vida -= danho;
    if (enemigo->vida < 0)
        enemigo->vida = 0;

    if (critico)
        printf("!Golpe critico!\n");
    printf("Atacas a %s: %d de danho. %s: %d/%d PV.\n",
           enemigo->nombre, danho, enemigo->nombre, enemigo->vida, enemigo->vidaMax);
}

/**
 * @brief Devuelve el indice (0-based) de una posicion en la lista del inventario.
 * Se usa para guardar la posicion en el historial.
 *
 * @param I Lista del inventario (cabeza, no NULL).
 * @param P Posicion a buscar (no NULL, distinta de la cabeza).
 * @return Indice de la posicion, o -1 si no se encuentra o parametros invalidos.
 */
static int obtener_posicion_inventario(Lista I, Posicion P)
{
    int pos = 0;
    if (I == NULL )
    {
	fprintf(stderr, "error: lista NULL\n");
        return -1;
    }
    if ( P == NULL )
    {
	fprintf(stderr, "error: Posicion NULL \n");
        return -1;
    }
    if ( P == I)
    {
	fprintf(stderr, "error: lista y posicion son iguales\n");
        return -1;
    }

    Posicion aux = I->next;

    while (aux != NULL && aux != P)
    {
        pos++;
        aux = aux->next;
    }

    if (aux == P)
        return pos;

    return -1;
}

/**
 * @brief Registra el descarte de una carta en el historial durante el combate.
 * Permite deshacer la accion si es necesario.
 *
 * @param gs  Puntero al estado del juego.
 * @param pos Posicion de la carta descartada en el inventario.
 */
static void registrar_descarte_combate(s_GameState *gs, Posicion pos)
{
    if (gs->Historial != NULL)
    {
        s_Reversion rev;
        rev.carta = pos->carta;
        rev.posicion = obtener_posicion_inventario(gs->inventario, pos);
        historial_registrar(gs->Historial, "Descartar carta en combate", A_DESCARTE, &rev);
    }
}

/**
 * @brief El enemigo ataca al personaje.
 * Calcula dano usando la defensa del personaje.
 *
 * @param gs      Puntero al estado del juego (no NULL).
 * @param enemigo Puntero al enemigo que ataca (no NULL).
 */
static void turno_enemigo(s_GameState *gs, s_Enemigo *enemigo)
{
    bool critico;
    int danho;

    if (gs == NULL)
    {
	fprintf(stderr, "error: el estado de juego es nulo\n");
        return;
    }

    if(enemigo == NULL)
    {
	fprintf(stderr, "error: enemigo NULL\n");
        return;
    }


    danho = calcular_danho(enemigo->ataque, gs->personaje.defensa, &critico);

    gs->personaje.vida -= danho;
    if (gs->personaje.vida < 0)
        gs->personaje.vida = 0;

    if (critico)
        printf("!Golpe critico del enemigo!\n");

    printf("%s ataca a %s: %d de danho. %s: %d/%d PV.\n",
           enemigo->nombre, gs->personaje.nombre, danho,
           gs->personaje.nombre, gs->personaje.vida, gs->personaje.vidaMax);
}

/**
 * @brief Permite al jugador usar una carta del inventario durante el combate.
 * Las cartas de curacion y vida maxima se consumen.
 * Las de ataque/defensa se equipan (aumentan stats permanentemente).
 *
 * @param gs             Puntero al estado del juego.
 * @param enemigo_actual Puntero al enemigo actual (no usado directamente, se pasa por consistencia).
 */
static void usar_carta_en_combate(s_GameState *gs, s_Enemigo *enemigo_actual)
{
    Posicion pos = NULL;
    int idx;

    if (gs == NULL || gs->inventario == NULL)
        return;

    if (gs->inventario->next == NULL)
    {
        printf("\nEl inventario esta vacio.\n");
        return;
    }

    imprimir_cartas(gs->inventario);
    printf("\nIngrese el numero de la carta a usar: ");
    idx = leer_entero_en_rango(1, cantidad_cartas(gs->inventario));

    pos = encontrar_carta(gs->inventario, idx);
    if (pos == NULL)
    {
        printf("Carta no encontrada.\n");
        return;
    }
    if(!confirmar_uso_carta(pos))
    {
        printf("\nUso de carta cancelado\n");
        return;
    }

    s_Carta *carta = &pos->carta;

    switch (carta->tipo)
    {
        case CURACION:
            gs->personaje.vida += carta->valor;
            if (gs->personaje.vida > gs->personaje.vidaMax)
                gs->personaje.vida = gs->personaje.vidaMax;
            printf("\nUsas %s y recuperas %d de vida. Vida actual: %d/%d\n",
                   carta->nombre, carta->valor, gs->personaje.vida, gs->personaje.vidaMax);
            registrar_descarte_combate(gs, pos);
            eliminar_carta(gs->inventario, pos);
            break;

        case VIDAMAXIMA:
            gs->personaje.vidaMax += carta->valor;
            gs->personaje.vida = gs->personaje.vidaMax;
            printf("\nUsas %s y aumentas tu vida maxima en %d. Vida maxima: %d\n",
                   carta->nombre, carta->valor, gs->personaje.vidaMax);
            registrar_descarte_combate(gs, pos);
            eliminar_carta(gs->inventario, pos);
            break;

        case ATAQUE:
            gs->personaje.ataque += carta->valor;
            printf("\nEquipas %s y aumentas tu ataque en %d. Ataque actual: %d\n",
                   carta->nombre, carta->valor, gs->personaje.ataque);
            eliminar_carta(gs->inventario, pos);
            break;

        case DEFENSA:
            gs->personaje.defensa += carta->valor;
            printf("\nEquipas %s y aumentas tu defensa en %d. Defensa actual: %d\n",
                   carta->nombre, carta->valor, gs->personaje.defensa);
            eliminar_carta(gs->inventario, pos);
            break;

        case ESPECIAL:
            printf("\n%s es una carta especial. Efecto: ", carta->nombre);
            if (carta->valor == 0)
            {
                printf("Efecto especial activado (pendiente de implementar).\n");
            }
            eliminar_carta(gs->inventario, pos);
            break;
    }
}

/**
 * @brief Orquesta el bucle de combate contra un grupo de enemigos.
 * Usa una cola (FIFO): el primer enemigo en entrar es el primero en luchar.
 *
 * @param gs Puntero al estado del juego (no NULL).
 */
void combate(s_GameState *gs)
{
    Cola grupo = NULL;
    s_Enemigo *enemigo_actual = NULL;
    int opcion;
    bool fin_combate = false;
    bool personaje_muerto = false;

    if (gs == NULL)
    {
        fprintf(stderr, "error: estado de juego invalido (NULL)\n");
        return;
    }

    grupo = generar_grupo();
    if (grupo == NULL)
    {
        fprintf(stderr, "error: no se pudo crear el grupo de enemigos\n");
        return;
    }

    while (!fin_combate && gs->ejecutando)
    {
        if (cola_esta_vacia(grupo))
        {
            printf("\n!Has derrotado a todos los enemigos!\n");
            fin_combate = true;
            break;
        }

        enemigo_actual = cola_frente(grupo);
        if (enemigo_actual == NULL)
        {
            fprintf(stderr, "error: cola vacia inesperadamente\n");
            break;
        }

        int en_espera = cola_tamanho(grupo) - 1;
        mostrar_estado(gs, enemigo_actual, en_espera);

        opcion = leer_entero_en_rango(1, 3);

        switch (opcion)
        {
            case 1:
                atacar_enemigo(gs, enemigo_actual);

                if (enemigo_actual->vida <= 0)
                {
                    printf("\n!Has derrotado a %s!\n", enemigo_actual->nombre);
                    if (cola_desencolar(grupo) != 0)
                    {
                        fprintf(stderr, "error: fallo al desencolar enemigo muerto\n");
                    }
                }
                else
                {
                    turno_enemigo(gs, enemigo_actual);
                    if (gs->personaje.vida <= 0)
                    {
                        gs->personaje.vida = 0;
                        personaje_muerto = true;
                        fin_combate = true;
                    }
                }
                break;

            case 2:
                imprimir_cartas(gs->inventario);
                break;

            case 3:
                usar_carta_en_combate(gs, enemigo_actual);
                if (!cola_esta_vacia(grupo) && enemigo_actual->vida > 0)
                {
                    turno_enemigo(gs, enemigo_actual);
                    if (gs->personaje.vida <= 0)
                    {
                        gs->personaje.vida = 0;
                        personaje_muerto = true;
                        fin_combate = true;
                    }
                }
                break;
        }
    }

    cola_destruir(grupo);
    grupo = NULL;

    if (personaje_muerto)
    {
        printf("\n%s ha caido en combate... GAME OVER\n", gs->personaje.nombre);
        gs->ejecutando = false;
    }
}