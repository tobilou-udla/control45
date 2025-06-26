#ifndef CALL_CENTER_H
#define CALL_CENTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Enumeraciones para estados y tipos
typedef enum {
    DISPONIBLE,
    OCUPADO,
    DESCANSO,
    DESCONECTADO
} EstadoOperador;

typedef enum {
    BAJA,
    MEDIA,
    ALTA,
    URGENTE
} PrioridadLlamada;

typedef enum {
    SOPORTE_TECNICO,
    VENTAS,
    ATENCION_CLIENTE,
    RECLAMOS,
    GENERAL
} TipoLlamada;

typedef enum {
    PENDIENTE,
    EN_CURSO,
    FINALIZADA,
    ABANDONADA
} EstadoLlamada;

// Estructuras principales
struct Operador {
    int id;
    char nombre[50];
    char especialidad[30];
    EstadoOperador estado;
    int llamadas_activas;
    int llamadas_totales;
    int tiempo_total_llamadas; // en minutos
    time_t ultima_llamada;
    float calificacion_promedio;
};

struct Llamada {
    int id;
    int operador_id;
    TipoLlamada tipo;
    PrioridadLlamada prioridad;
    EstadoLlamada estado;
    time_t inicio;
    time_t fin;
    int duracion; // en minutos
    char descripcion[200];
    float calificacion_cliente;
};

struct EstadisticasOperador {
    int operador_id;
    char nombre[50];
    int llamadas_atendidas;
    int tiempo_total;
    float tiempo_promedio;
    float calificacion_promedio;
    float porcentaje_carga;
};

struct SistemaDistribucion {
    struct Operador operadores[50];
    struct Llamada llamadas[1000];
    int num_operadores;
    int num_llamadas;
    int llamadas_en_cola;
};

// Funciones del sistema
void inicializar_sistema(struct SistemaDistribucion *sistema);
int menu_call_center();

// Gestión de operadores
void crear_operadores(struct SistemaDistribucion *sistema);
void agregar_operador(struct SistemaDistribucion *sistema);
void modificar_estado_operador(struct SistemaDistribucion *sistema);
void listar_operadores(struct SistemaDistribucion *sistema);

// Gestión de llamadas
void recibir_llamada(struct SistemaDistribucion *sistema);
int distribuir_llamada(struct SistemaDistribucion *sistema, struct Llamada *llamada);
void finalizar_llamada(struct SistemaDistribucion *sistema);
void listar_llamadas(struct SistemaDistribucion *sistema);

// Algoritmos de distribución
int encontrar_mejor_operador(struct SistemaDistribucion *sistema, TipoLlamada tipo, PrioridadLlamada prioridad);
int operador_por_especialidad(struct SistemaDistribucion *sistema, TipoLlamada tipo);
int operador_menor_carga(struct SistemaDistribucion *sistema);
int operador_disponible(struct SistemaDistribucion *sistema);

// Monitoreo y estadísticas
void mostrar_estadisticas_general(struct SistemaDistribucion *sistema);
void mostrar_estadisticas_operador(struct SistemaDistribucion *sistema);
void generar_reporte_carga(struct SistemaDistribucion *sistema);
void detectar_sobrecarga(struct SistemaDistribucion *sistema);

// Persistencia de datos
void guardar_operadores(struct Operador operadores[], int num_operadores);
int cargar_operadores(struct Operador operadores[], int *num_operadores);
void guardar_llamadas(struct Llamada llamadas[], int num_llamadas);
int cargar_llamadas(struct Llamada llamadas[], int *num_llamadas);

// Utilidades
void leer_cadena_call(char *cadena, int num);
const char* estado_operador_texto(EstadoOperador estado);
const char* tipo_llamada_texto(TipoLlamada tipo);
const char* prioridad_llamada_texto(PrioridadLlamada prioridad);
const char* estado_llamada_texto(EstadoLlamada estado);

#endif
