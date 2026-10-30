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

    ui_titulo("EMBOSCADA", COLOR_ROJO);
    ui_mensaje_aviso("Un grupo de enemigos te bloquea el paso!");
    ui_pausa_ms(PAUSA_MEDIA);
    printf("\nEnemigos:\n");

    for (int i = 1; i <= cantidad; i++)
    {
        s_Enemigo enemigo = enemigo_aleatorio(i);

        if (cola_encolar(grupo, enemigo) != 0)
        {
            cola_destruir(grupo);
            return NULL;
        }
        printf("  %s%d.%s %s (%d PV)\n", COLOR_ROJO, enemigo.id, COLOR_RESET,
               enemigo.nombre, enemigo.vida);
        ui_pausa_ms(PAUSA_CORTA);
    }

    printf("\n");
    ui_mensaje_info("Siguiente enemigo: %s", cola_frente(grupo)->nombre);
    ui_pausa_ms(PAUSA_LARGA);
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
    printf("\n");
    ui_titulo("COMBATE", COLOR_ROJO);

    printf("%s%s%s\n  ", COLOR_VERDE, gs->personaje.nombre, COLOR_RESET);
    ui_barra_vida(gs->personaje.vida, gs->personaje.vidaMax, ANCHO_BARRA);
    printf("  ATK %d | DEF %d\n\n", gs->personaje.ataque, gs->personaje.defensa);

    printf("%s%s%s\n  ", COLOR_ROJO, enemigo->nombre, COLOR_RESET);
    ui_barra_vida(enemigo->vida, enemigo->vidaMax, ANCHO_BARRA);
    printf("\n  Enemigos en espera: %d\n\n", enEspera);

    ui_opcion_menu(1, "Atacar");
    ui_opcion_menu(2, "Ver inventario");
    ui_opcion_menu(3, "Usar carta");
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

    printf("\n");
    if (critico)
        ui_mensaje_aviso("!Golpe critico!");
    printf("%sAtacas a %s:%s %d de danho. %s: %d/%d PV.\n",COLOR_VERDE, enemigo->nombre, COLOR_RESET, danho,
           enemigo->nombre, enemigo->vida, enemigo->vidaMax);
    ui_pausa_ms(PAUSA_MEDIA);
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
        ui_mensaje_aviso("!Golpe critico del enemigo!");

    printf("%s%s ataca a %s:%s %d de danho. %s: %d/%d PV.\n",
           COLOR_ROJO, enemigo->nombre, gs->personaje.nombre, COLOR_RESET, danho,
           gs->personaje.nombre, gs->personaje.vida, gs->personaje.vidaMax);
    ui_pausa_ms(PAUSA_MEDIA);   
}

/**
 * @brief Permite al jugador usar una carta del inventario durante el combate.
 * Las cartas de curacion y vida maxima se consumen.
 * Las de ataque/defensa se equipan (aumentan stats permanentemente).
 *
 * @param gs             Puntero al estado del juego.
 */
bool usar_carta_en_combate(s_GameState *gs)
{
    Posicion pos = NULL;
    int idx;

    if (gs == NULL || gs->inventario == NULL)
        return false;

    if (gs->inventario->next == NULL)
    {
        ui_mensaje_aviso("El inventario esta vacio.");
        return false; 
    }

    imprimir_cartas(gs->inventario);
    printf("\nIngrese el numero de la carta a usar: ");
    idx = leer_entero_en_rango(1, cantidad_cartas(gs->inventario));

    pos = encontrar_carta(gs->inventario, idx);
    if (pos == NULL)
    {
        ui_mensaje_aviso("Carta no encontrada.");
        return false;
    }
    if(!confirmar_uso_carta(pos))
    {
        ui_mensaje_info("Uso de carta cancelado");
        return false;
    }

    s_Carta *carta = &pos->carta;
    char descripcion[128];

    /* El texto se arma antes de usar la carta: al eliminarla del inventario
     * el nodo (y su nombre) se liberan. */
    snprintf(descripcion, sizeof(descripcion), "El personaje usó %s",
             carta->nombre ? carta->nombre : "una carta");

    switch (carta->tipo)
    {
        case CURACION:
            if(carta->valor == 0)
            {
                gs->personaje.vida = gs->personaje.vidaMax;
                ui_mensaje_exito("Usas %s y recuperas toda tu vida.", carta->nombre);
            }
            else
            {
                gs->personaje.vida += carta->valor;

                if(gs->personaje.vida > gs->personaje.vidaMax)
                    gs->personaje.vida = gs->personaje.vidaMax;

                ui_mensaje_exito("Usas %s y recuperas %d puntos de vida.", carta->nombre, carta->valor);
            }

            printf("Vida actual: ");
            ui_barra_vida(gs->personaje.vida, gs->personaje.vidaMax, ANCHO_BARRA);
            printf("\n");

            eliminar_carta(gs->inventario, pos);
            break;

        case VIDAMAXIMA:
            gs->personaje.vidaMax += carta->valor;
            gs->personaje.vida = gs->personaje.vidaMax;
            ui_mensaje_exito("Usas %s y aumentas tu vida maxima en %d. Vida maxima: %d",
                   carta->nombre, carta->valor, gs->personaje.vidaMax);
            eliminar_carta(gs->inventario, pos);
            break;

        case ATAQUE:
            gs->personaje.ataque += carta->valor;
            ui_mensaje_exito("Equipas %s y aumentas tu ataque en %d. Ataque actual: %d",
                   carta->nombre, carta->valor, gs->personaje.ataque);
            eliminar_carta(gs->inventario, pos);
            break;

        case DEFENSA:
            gs->personaje.defensa += carta->valor;
            ui_mensaje_exito("Equipas %s y aumentas tu defensa en %d. Defensa actual: %d",
                   carta->nombre, carta->valor, gs->personaje.defensa);
            eliminar_carta(gs->inventario, pos);
            break;

        default:
            ui_mensaje_aviso("Carta no disponible");
            return false;
    }

    ui_pausa_ms(PAUSA_MEDIA);
    if(gs->Historial != NULL)
        historial_registrar(gs->Historial, descripcion, A_CONSUMIBLE, NULL);

    return true;
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

    ui_limpiar_pantalla();
    grupo = generar_grupo();
    if (grupo == NULL)
    {
        ui_mensaje_error("No se pudo crear el grupo de enemigos");
        return;
    }

    historial_registrar(gs->Historial, "Inicio de combate", A_COMBATE_I, NULL);

    while (!fin_combate && gs->ejecutando)
    {
        if (cola_esta_vacia(grupo))
        {
            printf("\n");
            ui_mensaje_exito("!Has derrotado a todos los enemigos!");
            ui_esperar_enter();
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
                    ui_mensaje_exito("!Has derrotado a %s!", enemigo_actual->nombre);
                    char descripcion[128];
                    snprintf(descripcion, sizeof(descripcion), "Se derrotó al enemigo %s",
                             enemigo_actual->nombre ? enemigo_actual->nombre : "desconocido");
                    historial_registrar(gs->Historial, descripcion, A_REGISTRO, NULL);
                    if (cola_desencolar(grupo) != 0)
                    {
                        fprintf(stderr, "error: fallo al desencolar enemigo muerto\n");
                    }
                    else if (!cola_esta_vacia(grupo))
                    {
                        ui_mensaje_info("Siguiente enemigo: %s", cola_frente(grupo)->nombre);
                    }
                    ui_pausa_ms(PAUSA_MEDIA);
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
                if(usar_carta_en_combate(gs) &&
                !cola_esta_vacia(grupo) && enemigo_actual->vida > 0)
                {
                    turno_enemigo(gs, enemigo_actual);

                    if(gs->personaje.vida <= 0)
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

    historial_registrar(gs->Historial, "Fin de combate", A_FIN_COMBATE, NULL);

    if (personaje_muerto)
    {
        gs->ejecutando = false;
        ui_pantalla_game_over(gs->personaje.nombre);
    }
}