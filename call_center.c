#include "call_center.h"

void inicializar_sistema(struct SistemaDistribucion *sistema) {
    sistema->num_operadores = 0;
    sistema->num_llamadas = 0;
    sistema->llamadas_en_cola = 0;
    
    // Cargar datos existentes
    cargar_operadores(sistema->operadores, &sistema->num_operadores);
    cargar_llamadas(sistema->llamadas, &sistema->num_llamadas);
}

int menu_call_center() {
    int opc;
    printf("\n=== SISTEMA DE DISTRIBUCIÓN INTELIGENTE DE LLAMADAS ===\n");
    printf("========================================================\n");
    printf("1.  Crear Operadores Predeterminados\n");
    printf("2.  Agregar Operador\n");
    printf("3.  Listar Operadores\n");
    printf("4.  Modificar Estado de Operador\n");
    printf("5.  Recibir Nueva Llamada\n");
    printf("6.  Finalizar Llamada\n");
    printf("7.  Listar Llamadas\n");
    printf("8.  Estadísticas Generales\n");
    printf("9.  Estadísticas por Operador\n");
    printf("10. Reporte de Carga de Trabajo\n");
    printf("11. Detectar Sobrecarga\n");
    printf("12. Salir\n");
    printf("Seleccione una opción: ");
    scanf("%d", &opc);
    fflush(stdin);
    return opc;
}

void crear_operadores(struct SistemaDistribucion *sistema) {
    struct Operador operadores[] = {
        {1, "Ana García", "Soporte Técnico", DISPONIBLE, 0, 0, 0, 0, 4.5},
        {2, "Carlos López", "Ventas", DISPONIBLE, 0, 0, 0, 0, 4.2},
        {3, "María Rodríguez", "Atención Cliente", DISPONIBLE, 0, 0, 0, 0, 4.8},
        {4, "Juan Pérez", "Reclamos", DISPONIBLE, 0, 0, 0, 0, 4.0},
        {5, "Laura Martínez", "General", DISPONIBLE, 0, 0, 0, 0, 4.3},
        {6, "Pedro Sánchez", "Soporte Técnico", DISPONIBLE, 0, 0, 0, 0, 4.6},
        {7, "Carmen Díaz", "Ventas", DISPONIBLE, 0, 0, 0, 0, 4.1},
        {8, "Roberto Silva", "Atención Cliente", DISPONIBLE, 0, 0, 0, 0, 4.7},
        {9, "Andrea Torres", "General", DISPONIBLE, 0, 0, 0, 0, 4.4},
        {10, "Miguel Herrera", "Reclamos", DISPONIBLE, 0, 0, 0, 0, 3.9}
    };
    
    for (int i = 0; i < 10; i++) {
        sistema->operadores[i] = operadores[i];
    }
    sistema->num_operadores = 10;
    
    guardar_operadores(sistema->operadores, sistema->num_operadores);
    printf("Operadores creados y guardados correctamente.\n");
}

void agregar_operador(struct SistemaDistribucion *sistema) {
    if (sistema->num_operadores >= 50) {
        printf("Máximo número de operadores alcanzado.\n");
        return;
    }
    
    struct Operador nuevo;
    nuevo.id = sistema->num_operadores + 1;
    
    printf("Nombre del operador: ");
    leer_cadena_call(nuevo.nombre, sizeof(nuevo.nombre));
    
    printf("Especialidad (Soporte Técnico/Ventas/Atención Cliente/Reclamos/General): ");
    leer_cadena_call(nuevo.especialidad, sizeof(nuevo.especialidad));
    
    nuevo.estado = DISPONIBLE;
    nuevo.llamadas_activas = 0;
    nuevo.llamadas_totales = 0;
    nuevo.tiempo_total_llamadas = 0;
    nuevo.ultima_llamada = 0;
    nuevo.calificacion_promedio = 0.0;
    
    sistema->operadores[sistema->num_operadores] = nuevo;
    sistema->num_operadores++;
    
    guardar_operadores(sistema->operadores, sistema->num_operadores);
    printf("Operador agregado correctamente.\n");
}

void listar_operadores(struct SistemaDistribucion *sistema) {
    printf("\n=== LISTA DE OPERADORES ===\n");
    printf("ID | Nombre            | Especialidad      | Estado      | Llamadas | Calificación\n");
    printf("---|-------------------|-------------------|-------------|----------|-------------\n");
    
    for (int i = 0; i < sistema->num_operadores; i++) {
        printf("%-2d | %-17s | %-17s | %-11s | %-8d | %.1f\n",
               sistema->operadores[i].id,
               sistema->operadores[i].nombre,
               sistema->operadores[i].especialidad,
               estado_operador_texto(sistema->operadores[i].estado),
               sistema->operadores[i].llamadas_totales,
               sistema->operadores[i].calificacion_promedio);
    }
    
    if (sistema->num_operadores == 0) {
        printf("No hay operadores registrados.\n");
    }
}

void recibir_llamada(struct SistemaDistribucion *sistema) {
    if (sistema->num_llamadas >= 1000) {
        printf("Máximo número de llamadas alcanzado.\n");
        return;
    }
    
    struct Llamada nueva;
    nueva.id = sistema->num_llamadas + 1;
    nueva.inicio = time(NULL);
    nueva.fin = 0;
    nueva.duracion = 0;
    nueva.calificacion_cliente = 0.0;
    nueva.estado = PENDIENTE;
    
    printf("Tipo de llamada:\n");
    printf("0. Soporte Técnico\n1. Ventas\n2. Atención Cliente\n3. Reclamos\n4. General\n");
    printf("Seleccione: ");
    int tipo;
    scanf("%d", &tipo);
    nueva.tipo = (TipoLlamada)tipo;
    
    printf("Prioridad:\n");
    printf("0. Baja\n1. Media\n2. Alta\n3. Urgente\n");
    printf("Seleccione: ");
    int prioridad;
    scanf("%d", &prioridad);
    nueva.prioridad = (PrioridadLlamada)prioridad;
    
    printf("Descripción: ");
    fflush(stdin);
    leer_cadena_call(nueva.descripcion, sizeof(nueva.descripcion));
    
    // Intentar distribuir la llamada
    int operador_asignado = distribuir_llamada(sistema, &nueva);
    
    if (operador_asignado != -1) {
        nueva.operador_id = operador_asignado;
        nueva.estado = EN_CURSO;
        sistema->operadores[operador_asignado - 1].llamadas_activas++;
        sistema->operadores[operador_asignado - 1].llamadas_totales++;
        sistema->operadores[operador_asignado - 1].estado = OCUPADO;
        sistema->operadores[operador_asignado - 1].ultima_llamada = time(NULL);
        
        printf("Llamada asignada al operador %s (ID: %d)\n", 
               sistema->operadores[operador_asignado - 1].nombre, operador_asignado);
    } else {
        nueva.operador_id = 0;
        sistema->llamadas_en_cola++;
        printf("No hay operadores disponibles. Llamada en cola.\n");
    }
    
    sistema->llamadas[sistema->num_llamadas] = nueva;
    sistema->num_llamadas++;
    
    guardar_llamadas(sistema->llamadas, sistema->num_llamadas);
    guardar_operadores(sistema->operadores, sistema->num_operadores);
}

int distribuir_llamada(struct SistemaDistribucion *sistema, struct Llamada *llamada) {
    int mejor_operador = encontrar_mejor_operador(sistema, llamada->tipo, llamada->prioridad);
    return mejor_operador;
}

int encontrar_mejor_operador(struct SistemaDistribucion *sistema, TipoLlamada tipo, PrioridadLlamada prioridad) {
    // Para llamadas urgentes, buscar cualquier operador disponible
    if (prioridad == URGENTE) {
        int operador = operador_disponible(sistema);
        if (operador != -1) return operador;
    }
    
    // Buscar operador especializado disponible
    int operador_esp = operador_por_especialidad(sistema, tipo);
    if (operador_esp != -1) return operador_esp;
    
    // Buscar operador con menor carga
    int operador_carga = operador_menor_carga(sistema);
    if (operador_carga != -1) return operador_carga;
    
    return -1; // No hay operadores disponibles
}

int operador_por_especialidad(struct SistemaDistribucion *sistema, TipoLlamada tipo) {
    const char *especialidad_requerida;
    
    switch (tipo) {
        case SOPORTE_TECNICO:
            especialidad_requerida = "Soporte Técnico";
            break;
        case VENTAS:
            especialidad_requerida = "Ventas";
            break;
        case ATENCION_CLIENTE:
            especialidad_requerida = "Atención Cliente";
            break;
        case RECLAMOS:
            especialidad_requerida = "Reclamos";
            break;
        default:
            especialidad_requerida = "General";
            break;
    }
    
    for (int i = 0; i < sistema->num_operadores; i++) {
        if (sistema->operadores[i].estado == DISPONIBLE &&
            strcmp(sistema->operadores[i].especialidad, especialidad_requerida) == 0) {
            return sistema->operadores[i].id;
        }
    }
    
    return -1;
}

int operador_menor_carga(struct SistemaDistribucion *sistema) {
    int mejor_operador = -1;
    int menor_carga = 999999;
    
    for (int i = 0; i < sistema->num_operadores; i++) {
        if (sistema->operadores[i].estado == DISPONIBLE &&
            sistema->operadores[i].llamadas_activas < menor_carga) {
            menor_carga = sistema->operadores[i].llamadas_activas;
            mejor_operador = sistema->operadores[i].id;
        }
    }
    
    return mejor_operador;
}

int operador_disponible(struct SistemaDistribucion *sistema) {
    for (int i = 0; i < sistema->num_operadores; i++) {
        if (sistema->operadores[i].estado == DISPONIBLE) {
            return sistema->operadores[i].id;
        }
    }
    return -1;
}

void finalizar_llamada(struct SistemaDistribucion *sistema) {
    printf("ID de la llamada a finalizar: ");
    int id_llamada;
    scanf("%d", &id_llamada);
    
    // Buscar la llamada
    int indice_llamada = -1;
    for (int i = 0; i < sistema->num_llamadas; i++) {
        if (sistema->llamadas[i].id == id_llamada && sistema->llamadas[i].estado == EN_CURSO) {
            indice_llamada = i;
            break;
        }
    }
    
    if (indice_llamada == -1) {
        printf("Llamada no encontrada o ya finalizada.\n");
        return;
    }
    
    // Finalizar llamada
    sistema->llamadas[indice_llamada].fin = time(NULL);
    sistema->llamadas[indice_llamada].duracion = 
        (sistema->llamadas[indice_llamada].fin - sistema->llamadas[indice_llamada].inicio) / 60;
    sistema->llamadas[indice_llamada].estado = FINALIZADA;
    
    printf("Calificación del cliente (1-5): ");
    scanf("%f", &sistema->llamadas[indice_llamada].calificacion_cliente);
    
    // Actualizar operador
    int operador_id = sistema->llamadas[indice_llamada].operador_id;
    if (operador_id > 0) {
        sistema->operadores[operador_id - 1].llamadas_activas--;
        sistema->operadores[operador_id - 1].tiempo_total_llamadas += sistema->llamadas[indice_llamada].duracion;
        
        if (sistema->operadores[operador_id - 1].llamadas_activas == 0) {
            sistema->operadores[operador_id - 1].estado = DISPONIBLE;
        }
        
        // Actualizar calificación promedio
        float total_calificacion = sistema->operadores[operador_id - 1].calificacion_promedio * 
                                  (sistema->operadores[operador_id - 1].llamadas_totales - 1);
        total_calificacion += sistema->llamadas[indice_llamada].calificacion_cliente;
        sistema->operadores[operador_id - 1].calificacion_promedio = 
            total_calificacion / sistema->operadores[operador_id - 1].llamadas_totales;
    }
    
    printf("Llamada finalizada correctamente.\n");
    
    guardar_llamadas(sistema->llamadas, sistema->num_llamadas);
    guardar_operadores(sistema->operadores, sistema->num_operadores);
}

void mostrar_estadisticas_general(struct SistemaDistribucion *sistema) {
    printf("\n=== ESTADÍSTICAS GENERALES ===\n");
    
    int llamadas_pendientes = 0, llamadas_en_curso = 0, llamadas_finalizadas = 0;
    int tiempo_total = 0;
    float calificacion_total = 0.0;
    int llamadas_calificadas = 0;
    
    for (int i = 0; i < sistema->num_llamadas; i++) {
        switch (sistema->llamadas[i].estado) {
            case PENDIENTE:
                llamadas_pendientes++;
                break;
            case EN_CURSO:
                llamadas_en_curso++;
                break;
            case FINALIZADA:
                llamadas_finalizadas++;
                tiempo_total += sistema->llamadas[i].duracion;
                if (sistema->llamadas[i].calificacion_cliente > 0) {
                    calificacion_total += sistema->llamadas[i].calificacion_cliente;
                    llamadas_calificadas++;
                }
                break;
            default:
                break;
        }
    }
    
    printf("Total de llamadas: %d\n", sistema->num_llamadas);
    printf("Llamadas pendientes: %d\n", llamadas_pendientes);
    printf("Llamadas en curso: %d\n", llamadas_en_curso);
    printf("Llamadas finalizadas: %d\n", llamadas_finalizadas);
    
    if (llamadas_finalizadas > 0) {
        printf("Tiempo promedio de atención: %.1f minutos\n", 
               (float)tiempo_total / llamadas_finalizadas);
    }
    
    if (llamadas_calificadas > 0) {
        printf("Calificación promedio: %.1f/5.0\n", 
               calificacion_total / llamadas_calificadas);
    }
    
    // Estadísticas de operadores
    int operadores_disponibles = 0, operadores_ocupados = 0;
    for (int i = 0; i < sistema->num_operadores; i++) {
        if (sistema->operadores[i].estado == DISPONIBLE) {
            operadores_disponibles++;
        } else if (sistema->operadores[i].estado == OCUPADO) {
            operadores_ocupados++;
        }
    }
    
    printf("\nOperadores disponibles: %d\n", operadores_disponibles);
    printf("Operadores ocupados: %d\n", operadores_ocupados);
}

void generar_reporte_carga(struct SistemaDistribucion *sistema) {
    printf("\n=== REPORTE DE CARGA DE TRABAJO ===\n");
    printf("Operador            | Llamadas | Tiempo Total | Promedio | Calificación | Estado\n");
    printf("--------------------|----------|--------------|----------|--------------|--------\n");
    
    for (int i = 0; i < sistema->num_operadores; i++) {
        float tiempo_promedio = 0.0;
        if (sistema->operadores[i].llamadas_totales > 0) {
            tiempo_promedio = (float)sistema->operadores[i].tiempo_total_llamadas / 
                             sistema->operadores[i].llamadas_totales;
        }
        
        printf("%-19s | %-8d | %-12d | %-8.1f | %-12.1f | %s\n",
               sistema->operadores[i].nombre,
               sistema->operadores[i].llamadas_totales,
               sistema->operadores[i].tiempo_total_llamadas,
               tiempo_promedio,
               sistema->operadores[i].calificacion_promedio,
               estado_operador_texto(sistema->operadores[i].estado));
    }
}

void detectar_sobrecarga(struct SistemaDistribucion *sistema) {
    printf("\n=== DETECCIÓN DE SOBRECARGA ===\n");
    
    if (sistema->num_operadores == 0) {
        printf("No hay operadores registrados.\n");
        return;
    }
    
    // Calcular promedio de llamadas
    int total_llamadas = 0;
    for (int i = 0; i < sistema->num_operadores; i++) {
        total_llamadas += sistema->operadores[i].llamadas_totales;
    }
    
    float promedio_llamadas = (float)total_llamadas / sistema->num_operadores;
    float umbral_sobrecarga = promedio_llamadas * 1.5; // 50% más que el promedio
    
    printf("Promedio de llamadas por operador: %.1f\n", promedio_llamadas);
    printf("Umbral de sobrecarga: %.1f\n", umbral_sobrecarga);
    printf("\nOperadores sobrecargados:\n");
    
    int sobrecargados = 0;
    for (int i = 0; i < sistema->num_operadores; i++) {
        if (sistema->operadores[i].llamadas_totales > umbral_sobrecarga) {
            printf("- %s: %d llamadas (%.1f%% sobre el promedio)\n",
                   sistema->operadores[i].nombre,
                   sistema->operadores[i].llamadas_totales,
                   ((float)sistema->operadores[i].llamadas_totales / promedio_llamadas - 1) * 100);
            sobrecargados++;
        }
    }
    
    if (sobrecargados == 0) {
        printf("No se detectaron operadores sobrecargados.\n");
    }
    
    // Operadores inactivos
    printf("\nOperadores con baja actividad:\n");
    int inactivos = 0;
    float umbral_inactividad = promedio_llamadas * 0.5; // 50% menos que el promedio
    
    for (int i = 0; i < sistema->num_operadores; i++) {
        if (sistema->operadores[i].llamadas_totales < umbral_inactividad) {
            printf("- %s: %d llamadas\n",
                   sistema->operadores[i].nombre,
                   sistema->operadores[i].llamadas_totales);
            inactivos++;
        }
    }
    
    if (inactivos == 0) {
        printf("No se detectaron operadores con baja actividad.\n");
    }
}

// Funciones de persistencia
void guardar_operadores(struct Operador operadores[], int num_operadores) {
    FILE *archivo = fopen("operadores.dat", "wb");
    if (archivo == NULL) {
        printf("Error al guardar operadores.\n");
        return;
    }
    fwrite(operadores, sizeof(struct Operador), num_operadores, archivo);
    fclose(archivo);
}

int cargar_operadores(struct Operador operadores[], int *num_operadores) {
    FILE *archivo = fopen("operadores.dat", "rb");
    if (archivo == NULL) {
        *num_operadores = 0;
        return 0;
    }
    *num_operadores = fread(operadores, sizeof(struct Operador), 50, archivo);
    fclose(archivo);
    return 1;
}

void guardar_llamadas(struct Llamada llamadas[], int num_llamadas) {
    FILE *archivo = fopen("llamadas.dat", "wb");
    if (archivo == NULL) {
        printf("Error al guardar llamadas.\n");
        return;
    }
    fwrite(llamadas, sizeof(struct Llamada), num_llamadas, archivo);
    fclose(archivo);
}

int cargar_llamadas(struct Llamada llamadas[], int *num_llamadas) {
    FILE *archivo = fopen("llamadas.dat", "rb");
    if (archivo == NULL) {
        *num_llamadas = 0;
        return 0;
    }
    *num_llamadas = fread(llamadas, sizeof(struct Llamada), 1000, archivo);
    fclose(archivo);
    return 1;
}

// Funciones de utilidad
void leer_cadena_call(char *cadena, int num) {
    fflush(stdin);
    fgets(cadena, num, stdin);
    int len = strlen(cadena) - 1;
    if (cadena[len] == '\n') {
        cadena[len] = '\0';
    }
}

const char* estado_operador_texto(EstadoOperador estado) {
    switch (estado) {
        case DISPONIBLE: return "Disponible";
        case OCUPADO: return "Ocupado";
        case DESCANSO: return "Descanso";
        case DESCONECTADO: return "Desconectado";
        default: return "Desconocido";
    }
}

const char* tipo_llamada_texto(TipoLlamada tipo) {
    switch (tipo) {
        case SOPORTE_TECNICO: return "Soporte Técnico";
        case VENTAS: return "Ventas";
        case ATENCION_CLIENTE: return "Atención Cliente";
        case RECLAMOS: return "Reclamos";
        case GENERAL: return "General";
        default: return "Desconocido";
    }
}

const char* prioridad_llamada_texto(PrioridadLlamada prioridad) {
    switch (prioridad) {
        case BAJA: return "Baja";
        case MEDIA: return "Media";
        case ALTA: return "Alta";
        case URGENTE: return "Urgente";
        default: return "Desconocida";
    }
}

const char* estado_llamada_texto(EstadoLlamada estado) {
    switch (estado) {
        case PENDIENTE: return "Pendiente";
        case EN_CURSO: return "En Curso";
        case FINALIZADA: return "Finalizada";
        case ABANDONADA: return "Abandonada";
        default: return "Desconocido";
    }
}
