#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "Map.h"
#include <time.h>

typedef struct{
    char* nombre;
    int visitado;
    List* pistas;
    List* caminos;
}habitacion;

typedef struct{
    char* nombre;//cuchillo
    char* menu;//revisar utensilios 
    char* info;
    int vista;
}pista;

typedef struct 
{
    char* nombre;
    int energia;
    List* pistas;
    int pistasEspeciales;
}personaje;

int is_equal_string(void * key1, void * key2) 
{
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

void mostrarInicio(char* nombre)
{
    FILE *fichero;
    char c[1024], respuesta[1];
    int argc;
    char *argv;
    int check = 0;

    argv = "texto/intro1.txt";
    fichero = fopen (argv, "rb");
    while (fgets(c, 1024, fichero)) {
       printf("%s", c);
       check++;
       if (check != 17) getchar();
    }
    printf(" %s", nombre);
    getchar();
   
    fclose (fichero);
    argv = "texto/intro2.txt";
    fichero = fopen (argv, "rb");
    while (fgets(c, 1024, fichero)) {
       printf("%s", c);
       check++;
       if (check != 22) getchar();
       else while (strcmp(respuesta, "s") != 0 || strcmp(respuesta, "n") != 0)
       {
            scanf("%s", respuesta);
            getchar();
            if (strcmp(respuesta, "s") == 0) break;
            else if (strcmp(respuesta, "n") == 0)
            {
                fclose (fichero);
                argv = "texto/malfinal1.txt";
                fichero = fopen (argv, "rb");
                while (fgets(c, 1024, fichero)) {
                    printf("%s", c);
                    getchar();
                }
            }
            exit(1);
            break;
       }
    }
    fclose (fichero);
}

void insertarPistas(Map* grafo)
{
    FILE* pistas = fopen("texto/pistas.txt", "rt");
    char aux[1024];
    char* token;
    while(fgets(aux,1024,pistas))
    {
        pista* p = (pista*) malloc (sizeof(pista));
        token = strtok(aux,",");
        p->nombre = strdup(token);
        token = strtok(NULL, ",");
        p->menu = strdup(token);
        token = strtok(NULL, ",");
        p->info=strdup(token);
        p->vista = 0;
        token = strtok(NULL, "\n");
        habitacion * res = searchMap(grafo, token);
        pushBack(res->pistas, p);
    }
}

void definirGrafo(Map* grafo)
{
    FILE* zonas = fopen("texto/zonas.txt", "rt");
    char aux[1024];
    char* token;
    habitacion* principal;
    while(fgets(aux,1024,zonas))
    {
        habitacion* hab=(habitacion*) malloc (sizeof(habitacion));
        token = strtok(aux,"\n");
        hab->nombre = strdup(token);
        hab->visitado = 0;
        hab->pistas = createList();
        hab->caminos = createList();
        habitacion* res;
        if (strcmp(hab->nombre, "Living") == 0)
        {
            insertMap(grafo, hab->nombre, hab);
            principal = hab;
        }
        else
        {
            res = searchMap(grafo, "Living");
            insertMap(grafo, hab->nombre, hab);
            pushBack(res->caminos, hab);
            res = searchMap(grafo, hab->nombre);
            pushBack(res->caminos, principal);
        }
    }
    fclose(zonas);
    insertarPistas(grafo);
}

void mostrarZonas(Map* grafo)
{
    habitacion* a = searchMap(grafo,"Living");
    habitacion* test = (firstList(a->caminos));
    printf("0.- Menu de opciones\n");
    int num = 1;
    while(test)
    {
        printf("%i.- %s\n",num, test->nombre);
        num++;
        test = (nextList(a->caminos));
    }
}

void mostrarPistas(personaje* usuario)
{
    printf("\nPistas encontradas\n\n");
    int num = 0;
    pista* test = firstList(usuario->pistas);
    while(test)
    {
        num++;
        printf("%i.- %s,",num, test->nombre);
        printf("%s\n",test->info);
        test=nextList(usuario->pistas);
    }
    printf("\n");
}

void menuOpciones(personaje* usuario,Map* grafo)
{
    int opciones;
    printf("Seleccione la opcion\n");
    printf("1.- Mostrar Pistas\n2.- Guardar Partida\n3.- Salir del Juego\n");
    scanf("%i", &opciones);
    switch (opciones)
    {
    case 1:
        mostrarPistas(usuario);
        break;
    case 2:
        //guardarpartida(usuario, grafo);
        break;  
    case 3:
        exit(0);
        break;  
    default:
        break;
    } 
}

void zonaHabitacionPri(personaje* usuario, Map* grafo)
{
    int input;
    printf("Donde desea investigar?\n");
    habitacion* a = searchMap(grafo, "Habitacion Principal");
    a->visitado++;
    pista* p = firstList(a->pistas);
    printf("1.- %s\n", p->menu);
    pista* b = nextList(a->pistas);
    printf("2.- %s\n", b->menu);
    printf("3.- Volver al Living\n");
    scanf("%d", &input);
    while(input != 3)
    {
        switch(input)
        {
        case 1:
        if(p->vista == 0)
        {
            p->vista++;
            printf("%s\n", p->info);
            pushBack(usuario->pistas, p);
        }
        else
        {
            printf("Ya has revisado esta pista.\n");
        }
        break;

        case 2:
        if(b->vista == 0)
        {
            b->vista++;
            printf("%s\n", b->info);
            pushBack(usuario->pistas, b);

        }
        else printf("Ya has revisado esta pista.\n");
        break;

        case 3:
        break;
        }
        printf("1.- %s\n", p->menu);
        printf("2.- %s\n", b->menu);
        printf("3.- Volver al Living\n");
        scanf("%d", &input);
    }

}
void zonaBanyo(personaje* usuario, Map* grafo)
{
    int input;
    printf("Donde desea investigar?\n");
    habitacion* a = searchMap(grafo, "Banyo de Invitados");
    a->visitado++;
    pista* p = firstList(a->pistas);
    printf("1.- %s\n", p->menu);
    pista* b = nextList(a->pistas);
    printf("2.- %s\n", b->menu);
    printf("3.- Volver al Living\n");
    scanf("%d", &input);
    while(input!=3)
    {
        switch(input)
        {
        case 1:
        if(p->vista == 0)
        {
            p->vista++;
            printf("%s\n", p->info);
            pushBack(usuario->pistas, p);

        }
        else printf("Ya has revisado esta pista.\n");
        break;

        case 2:
        if(b->vista == 0)
        {
            b->vista++;
            printf("%s\n", b->info);
            pushBack(usuario->pistas, b);

        }
        else printf("Ya has revisado esta pista.\n");
        break;

        case 3:
        break;
        }
        
        printf("1.- %s\n", p->menu);
        printf("2.- %s\n", b->menu);
        printf("3.- Volver al Living\n");
        scanf("%d", &input);

    }

}
void zonaCocina(personaje* usuario, Map* grafo)
{
    int input;
    printf("Donde desea investigar?\n");
    habitacion* a = searchMap(grafo, "Cocina");
    a->visitado++;
    pista* p = firstList(a->pistas);
    printf("1.- %s\n", p->menu);
    pista* b = nextList(a->pistas);
    printf("2.- %s\n", b->menu);
    printf("3.- Volver al Living\n");
    scanf("%d", &input);
    while(input!=3)
    {

        switch(input)
        {
        case 1:
        
        if(p->vista == 0)
        {
            p->vista++;
            printf("%s\n", p->info);
            pushBack(usuario->pistas, p);

        }
        else printf("Ya has revisado esta pista.\n");
        break;

        case 2:
        if(b->vista == 0)
        {
            b->vista++;
            printf("%s\n", b->info);
            pushBack(usuario->pistas, b);

        }
        else printf("Ya has revisado esta pista.\n");
        break;

        case 3:
        break;
        }
        printf("1.- %s\n", p->menu);
        printf("2.- %s\n", b->menu);
        printf("3.- Volver al Living\n");
        scanf("%d", &input);
    }
}
void zonaGaraje(personaje* usuario, Map* grafo)
{
    int input;
    printf("Donde desea investigar?\n");
    habitacion* a = searchMap(grafo, "Garaje");
    a->visitado++;
    pista* p = firstList(a->pistas);
    printf("1.- %s\n", p->menu);
    pista* b = nextList(a->pistas);
    printf("2.- %s\n", b->menu);
    printf("3.- Volver al Living\n");
    scanf("%d", &input);
    while(input!=3)
    {

        switch(input)
        {
        case 1:
        
        if(p->vista == 0)
        {
            p->vista++;
            printf("%s\n", p->info);
            pushBack(usuario->pistas, p);

        }
        else printf("Ya has revisado esta pista.\n");
        break;

        case 2:
        if(b->vista == 0)
        {
            b->vista++;
            printf("%s\n", b->info);
            pushBack(usuario->pistas, b);

        }
        else printf("Ya has revisado esta pista.\n");
        break;

        case 3:
        break;
        }
        printf("1.- %s\n", p->menu);
        printf("2.- %s\n", b->menu);
        printf("3.- Volver al Living\n");
        scanf("%d", &input);
    }
}
void zonaHabitacionHijo(personaje* usuario, Map* grafo)
{
    int input;
    printf("Donde desea investigar?\n");
    habitacion* a = searchMap(grafo, "Habitacion del Hijo");
    a->visitado++;
    pista* p = firstList(a->pistas);
    printf("1.- %s\n", p->menu);
    pista* b = nextList(a->pistas);
    printf("2.- %s\n", b->menu);
    printf("3.- Volver al Living\n");
    scanf("%d", &input);
    while(input!=3)
    {

        switch(input)
        {
        case 1:
        
        if(p->vista == 0)
        {
            p->vista++;
            printf("%s\n", p->info);
            pushBack(usuario->pistas, p);

        }
        else printf("Ya has revisado esta pista.\n");
        break;

        case 2:
        if(b->vista == 0)
        {
            b->vista++;
            printf("%s\n", b->info);
            pushBack(usuario->pistas, b);

        }
        else printf("Ya has revisado esta pista.\n");
        break;

        case 3:
        break;
        }
        printf("1.- %s\n", p->menu);
        printf("2.- %s\n", b->menu);
        printf("3.- Volver al Living\n");
        scanf("%d", &input);
    }
}
void zonaPatio(personaje* usuario, Map* grafo)
{
    int input;
    printf("Donde desea investigar?\n");
    habitacion* a = searchMap(grafo, "Patio Trasero");
    a->visitado++;
    pista* p = firstList(a->pistas);
    printf("1.- %s\n", p->menu);
    pista* b = nextList(a->pistas);
    printf("2.- %s\n", b->menu);
    printf("3.- Volver al Living\n");
    scanf("%d", &input);
    while(input!=3)
    {

        switch(input)
        {
        case 1:
        if(p->vista == 0)
        {
            p->vista++;
            printf("%s\n", p->info);
            pushBack(usuario->pistas, p);

        }
        else printf("Ya has revisado esta pista.\n");
        break;

        case 2:
        if(b->vista == 0)
        {
            b->vista++;
            printf("%s\n", b->info);
            pushBack(usuario->pistas, b);

        }
        else printf("Ya has revisado esta pista.\n");
        break;

        case 3:
        break;
        }
        printf("1.- %s\n", p->menu);
        printf("2.- %s\n", b->menu);
        printf("3.- Volver al Living\n");
        scanf("%d", &input);
    }
}
void zonaSotano(personaje* usuario, Map* grafo)
{
    int input;
    printf("Donde desea investigar?\n");
    habitacion* a = searchMap(grafo, "Sotano");
    a->visitado++;
    pista* p = firstList(a->pistas);
    printf("1.- %s\n", p->menu);
    pista* b = nextList(a->pistas);
    printf("2.- %s\n", b->menu);
    printf("3.- Volver al Living\n");
    scanf("%d", &input);
    while(input!=3)
    {

        switch(input)
        {
        case 1:
        if(p->vista == 0)
        {
            p->vista++;
            printf("%s\n", p->info);
            pushBack(usuario->pistas, p);
        }
        else printf("Ya has revisado esta pista.\n");
        break;

        case 2:
        if(b->vista == 0)
        {
            b->vista++;
            printf("%s\n", b->info);
            pushBack(usuario->pistas, b);
        }
        else printf("Ya has revisado esta pista.\n");
        break;

        case 3:
        break;
        }
        printf("1.- %s\n", p->menu);
        printf("2.- %s\n", b->menu);
        printf("3.- Volver al Living\n");
        scanf("%d", &input);
    }
}

void finalRandom()
{
    srand(time(NULL));
    int numero = rand() % 51;
    if(numero == 15)
    {
        printf("final random");
        exit(0);
    }
}

void finalJuego(personaje* usuario)
{
    int eleccion = 0;
    printf("texto que se te acabo el tiempo\n");
    printf("Quien crees que es asesino de los 3 sospechosos?\n");
    printf("0.- Ver pistas\n1.- Esposa de la victima\n2.- El hermano de la victima\n3.- La sirvienta de la casa\n");
    while(eleccion == 0)
    {
        scanf("%i", &eleccion);
        switch (eleccion)
        {
        case 0:
            mostrarPistas(usuario);
            printf("Quien crees que es asesino de los 3 sospechosos luego de ver tus pistas?\n");
            printf("1.- Esposa de la victima\n2.- El hermano de la victima\n3.- La sirvienta de la casa\n");
            break;
        case 1:
            printf("texto final esposa\n");
            break;
        case 2:
            printf("texto final hermano\n");
            break;
        case 3:
            printf("texto final sirvienta\n"); 
            break;   
        default:
            break;
        }
    }
    exit(0);
}

void comienzoJuego(personaje* usuario, Map* grafo)
{
    int ingreso;
    int numero;
    usuario->energia=7;//acuerdate sacar esta variable
    
    while(usuario->energia>0)
    {
        printf("Ingrese el numero de la zona a la cual quiere investigar\n");
        mostrarZonas(grafo);
        scanf("%i",&ingreso);
        
        switch (ingreso)
        {
        case 0:
            printf("menu de opciones\n");
            menuOpciones(usuario, grafo);
            break;
        case 1:
            usuario->energia--;
            finalRandom();
            zonaHabitacionPri(usuario, grafo);
            break;
        case 2:
            usuario->energia--;
            finalRandom();
            zonaBanyo(usuario, grafo);
            break;
        case 3:
            usuario->energia--;
            finalRandom();
            zonaCocina(usuario, grafo);
            break;
        case 4:
            usuario->energia--;
            finalRandom();
            zonaGaraje(usuario, grafo);
            break;            
        case 5:
            usuario->energia--;
            finalRandom();
            zonaHabitacionHijo(usuario, grafo);
            break;
        case 6:
            usuario->energia--;
            finalRandom();
            zonaPatio(usuario, grafo);
            break;
        case 7:
            usuario->energia--;
            finalRandom();
            zonaSotano(usuario, grafo);
            break;    
        default:
            printf("Ha ingresado un numero invalido\n");
            break;
        }
    }
}

void iniciarPartida(personaje* usuario,Map* lugaresZona)
{
    //mostrarInicio(usuario->nombre); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    usuario->energia = 8;
    usuario->pistas = createList();
    usuario->pistasEspeciales = 0;
    definirGrafo(lugaresZona);
}

void mostrapersonajes(FILE* carga)
{
    char *token;
    char aux[100];
    char* aux2;
    printf("PERSONAJES DISPONIBLES\n"); 
    
    while(fgets(aux,100,carga))
    {
        token=strtok(aux,":");
        aux2=strdup(token);
        if(strcmp(aux2,"personaje")==0)
        {
            printf("Personaje: ");
            token=strtok(NULL, "\n");
            printf("%s\n", token);

        }  
    }

}

/*
void cargardatos(FILE* carga,char persona,personaje *usuario)
{
    char *token;
    char aux[100];
    char* aux2;
    printf("PERSONAJES DISPONIBLES\n"); 
    while(fgets(aux,100,carga)!=NULL)
    {
        token=strtok(aux,":");
        aux2=strdup(token);
        if(strcmp(aux2,"personaje")==0)
        {
            
            token=strtok(NULL,"\n");
            aux2=strdup(token);
            if(strcmp(aux2,persona)==0)
            {

            }
        }  
    }
}
*/

void cargarPartida(personaje* usuario,Map* lugaresZona)
{
    FILE* carga;
    char aux[100] = "texto/guardado.txt";
    carga = fopen(aux,"rt");
    definirGrafo(lugaresZona);
    mostrapersonajes(carga);
    getchar();
    fgets(aux,100,stdin);
    fseek(carga,0,SEEK_SET);
    //cargardatos(carga,aux,usuario);
}

void mostrarInfo()
{
    printf("Se te van a dar una cierta cantidad de opciones en las cuales tienes que estar constantemente eligiendo segun el\n");
    printf("numero que tenga la opcion, tambien para poder avanzar los dialogos al inicio y final del juego\n");
}

void menuInicial(personaje *usuario, Map* lugaresZona)
{
    char aux[101];
    int opcion;
    printf("Bienvenido a un juego\n");//inicio
    printf("Menu\n 1.- Nueva Partida\n 2.- Cargar Partida\n 3.- Info del Juego\n");
    scanf("%i", &opcion);
    switch (opcion)
    {
    case 1 :
        printf("Como te llamas?: \n");
        getchar();
        fgets(aux,100,stdin);
        usuario->nombre = strdup(aux);
        iniciarPartida(usuario, lugaresZona);
        break;
    case 2 :
        cargarPartida(usuario, lugaresZona);
        break;
    case 3 :
        mostrarInfo();
        menuInicial(usuario, lugaresZona);
        break;
    case 4:
        exit(0);
    default:
        exit(0);
        break;
    }
}

int main()
{
    personaje *usuario = (personaje*) malloc (sizeof(personaje));
    Map* lugaresZona = createMap(is_equal_string);
    menuInicial(usuario, lugaresZona);
    comienzoJuego(usuario, lugaresZona);
    if(usuario->energia == 0)
    {
        finalJuego(usuario);
        exit(0);
    }
    printf("a");
}


