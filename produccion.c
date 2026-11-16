#include <stdio.h>
#include <stdlib.h>

#define MAX_PROD 5
#define LONG_NOMBRE 30
#define LONG_LINEA 100

int longitudCadena(const char *s)
{
    int n = 0;
    while (*(s + n) != '\0')
    {
        n++;
    }
    return n;
}

char aMinuscula(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return (char)(c + ('a' - 'A'));
    }
    return c;
}

int sonIguales(const char *a, const char *b)
{
    while (*a != '\0' && *b != '\0')
    {
        if (aMinuscula(*a) != aMinuscula(*b))
        {
            return 0;
        }
        a++;
        b++;
    }
    return (*a == '\0' && *b == '\0');
}

void copiarNombre(char *destino, const char *origen)
{
    int i = 0;
    while (*(origen + i) != '\0' && i < LONG_NOMBRE - 1)
    {
        *(destino + i) = *(origen + i);
        i++;
    }
    *(destino + i) = '\0';
}

void recortar(char *s)
{
    int inicio = 0;
    int fin = longitudCadena(s) - 1;
    int i;

    while (fin >= 0 && (*(s + fin) == ' ' || *(s + fin) == '\n' ||*(s + fin) == '\r' || *(s + fin) == '\t'))
    {
        fin--;
    }
    while (inicio <= fin && (*(s + inicio) == ' ' || *(s + inicio) == '\t'))
    {
        inicio++;
    }
    for (i = 0; inicio + i <= fin; i++)
    {
        *(s + i) = *(s + inicio + i);
    }
    *(s + i) = '\0';
}

static void descartarResto(const char *linea)
{
    int n = longitudCadena(linea);
    if (n > 0 && linea[n - 1] != '\n')
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }
    }
}

int leerEntero(const char *mensaje, int min, int max)
{
    char linea[LONG_LINEA];
    char *fin;
    long valor;

    while (1)
    {
        printf("%s", mensaje);
        if (fgets(linea, LONG_LINEA, stdin) == NULL)
        {
            exit(0);
        }
        descartarResto(linea);
        valor = strtol(linea, &fin, 10);
        if (fin == linea)
        {
            printf("  Entrada invalida. Ingrese un numero entero.\n");
            continue;
        }
        while (*fin == ' ' || *fin == '\n' || *fin == '\r')
        {
            fin++;
        }
        if (*fin != '\0')
        {
            printf("  Entrada invalida. Ingrese solo un numero entero.\n");
            continue;
        }
        if (valor < min || valor > max)
        {
            printf("  Valor fuera de rango (%d a %d).\n", min, max);
            continue;
        }
        return (int)valor;
    }
}

float leerRealPositivo(const char *mensaje)
{
    char linea[LONG_LINEA];
    char *fin;
    float valor;
    int i;

    while (1)
    {
        printf("%s", mensaje);
        if (fgets(linea, LONG_LINEA, stdin) == NULL)
        {
            exit(0);
        }
        descartarResto(linea);
        for (i = 0; linea[i] != '\0'; i++)
        {
            if (linea[i] == ',')
            {
                linea[i] = '.';
            }
        }
        valor = strtof(linea, &fin);
        if (fin == linea)
        {
            printf("  Entrada invalida. Ingrese un numero.\n");
            continue;
        }
        while (*fin == ' ' || *fin == '\n' || *fin == '\r')
        {
            fin++;
        }
        if (*fin != '\0')
        {
            printf("  Entrada invalida. Ingrese solo un numero.\n");
            continue;
        }
        if (valor <= 0)
        {
            printf("  El valor debe ser mayor que cero.\n");
            continue;
        }
        return valor;
    }
}

void leerNombre(const char *mensaje, char *destino)
{
    char linea[LONG_LINEA];

    while (1)
    {
        printf("%s", mensaje);
        if (fgets(linea, LONG_LINEA, stdin) == NULL)
        {
            exit(0);
        }
        descartarResto(linea);
        recortar(linea);
        if (longitudCadena(linea) == 0)
        {
            printf("  El nombre no puede estar vacio.\n");
            continue;
        }
        if (longitudCadena(linea) > LONG_NOMBRE - 1)
        {
            printf("  El nombre admite maximo %d caracteres.\n", LONG_NOMBRE - 1);
            continue;
        }
        copiarNombre(destino, linea);
        return;
    }
}

int buscarProducto(char nombres[][LONG_NOMBRE], int n, const char *buscado)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (sonIguales(nombres[i], buscado))
        {
            return i;
        }
    }
    return -1;
}

void mostrarMenu(void)
{
    printf("\n========== CONTROL DE PRODUCCION ==========\n");
    printf(" 1. Ingresar producto\n");
    printf(" 2. Listar productos\n");
    printf(" 3. Editar producto\n");
    printf(" 4. Eliminar producto\n");
    printf(" 5. Definir tiempo y recursos disponibles\n");
    printf(" 6. Evaluar produccion\n");
    printf(" 7. Salir\n");
    printf("===========================================\n");
}

void ingresarProducto(char nombres[][LONG_NOMBRE], float *tiempo,  float *recurso, int *demanda, int *n)
{
    char nombre[LONG_NOMBRE];

    if (*n >= MAX_PROD)
    {
        printf("  Ya se registraron los %d productos permitidos.\n", MAX_PROD);
        return;
    }

    printf("\n--- Ingreso del producto %d de %d ---\n", *n + 1, MAX_PROD);
    leerNombre("Nombre del producto: ", nombre);

    if (buscarProducto(nombres, *n, nombre) != -1)
    {
        printf("  Ese producto ya existe. No se permiten nombres duplicados.\n");
        return;
    }

    copiarNombre(nombres[*n], nombre);
    *(tiempo + *n) = leerRealPositivo("Tiempo de fabricacion por unidad (horas): ");
    *(recurso + *n) = leerRealPositivo("Recursos por unidad: ");
    *(demanda + *n) = leerEntero("Cantidad demandada (unidades): ", 1, 1000000);

    (*n)++;
    printf("  Producto registrado correctamente.\n");
}

void listarProductos(char nombres[][LONG_NOMBRE], const float *tiempo,const float *recurso, const int *demanda, int n)
{
    int i;

    if (n == 0)
    {
        printf("  No hay productos registrados.\n");
        return;
    }
    printf("\n%-3s %-30s %10s %10s %10s\n", "N", "Producto", "h/unidad","rec/unidad", "Demanda");
    printf("--------------------------------------------------------------------\n");
    for (i = 0; i < n; i++)
    {
        printf("%-3d %-30s %10.2f %10.2f %10d\n", i + 1, nombres[i],*(tiempo + i), *(recurso + i), *(demanda + i));
    }
}

void editarProducto(char nombres[][LONG_NOMBRE], float *tiempo,  float *recurso, int *demanda, int n)
{
    char buscado[LONG_NOMBRE];
    char nuevo[LONG_NOMBRE];
    int pos, opcion, otro;

    if (n == 0)
    {
        printf("  No hay productos para editar.\n");
        return;
    }
    leerNombre("Nombre del producto a editar: ", buscado);
    pos = buscarProducto(nombres, n, buscado);
    if (pos == -1)
    {
        printf("  Producto no encontrado.\n");
        return;
    }

    printf("  Producto encontrado: %s\n", nombres[pos]);
    printf("  1. Nombre\n  2. Tiempo por unidad\n  3. Recursos por unidad\n""  4. Demanda\n  5. Cancelar\n");
    opcion = leerEntero("Dato a modificar: ", 1, 5);

    switch (opcion)
    {
    case 1:
        leerNombre("Nuevo nombre: ", nuevo);
        otro = buscarProducto(nombres, n, nuevo);
        if (otro != -1 && otro != pos)
        {
            printf("  Ya existe otro producto con ese nombre.\n");
        }
        else
        {
            copiarNombre(nombres[pos], nuevo);
            printf("  Nombre actualizado.\n");
        }
        break;
    case 2:
        *(tiempo + pos) = leerRealPositivo("Nuevo tiempo por unidad (horas): ");
        printf("  Tiempo actualizado.\n");
        break;
    case 3:
        *(recurso + pos) = leerRealPositivo("Nuevos recursos por unidad: ");
        printf("  Recursos actualizados.\n");
        break;
    case 4:
        *(demanda + pos) = leerEntero("Nueva demanda (unidades): ", 1, 1000000);
        printf("  Demanda actualizada.\n");
        break;
    default:
        printf("  Edicion cancelada.\n");
    }
}

void eliminarProducto(char nombres[][LONG_NOMBRE], float *tiempo,float *recurso, int *demanda, int *n)
{
    char buscado[LONG_NOMBRE];
    int pos, i;

    if (*n == 0)
    {
        printf("  No hay productos para eliminar.\n");
        return;
    }
    leerNombre("Nombre del producto a eliminar: ", buscado);
    pos = buscarProducto(nombres, *n, buscado);
    if (pos == -1)
    {
        printf("  Producto no encontrado.\n");
        return;
    }

    for (i = pos; i < *n - 1; i++)
    {
        copiarNombre(nombres[i], nombres[i + 1]);
        *(tiempo + i) = *(tiempo + i + 1);
        *(recurso + i) = *(recurso + i + 1);
        *(demanda + i) = *(demanda + i + 1);
    }
    (*n)--;
    printf("  Producto eliminado correctamente.\n");
}

void definirLimites(float *tiempoDisp, float *recursoDisp)
{
    *tiempoDisp = leerRealPositivo("Tiempo de produccion disponible (horas): ");
    *recursoDisp = leerRealPositivo("Recursos disponibles (unidades): ");
    printf("  Limites registrados.\n");
}

void evaluarProduccion(char nombres[][LONG_NOMBRE], const float *tiempo,const float *recurso, const int 
    *demanda, int n,float tiempoDisp, float recursoDisp)
{
    float tiempoTotal = 0.0f, recursoTotal = 0.0f;
    float tp, rp;
    int i;

    if (n == 0)
    {
        printf("  No hay productos registrados para evaluar.\n");
        return;
    }
    if (tiempoDisp <= 0 || recursoDisp <= 0)
    {
        printf("  Primero defina el tiempo y los recursos disponibles (opcion 5).\n");
        return;
    }

    printf("\n%-30s %12s %12s\n", "Producto", "Tiempo (h)", "Recursos");
    printf("----------------------------------------------------------\n");
    for (i = 0; i < n; i++)
    {
        tp = (float)*(demanda + i) * *(tiempo + i);
        rp = (float)*(demanda + i) * *(recurso + i);
        printf("%-30s %12.2f %12.2f\n", nombres[i], tp, rp);
        tiempoTotal += tp;
        recursoTotal += rp;
    }
    printf("----------------------------------------------------------\n");
    printf("%-30s %12.2f %12.2f\n", "TOTAL REQUERIDO", tiempoTotal, recursoTotal);
    printf("%-30s %12.2f %12.2f\n", "DISPONIBLE", tiempoDisp, recursoDisp);

    if (tiempoTotal <= tiempoDisp && recursoTotal <= recursoDisp)
    {
        printf("\n  RESULTADO: SI se puede cumplir con la demanda.\n");
        printf("  Holgura: %.2f horas y %.2f unidades de recurso.\n",
               tiempoDisp - tiempoTotal, recursoDisp - recursoTotal);
    }
    else
    {
        printf("\n  RESULTADO: NO se puede cumplir con la demanda.\n");
        if (tiempoTotal > tiempoDisp)
        {
            printf("  Faltan %.2f horas de produccion.\n", tiempoTotal - tiempoDisp);
        }
        if (recursoTotal > recursoDisp)
        {
            printf("  Faltan %.2f unidades de recurso.\n", recursoTotal - recursoDisp);
        }
    }
}

int main(void)
{

    char nombres[MAX_PROD][LONG_NOMBRE];
    float tiempoUnit[MAX_PROD];
    float recursoUnit[MAX_PROD];
    int demanda[MAX_PROD];

    int cantidad = 0;
    float tiempoDisp = 0.0f;
    float recursoDisp = 0.0f;
    int opcion;

    do
    {
        mostrarMenu();
        opcion = leerEntero("Seleccione una opcion: ", 1, 7);

        switch (opcion)
        {
        case 1:
            ingresarProducto(nombres, tiempoUnit, recursoUnit, demanda, &cantidad);
            break;
        case 2:
            listarProductos(nombres, tiempoUnit, recursoUnit, demanda, cantidad);
            break;
        case 3:
            editarProducto(nombres, tiempoUnit, recursoUnit, demanda, cantidad);
            break;
        case 4:
            eliminarProducto(nombres, tiempoUnit, recursoUnit, demanda, &cantidad);
            break;
        case 5:
            definirLimites(&tiempoDisp, &recursoDisp);
            break;
        case 6:
            evaluarProduccion(nombres, tiempoUnit, recursoUnit, demanda, cantidad, tiempoDisp, recursoDisp);
            break;
        case 7:
            printf("\nPrograma finalizado.\n");
            break;
        }
    } while (opcion != 7);

    return 0;
}