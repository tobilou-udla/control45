#include "call_center.h"
#include <stdio.h>
int main() {
    struct SistemaDistribucion sistema;
    inicializar_sistema(&sistema);
    
    printf("=== SISTEMA DE DISTRIBUCIÓN INTELIGENTE DE LLAMADAS ===\n");
    printf("Bienvenido al sistema de gestión de centro de llamadas\n");
    
    int opcion;
    do {
        opcion = menu_call_center();
        
        switch (opcion) {
            case 1:
                crear_operadores(&sistema);
                break;
            case 2:
                agregar_operador(&sistema);
                break;
            case 3:
                listar_operadores(&sistema);
                break;
            case 4:
                // Implementar modificación de estado
                printf("Funcionalidad en desarrollo.\n");
                break;
            case 5:
                recibir_llamada(&sistema);
                break;
            case 6:
                finalizar_llamada(&sistema);
                break;
            case 7:
                // Implementar listado de llamadas
                printf("Funcionalidad en desarrollo.\n");
                break;
            case 8:
                mostrar_estadisticas_general(&sistema);
                break;
            case 9:
                // Implementar estadísticas por operador
                printf("Funcionalidad en desarrollo.\n");
                break;
            case 10:
                generar_reporte_carga(&sistema);
                break;
            case 11:
                detectar_sobrecarga(&sistema);
                break;
            case 12:
                printf("Gracias por usar el sistema.\n");
                break;
            default:
                printf("Opción no válida.\n");
                break;
        }
        
        if (opcion != 12) {
            printf("\nPresione Enter para continuar...");
            getchar();
        }
        
    } while (opcion != 12);
    
    return 0;
}
