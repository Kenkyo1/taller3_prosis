#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_LINE 256
#define MAX_STUDENTS 30
#define MAX_SUBJECTS 10

// Estructuras
typedef struct
{
    char nombre[50];
    char codigo[10];
    char estado[10];
} Materia;

typedef struct
{
    char nombres[50];
    char apellidos[50];
    char matricula[15];
    char usuario[20];
    char clave[20];
    char estado[10];
} Estudiante;

typedef struct
{
    char nombres[50];
    char apellidos[50];
    char cc[15];
    char usuario[20];
    char clave[20];
    char estado[10];
    char materias[MAX_SUBJECTS][10];
    int numMaterias;
} Profesor;

typedef struct
{
    char codigo[10];
    char codigoMateria[10];
    char ccProfesor[15];
    char fechaInicio[15];
    char fechaFin[15];
    char estudiantes[MAX_STUDENTS][15];
    int numEstudiantes;
} Curso;

// Prototipos de funciones
void cargarDatos();
void guardarDatos();
void menu();
void menuMaterias();
void menuEstudiantes();
void menuProfesores();
void menuCursos();
void crearMateria();
void editarMateria();
void crearEstudiante();
void editarEstudiante();
void crearProfesor();
void editarProfesor();
void crearCurso();
void editarCurso();

// Variables globales
Materia materias[MAX_SUBJECTS];
Estudiante estudiantes[MAX_STUDENTS];
Profesor profesores[MAX_STUDENTS];
Curso cursos[MAX_STUDENTS];

int numMaterias = 0;
int numEstudiantes = 0;
int numProfesores = 0;
int numCursos = 0;

// Prototipos de funciones auxiliares
int verificarUnicidadCodigo(char *codigo, char tipo);
int verificarUnicidadUsuario(char *usuario);
int verificarCursoEnProceso(char *identificador, char tipo);
int compararFechas(const char *fecha1, const char *fecha2);
int fechaActualEsMayor(const char *fecha);

int main()
{
    cargarDatos();
    menu();
    guardarDatos();
    return 0;
}

void cargarDatos()
{
    FILE *file;
    char line[MAX_LINE];

    // Cargar materias
    file = fopen("materias.txt", "r");
    if (file)
    {
        while (fgets(line, sizeof(line), file))
        {
            sscanf(line, "%[^-]-%[^-]-%s", materias[numMaterias].nombre, materias[numMaterias].codigo, materias[numMaterias].estado);
            numMaterias++;
        }
        fclose(file);
    }

    // Cargar estudiantes
    file = fopen("estudiantes.txt", "r");
    if (file)
    {
        while (fgets(line, sizeof(line), file))
        {
            sscanf(line, "%[^-]-%[^-]-%[^-]-%[^-]-%[^-]-%s", estudiantes[numEstudiantes].nombres, estudiantes[numEstudiantes].apellidos, estudiantes[numEstudiantes].matricula, estudiantes[numEstudiantes].usuario, estudiantes[numEstudiantes].clave, estudiantes[numEstudiantes].estado);
            numEstudiantes++;
        }
        fclose(file);
    }

    // Cargar profesores
    file = fopen("profesores.txt", "r");
    if (file)
    {
        while (fgets(line, sizeof(line), file))
        {
            char materiasStr[MAX_LINE];
            sscanf(line, "%[^-]-%[^-]-%[^-]-%[^-]-%[^-]-%[^-]-%s", profesores[numProfesores].nombres, profesores[numProfesores].apellidos, profesores[numProfesores].cc, profesores[numProfesores].usuario, profesores[numProfesores].clave, profesores[numProfesores].estado, materiasStr);

            // Procesar materias
            char *token = strtok(materiasStr, "/");
            while (token)
            {
                strcpy(profesores[numProfesores].materias[profesores[numProfesores].numMaterias++], token);
                token = strtok(NULL, "/");
            }
            numProfesores++;
        }
        fclose(file);
    }

    // Cargar cursos
    file = fopen("cursos.txt", "r");
    if (file)
    {
        while (fgets(line, sizeof(line), file))
        {
            char estudiantesStr[MAX_LINE];
            sscanf(line, "%[^-]-%[^-]-%[^-]-%[^-]-%[^-]-%s", cursos[numCursos].codigo, cursos[numCursos].codigoMateria, cursos[numCursos].ccProfesor, cursos[numCursos].fechaInicio, cursos[numCursos].fechaFin, estudiantesStr);

            // Procesar estudiantes
            char *token = strtok(estudiantesStr, "/");
            while (token)
            {
                strcpy(cursos[numCursos].estudiantes[cursos[numCursos].numEstudiantes++], token);
                token = strtok(NULL, "/");
            }
            numCursos++;
        }
        fclose(file);
    }
}

void guardarDatos()
{
    FILE *file;

    // Guardar materias
    file = fopen("materias.txt", "w");
    for (int i = 0; i < numMaterias; i++)
    {
        fprintf(file, "%s-%s-%s\n", materias[i].nombre, materias[i].codigo, materias[i].estado);
    }
    fclose(file);

    // Guardar estudiantes
    file = fopen("estudiantes.txt", "w");
    for (int i = 0; i < numEstudiantes; i++)
    {
        fprintf(file, "%s-%s-%s-%s-%s-%s\n", estudiantes[i].nombres, estudiantes[i].apellidos, estudiantes[i].matricula, estudiantes[i].usuario, estudiantes[i].clave, estudiantes[i].estado);
    }
    fclose(file);

    // Guardar profesores
    file = fopen("profesores.txt", "w");
    for (int i = 0; i < numProfesores; i++)
    {
        fprintf(file, "%s-%s-%s-%s-%s-%s", profesores[i].nombres, profesores[i].apellidos, profesores[i].cc, profesores[i].usuario, profesores[i].clave, profesores[i].estado);
        for (int j = 0; j < profesores[i].numMaterias; j++)
        {
            fprintf(file, "/%s", profesores[i].materias[j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);

    // Guardar cursos
    file = fopen("cursos.txt", "w");
    for (int i = 0; i < numCursos; i++)
    {
        fprintf(file, "%s-%s-%s-%s-%s", cursos[i].codigo, cursos[i].codigoMateria, cursos[i].ccProfesor, cursos[i].fechaInicio, cursos[i].fechaFin);
        for (int j = 0; j < cursos[i].numEstudiantes; j++)
        {
            fprintf(file, "/%s", cursos[i].estudiantes[j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void menu()
{
    int opcion;
    do
    {
        printf("\n--- Men\u00fa Principal ---\n");
        printf("1. Materias\n");
        printf("2. Estudiantes\n");
        printf("3. Profesores\n");
        printf("4. Cursos\n");
        printf("5. Salir\n");
        printf("Seleccione una opci\u00f3n: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            menuMaterias();
            break;
        case 2:
            menuEstudiantes();
            break;
        case 3:
            menuProfesores();
            break;
        case 4:
            menuCursos();
            break;
        case 5:
            printf("Saliendo del programa...\n");
            break;
        default:
            printf("Opci\u00f3n no v\u00e1lida. Intente de nuevo.\n");
        }
    } while (opcion != 5);
}

void menuMaterias()
{
    int opcion;
    do
    {
        printf("\n--- Men\u00fa Materias ---\n");
        printf("1. Crear Materia\n");
        printf("2. Editar Materia\n");
        printf("3. Volver\n");
        printf("Seleccione una opci\u00f3n: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            crearMateria();
            break;
        case 2:
            editarMateria();
            break;
        case 3:
            printf("Volviendo al men\u00fa principal...\n");
            break;
        default:
            printf("Opci\u00f3n no v\u00e1lida. Intente de nuevo.\n");
        }
    } while (opcion != 3);
}

void menuEstudiantes()
{
    int opcion;
    do
    {
        printf("\n--- Men\u00fa Estudiantes ---\n");
        printf("1. Crear Estudiante\n");
        printf("2. Editar Estudiante\n");
        printf("3. Volver\n");
        printf("Seleccione una opci\u00f3n: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            crearEstudiante();
            break;
        case 2:
            editarEstudiante();
            break;
        case 3:
            printf("Volviendo al men\u00fa principal...\n");
            break;
        default:
            printf("Opci\u00f3n no v\u00e1lida. Intente de nuevo.\n");
        }
    } while (opcion != 3);
}

void menuProfesores()
{
    int opcion;
    do
    {
        printf("\n--- Men\u00fa Profesores ---\n");
        printf("1. Crear Profesor\n");
        printf("2. Editar Profesor\n");
        printf("3. Volver\n");
        printf("Seleccione una opci\u00f3n: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            crearProfesor();
            break;
        case 2:
            editarProfesor();
            break;
        case 3:
            printf("Volviendo al men\u00fa principal...\n");
            break;
        default:
            printf("Opci\u00f3n no v\u00e1lida. Intente de nuevo.\n");
        }
    } while (opcion != 3);
}

void menuCursos()
{
    int opcion;
    do
    {
        printf("\n--- Men\u00fa Cursos ---\n");
        printf("1. Crear Curso\n");
        printf("2. Editar Curso\n");
        printf("3. Volver\n");
        printf("Seleccione una opci\u00f3n: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            crearCurso();
            break;
        case 2:
            editarCurso();
            break;
        case 3:
            printf("Volviendo al men\u00fa principal...\n");
            break;
        default:
            printf("Opci\u00f3n no v\u00e1lida. Intente de nuevo.\n");
        }
    } while (opcion != 3);
}

void crearMateria()
{
    Materia nuevaMateria;
    printf("\nIngrese el nombre de la materia: ");
    scanf(" %49[^'\n']", nuevaMateria.nombre);
    printf("Ingrese el c\u00f3digo de la materia: ");
    scanf(" %9s", nuevaMateria.codigo);

    // Verificar unicidad del código
    if (verificarUnicidadCodigo(nuevaMateria.codigo, 'M'))
    {
        printf("El código ingresado ya existe. Operación cancelada.\n");
        return;
    }

    strcpy(nuevaMateria.estado, "Activo");
    materias[numMaterias++] = nuevaMateria;
    printf("Materia creada exitosamente.\n");
}

void editarMateria()
{
    char codigo[10];
    printf("\nIngrese el código de la materia a editar: ");
    scanf(" %9s", codigo);

    for (int i = 0; i < numMaterias; i++)
    {
        if (strcmp(materias[i].codigo, codigo) == 0)
        {
            // Verificar si tiene cursos en proceso
            if (verificarCursoEnProceso(codigo, 'M'))
            {
                printf("No se puede inactivar la materia porque tiene cursos en proceso.\n");
                return;
            }

            printf("Estado actual: %s\n", materias[i].estado);
            printf("Ingrese el nuevo estado (Activo/Inactivo): ");
            scanf(" %9s", materias[i].estado);
            printf("Materia editada exitosamente.\n");
            return;
        }
    }
    printf("No se encontró una materia con el código ingresado.\n");
}

void crearEstudiante()
{
    Estudiante nuevoEstudiante;
    printf("\nIngrese los nombres del estudiante: ");
    scanf(" %49[^'\n']", nuevoEstudiante.nombres);
    printf("Ingrese los apellidos del estudiante: ");
    scanf(" %49[^'\n']", nuevoEstudiante.apellidos);
    printf("Ingrese la matrícula del estudiante: ");
    scanf(" %14s", nuevoEstudiante.matricula);

    // Verificar unicidad de matrícula y usuario
    if (verificarUnicidadCodigo(nuevoEstudiante.matricula, 'E'))
    {
        printf("La matrícula ingresada ya existe. Operación cancelada.\n");
        return;
    }

    printf("Ingrese el usuario del estudiante: ");
    scanf(" %19s", nuevoEstudiante.usuario);
    if (verificarUnicidadUsuario(nuevoEstudiante.usuario))
    {
        printf("El usuario ingresado ya existe. Operación cancelada.\n");
        return;
    }

    printf("Ingrese la clave del estudiante: ");
    scanf(" %19s", nuevoEstudiante.clave);
    strcpy(nuevoEstudiante.estado, "Activo");
    estudiantes[numEstudiantes++] = nuevoEstudiante;
    printf("Estudiante creado exitosamente.\n");
}

void editarEstudiante()
{
    char matricula[15];
    printf("\nIngrese la matrícula del estudiante a editar: ");
    scanf(" %14s", matricula);

    for (int i = 0; i < numEstudiantes; i++)
    {
        if (strcmp(estudiantes[i].matricula, matricula) == 0)
        {
            // Verificar si tiene cursos en proceso
            if (verificarCursoEnProceso(matricula, 'E'))
            {
                printf("No se puede inactivar el estudiante porque tiene cursos en proceso.\n");
                return;
            }

            printf("Clave actual: %s\n", estudiantes[i].clave);
            printf("Ingrese la nueva clave: ");
            scanf(" %19s", estudiantes[i].clave);
            printf("Estado actual: %s\n", estudiantes[i].estado);
            printf("Ingrese el nuevo estado (Activo/Inactivo): ");
            scanf(" %9s", estudiantes[i].estado);
            printf("Estudiante editado exitosamente.\n");
            return;
        }
    }
    printf("No se encontró un estudiante con la matrícula ingresada.\n");
}

void crearProfesor()
{
    Profesor nuevoProfesor;
    printf("\nIngrese los nombres del profesor: ");
    scanf(" %49[^'\n']", nuevoProfesor.nombres);
    printf("Ingrese los apellidos del profesor: ");
    scanf(" %49[^'\n']", nuevoProfesor.apellidos);
    printf("Ingrese el C.C. del profesor: ");
    scanf(" %14s", nuevoProfesor.cc);

    // Verificar unicidad de C.C. y usuario
    if (verificarUnicidadCodigo(nuevoProfesor.cc, 'P'))
    {
        printf("El C.C. ingresado ya existe. Operación cancelada.\n");
        return;
    }

    printf("Ingrese el usuario del profesor: ");
    scanf(" %19s", nuevoProfesor.usuario);
    if (verificarUnicidadUsuario(nuevoProfesor.usuario))
    {
        printf("El usuario ingresado ya existe. Operación cancelada.\n");
        return;
    }

    printf("Ingrese la clave del profesor: ");
    scanf(" %19s", nuevoProfesor.clave);
    strcpy(nuevoProfesor.estado, "Activo");

    // Asociar materias
    printf("Ingrese los códigos de las materias que puede dictar separados por espacios (max %d): ", MAX_SUBJECTS);
    char materiasInput[MAX_LINE];
    scanf(" %[^'\n']", materiasInput);
    char *token = strtok(materiasInput, " ");
    while (token && nuevoProfesor.numMaterias < MAX_SUBJECTS)
    {
        strcpy(nuevoProfesor.materias[nuevoProfesor.numMaterias++], token);
        token = strtok(NULL, " ");
    }

    profesores[numProfesores++] = nuevoProfesor;
    printf("Profesor creado exitosamente.\n");
}

void editarProfesor()
{
    char cc[15];
    printf("\nIngrese el C.C. del profesor a editar: ");
    scanf(" %14s", cc);

    for (int i = 0; i < numProfesores; i++)
    {
        if (strcmp(profesores[i].cc, cc) == 0)
        {
            // Verificar si tiene cursos en proceso
            if (verificarCursoEnProceso(cc, 'P'))
            {
                printf("No se puede inactivar el profesor porque tiene cursos en proceso.\n");
                return;
            }

            printf("Clave actual: %s\n", profesores[i].clave);
            printf("Ingrese la nueva clave: ");
            scanf(" %19s", profesores[i].clave);
            printf("Estado actual: %s\n", profesores[i].estado);
            printf("Ingrese el nuevo estado (Activo/Inactivo): ");
            scanf(" %9s", profesores[i].estado);
            printf("Profesor editado exitosamente.\n");
            return;
        }
    }
    printf("No se encontró un profesor con el C.C. ingresado.\n");
}

void crearCurso()
{
    Curso nuevoCurso;
    printf("\nIngrese el código del curso: ");
    scanf(" %9s", nuevoCurso.codigo);

    // Verificar unicidad del código
    if (verificarUnicidadCodigo(nuevoCurso.codigo, 'C'))
    {
        printf("El código ingresado ya existe. Operación cancelada.\n");
        return;
    }

    printf("Ingrese el código de la materia: ");
    scanf(" %9s", nuevoCurso.codigoMateria);
    printf("Ingrese el C.C. del profesor: ");
    scanf(" %14s", nuevoCurso.ccProfesor);
    printf("Ingrese la fecha de inicio (DD/MM/AAAA): ");
    scanf(" %14s", nuevoCurso.fechaInicio);
    printf("Ingrese la fecha de fin (DD/MM/AAAA): ");
    scanf(" %14s", nuevoCurso.fechaFin);

    printf("Ingrese las matrículas de los estudiantes separados por espacios (max %d): ", MAX_STUDENTS);
    char estudiantesInput[MAX_LINE];
    scanf(" %[^'\n']", estudiantesInput);
    char *token = strtok(estudiantesInput, " ");
    while (token && nuevoCurso.numEstudiantes < MAX_STUDENTS)
    {
        strcpy(nuevoCurso.estudiantes[nuevoCurso.numEstudiantes++], token);
        token = strtok(NULL, " ");
    }

    cursos[numCursos++] = nuevoCurso;
    printf("Curso creado exitosamente.\n");
}

void editarCurso()
{
    char codigo[10];
    printf("\nIngrese el código del curso a editar: ");
    scanf(" %9s", codigo);

    for (int i = 0; i < numCursos; i++)
    {
        if (strcmp(cursos[i].codigo, codigo) == 0)
        {
            printf("Ingrese el nuevo código de la materia: ");
            scanf(" %9s", cursos[i].codigoMateria);
            printf("Ingrese el nuevo C.C. del profesor: ");
            scanf(" %14s", cursos[i].ccProfesor);
            printf("Ingrese la nueva fecha de inicio (DD/MM/AAAA): ");
            scanf(" %14s", cursos[i].fechaInicio);
            printf("Ingrese la nueva fecha de fin (DD/MM/AAAA): ");
            scanf(" %14s", cursos[i].fechaFin);

            printf("Ingrese las matrículas de los estudiantes separados por espacios (max %d): ", MAX_STUDENTS);
            cursos[i].numEstudiantes = 0;
            char estudiantesInput[MAX_LINE];
            scanf(" %[^'\n']", estudiantesInput);
            char *token = strtok(estudiantesInput, " ");
            while (token && cursos[i].numEstudiantes < MAX_STUDENTS)
            {
                strcpy(cursos[i].estudiantes[cursos[i].numEstudiantes++], token);
                token = strtok(NULL, " ");
            }

            printf("Curso editado exitosamente.\n");
            return;
        }
    }
    printf("No se encontró un curso con el código ingresado.\n");
}

// Implementación de funciones auxiliares
int verificarUnicidadCodigo(char *codigo, char tipo)
{
    if (tipo == 'M')
    {
        for (int i = 0; i < numMaterias; i++)
        {
            if (strcmp(materias[i].codigo, codigo) == 0)
            {
                return 1;
            }
        }
    }
    else if (tipo == 'E')
    {
        for (int i = 0; i < numEstudiantes; i++)
        {
            if (strcmp(estudiantes[i].matricula, codigo) == 0)
            {
                return 1;
            }
        }
    }
    else if (tipo == 'P')
    {
        for (int i = 0; i < numProfesores; i++)
        {
            if (strcmp(profesores[i].cc, codigo) == 0)
            {
                return 1;
            }
        }
    }
    else if (tipo == 'C')
    {
        for (int i = 0; i < numCursos; i++)
        {
            if (strcmp(cursos[i].codigo, codigo) == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

int verificarUnicidadUsuario(char *usuario)
{
    for (int i = 0; i < numEstudiantes; i++)
    {
        if (strcmp(estudiantes[i].usuario, usuario) == 0)
        {
            return 1;
        }
    }
    for (int i = 0; i < numProfesores; i++)
    {
        if (strcmp(profesores[i].usuario, usuario) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int verificarCursoEnProceso(char *identificador, char tipo)
{
    for (int i = 0; i < numCursos; i++)
    {
        if (tipo == 'M' && strcmp(cursos[i].codigoMateria, identificador) == 0)
        {
            if (fechaActualEsMayor(cursos[i].fechaInicio) && !fechaActualEsMayor(cursos[i].fechaFin))
            {
                return 1;
            }
        }
        else if (tipo == 'E')
        {
            for (int j = 0; j < cursos[i].numEstudiantes; j++)
            {
                if (strcmp(cursos[i].estudiantes[j], identificador) == 0)
                {
                    if (fechaActualEsMayor(cursos[i].fechaInicio) && !fechaActualEsMayor(cursos[i].fechaFin))
                    {
                        return 1;
                    }
                }
            }
        }
        else if (tipo == 'P' && strcmp(cursos[i].ccProfesor, identificador) == 0)
        {
            if (fechaActualEsMayor(cursos[i].fechaInicio) && !fechaActualEsMayor(cursos[i].fechaFin))
            {
                return 1;
            }
        }
    }
    return 0;
}

int compararFechas(const char *fecha1, const char *fecha2)
{
    struct tm tm1, tm2;
    strptime(fecha1, "%d/%m/%Y", &tm1);
    strptime(fecha2, "%d/%m/%Y", &tm2);

    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);

    if (t1 < t2)
        return -1;
    if (t1 > t2)
        return 1;
    return 0;
}

int fechaActualEsMayor(const char *fecha)
{
    time_t t = time(NULL);
    struct tm tmActual = *localtime(&t);

    char fechaActual[15];
    strftime(fechaActual, sizeof(fechaActual), "%d/%m/%Y", &tmActual);

    return compararFechas(fecha, fechaActual) < 0;
}