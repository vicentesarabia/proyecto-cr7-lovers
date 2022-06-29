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
    int pis;
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
    char *argv;
    int check = 0;

    argv = "texto/intro1.txt";
    fichero = fopen(argv, "rb");
    while (fgets(c, 1024, fichero)) {
       printf("%s", c);
       check++;
       if (check != 17) getchar();
    }
    printf(" %s", nombre);
    getchar();
   
    fclose (fichero);
    argv = "texto/intro2.txt";
    fichero = fopen(argv, "rb");
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
                argv = "texto/finalinicio.txt";
                fichero = fopen(argv, "rb");
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
/*------------Funcion mostrarZonas--------------*/
//funcion que muestra en orden las zonas a las que puedes acceder
//segun que numero escojas vas a la zona
void mostrarZonas(Map* grafo)
{
    habitacion* a = searchMap(grafo, "Living");
    habitacion* test = (firstList(a->caminos));
    printf("0.- Menu de opciones\n");
    int num = 1;
    while(test)
    {
        if (strcmp(test->nombre, "Banyo de Invitados") == 0)
        {
            printf("%i.- Baño de Invitados\n", num);
            num++;
            test = (nextList(a->caminos));
        }
        else
        {
            printf("%i.- %s\n", num, test->nombre);
            num++;
            test = (nextList(a->caminos));
        }
    }
}
/*--------------Funcion mostrarpistas----------------*/
//funcion que muestra las pistas que tiene el personaje en medio de la partida
void mostrarPistas(personaje* usuario)
{
    printf("\nPistas encontradas\n\n");
    int num = 0;
    if(usuario->pis == 0)//entra si no a encontrado ninguna pistas
    {
        printf("No has encontrado ninguna pista\n");
        return;
    }
    pista* test = firstList(usuario->pistas);
    while(test)//muestra todas la pistas que tiene la lista 
    {
        num++;
        printf("%i.- %s,",num, test->nombre);
        printf("%s\n", test->info);
        test=nextList(usuario->pistas);
    }
    printf("\n");
}
/*-----------Funcion guardarpartida--------------*/
//funcion que guarda la partida del jugador en el archivo datos guardado
void guardarpartida(personaje* usuario, Map*grafo)
{
    FILE* carga = fopen("texto/datos guardado.txt", "r+");
    char *token;
    char aux[1024];
    pista* pistas;
    char* aux2; 
    //while que guarda los datos si es que el personaje ya una partida guardada
    while(fgets(aux,1024,carga))
    {
        token = strtok(aux,":");
        aux2 = strdup(token);
        if(strcmp(aux2,"nombre") == 0)
        {
            token = strtok(NULL,"\n");
            printf("%s-----", token);
            printf("%s\n", usuario->nombre);
            int length = -(strlen(usuario->nombre) + 9);
            if(strcmp(usuario->nombre, token) == 0)
            {
                fseek(carga, length, SEEK_CUR);
                fprintf(carga, "nombre:%s\n", usuario->nombre);
                fprintf(carga, "energia:");
                fprintf(carga, "%i\n", usuario->energia);
                fprintf(carga, "pis:%i\n", usuario->pis);
                if(usuario->pis != 0)
                {
                    pistas = firstList(usuario->pistas);
                    fprintf(carga, "pistas:");
                    while(pistas)
                    {
                        fprintf(carga, "%s,", pistas->nombre);
                        pistas = nextList(usuario->pistas);
                    }
                }
                else fprintf(carga, "pistas:0");
                fprintf(carga, "\n");
                habitacion* a = searchMap(grafo, "Living");
                habitacion* test = (firstList(a->caminos));
                fprintf(carga, "habitaciones:");
                while(test)
                {
                    fprintf(carga,"%i,",test->visitado);
                    test = (nextList(a->caminos));
                } 
                fclose(carga);
                return;
            }
        }  
    }
    //guardar datos de personaje sin una partida guardada
    fprintf(carga,"\n");
    fprintf(carga, "nombre:%s\n", usuario->nombre);
    fprintf(carga, "energia:%i\n", usuario->energia);
    fprintf(carga, "pis:%i\n", usuario->pis);
    if(usuario->pis != 0)
    {
        pistas = firstList(usuario->pistas);
        fprintf(carga, "pistas:");
        while(pistas)
        {
            fprintf(carga, "%s,", pistas->nombre);
            pistas=nextList(usuario->pistas);
        }
    }
    else fprintf(carga, "pistas:0");
    fprintf(carga, "\n");
    habitacion* a = searchMap(grafo, "Living");
    habitacion* test = (firstList(a->caminos));
    fprintf(carga, "habitaciones:");
    while(test)
    {
        fprintf(carga,"%i,",test->visitado);
        test = (nextList(a->caminos));
    } 
    fprintf(carga, "\n");
    fclose(carga);
}
/*---------------Funcion menuopciones----------------*/
//funcion que se ingresa al ingresar un 0 la cual te permite ver las pistas encontradas
//la opcion de guardar partida y la opcion de salir del
void menuOpciones(personaje* usuario,Map* grafo)
{
    int opciones;
    printf("Seleccione una opcion:\n");
    printf("1.- Mostrar Pistas\n2.- Guardar Partida\n3.- Salir del Juego\n");
    scanf("%i", &opciones);
    switch (opciones)
    {
    case 1:
        mostrarPistas(usuario);
        break;
    case 2:
        guardarpartida(usuario,grafo);
        break;  
    case 3:
        exit(0);
        break;  
    default:
        break;
    } 
}
/*---------Funcion zonahabitacionpri----------------------*/
//funcion que ingresa a la zona  habitacion principal y permite realizar
//la opcion de investigar los lugares o regresar a la zona principal
//estructura comentada en zonahabitacionpri ua que son todas identicas
void zonaHabitacionPri(personaje* usuario, Map* grafo)
{
    int input;
    printf("Dónde desea investigar?\n");
    habitacion* a = searchMap(grafo, "Habitacion Principal");
    a->visitado++;//grafo que conecta con el nodo habitacion principal el cual tiene unido las dos pistas de la zona
    pista* p = firstList(a->pistas);//pista uno
    printf("1.- %s\n", p->menu);//accion de pista
    pista* b = nextList(a->pistas);//pista uno
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
                usuario->pis++;
                printf("%s\n", p->info);
                pushBack(usuario->pistas, p);//ingresando a la lista del tipo personaje la pista
            }
            else printf("Ya has revisado esta pista.\n");
            return;
            break;

            case 2:
            if(b->vista == 0)
            {
                b->vista++;
                usuario->pis++;
                printf("%s\n", b->info);
                pushBack(usuario->pistas, b);
            }
            else printf("Ya has revisado esta pista\n");
            return;
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
/*---------Funcion zonabanyo----------------------*/
//funcion que ingresa a la zona  del banyo y permite realizar
//la opcion de investigar los lugares o regresar a la zona principal
//estructura comentada en zonahabitacionpri ua que son todas identicas
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
    while(input != 3)
    {
        switch(input)
        {
            case 1:
            if(p->vista == 0)
            {
                p->vista++;
                usuario->pis++;
                printf("%s\n", p->info);
                pushBack(usuario->pistas, p);
            }
            else printf("Ya has revisado esta pista\n");
            return;
            break;

            case 2:
            if(b->vista == 0)
            {
                b->vista++;
                usuario->pis++;
                printf("%s\n", b->info);
                pushBack(usuario->pistas, b);
            }
            else printf("Ya has revisado esta pista\n");
            return;
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
/*---------Funcion zonacocina----------------------*/
//funcion que ingresa a la zona cocina y permite realizar
//la opcion de investigar los lugares o regresar a la zona principal
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
    while(input != 3)
    {
        switch(input)
            {
            case 1:
            if(p->vista == 0)
            {
                p->vista++;
                usuario->pis++;
                printf("%s\n", p->info);
                pushBack(usuario->pistas, p);
            }
            else printf("Ya has revisado esta pista\n");
            return;
            break;

            case 2:
            if(b->vista == 0)
            {
                b->vista++;
                usuario->pis++;
                printf("%s\n", b->info);
                pushBack(usuario->pistas, b);
            }
            else printf("Ya has revisado esta pista\n");
            return;
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
/*---------Funcion zonagaraje----------------------*/
//funcion que ingresa a la zona garaje y permite realizar
//la opcion de investigar los lugares o regresar a la zona principal
//estructura comentada en zonahabitacionpri ua que son todas identicas
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
    while(input != 3)
    {
        switch(input)
        {
            case 1:
            if(p->vista == 0)
            {
                p->vista++;
                usuario->pis++;
                printf("%s\n", p->info);
                pushBack(usuario->pistas, p);
            }
            else printf("Ya has revisado esta pista\n");
            return;
            break;

            case 2:
            if(b->vista == 0)
            {
                b->vista++;
                usuario->pis++;
                printf("%s\n", b->info);
                pushBack(usuario->pistas, b);
            }
            else printf("Ya has revisado esta pista\n");
            return;
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
/*---------Funcion zonashabitacionhijo----------------------*/
//funcion que ingresa a la zona habitacion del hijo y permite realizar
//la opcion de investigar los lugares o regresar a la zona principal
//estructura comentada en zonahabitacionpri ua que son todas identicas
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
    while(input != 3)
    {
        switch(input)
        {
            case 1:
            if(p->vista == 0)
            {
                p->vista++;
                usuario->pis++;
                printf("%s\n", p->info);
                pushBack(usuario->pistas, p);
            }
            else printf("Ya has revisado esta pista\n");
            return;
            break;

            case 2:
            if(b->vista == 0)
            {
                b->vista++;
                usuario->pis++;
                printf("%s\n", b->info);
                pushBack(usuario->pistas, b);
            }
            else printf("Ya has revisado esta pista\n");
            return;
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
/*---------Funcion zonapatioo----------------------*/
//funcion que ingresa a la zona del patio trasero y permite realizar
//la opcion de investigar los lugares o regresar a la zona principal
//estructura comentada en zonahabitacionpri ua que son todas identicas
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
    while(input != 3)
    {
        switch(input)
        {
            case 1:
            if(p->vista == 0)
            {
                p->vista++;
                usuario->pis++;
                printf("%s\n", p->info);
                pushBack(usuario->pistas, p);
            }
            else printf("Ya has revisado esta pista.\n");
            return;
            break;

            case 2:
            if(b->vista == 0)
            {
                b->vista++;
                usuario->pis++;
                printf("%s\n", b->info);
                pushBack(usuario->pistas, b);
            }
            else printf("Ya has revisado esta pista.\n");
            return;
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
/*---------Funcion zonasotano----------------------*/
//funcion que ingresa a la zona sotano y permite realizar
//la opcion de investigar los lugares o regresar a la zona principal
//estructura comentada en zonahabitacionpri ua que son todas identicas
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
    while(input != 3)
    {
        switch(input)
        {
            case 1:
            if(p->vista == 0)
            {
                p->vista++;
                usuario->pis++;
                printf("%s\n", p->info);
                pushBack(usuario->pistas, p);
            }
            else printf("Ya has revisado esta pista\n");
            return;
            break;

            case 2:
            if(b->vista == 0)
            {
                b->vista++;
                usuario->pis++;
                printf("%s\n", b->info);
                pushBack(usuario->pistas, b);
            }
            else printf("Ya has revisado esta pista\n");
            return;
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

void zona(personaje* usuario, Map* grafo, int eleccion)
{
    int input;
    printf("Donde desea investigar?\n");
}
/*----------------Funcion finalrandom------------*/
//funcion que da un final totalmente al alzar
void finalRandom()
{
    srand(time(NULL));
    int numero = rand() % 51;//numero del 0 al 50 al azar
    if(numero == 15)
    {
        FILE *fichero;
        char c[1024];
        char *argv;
        int check = 0;
        argv = "texto/finalrandom.txt";
        fichero = fopen(argv, "rb");
        while (fgets(c, 1024, fichero)) printf("%s", c);
        exit(0);
    }
}
/*-------------Funcion final------------------*/
//funcion que dependiendo de la eleccion te da un tipo de final diferente
void final(int eleccion)
{
    FILE *fichero;
    char c[1024];
    char *argv;
    switch (eleccion)
    {
        case 1:
        argv = "texto/finalesposa.txt";
        break;

        case 2:
        argv = "texto/finalhermano.txt";
        break;

        case 3:
        argv = "texto/finalsirvienta.txt";
        break;
    }
    fichero = fopen(argv, "rb");
    while (fgets(c, 1024, fichero)) printf("%s", c);//funcion que muestra los textos apretando ENTER
}
/*------------------Funcion finaljuego---------------------------*/
//funcion que se activa cuando el jugador se queda sin energia
void finalJuego(personaje* usuario)
{
    int eleccion = 0, chk = 0;
    printf("Se te ha acabado el tiempo de investigación!\n");
    printf("Quién crees que es asesino de los 3 sospechosos?\n");
    printf("0.- Ver pistas\n1.- Esposa de la víctima\n2.- El hermano de la víctima\n3.- La sirvienta de la casa\n");
    while(eleccion == 0)
    {
        scanf("%i", &eleccion);
        if (eleccion == 0 && chk == 0)
        {
            mostrarPistas(usuario);//funcion que muestar las pistas conseguidas a lo largo del juego
            printf("Quién crees que es asesino de los 3 sospechosos luego de ver tus pistas?\n");
            printf("1.- Esposa de la víctima\n2.- El hermano de la víctima\n3.- La sirvienta de la casa\n");
            chk++;
        }
        else if (eleccion >= 1 || eleccion <= 3) final(eleccion);
    }
    exit(0);
}
/*------------Funcion comienzojuego---------------*/
//funcion inicial o base del juego, por aqui pasan todas las opciones
//y funciones del juego
void comienzoJuego(personaje* usuario, Map* grafo)
{
    int ingreso;
    int numero;
    //while que se rompe cuando el usuario se queda sin energia en el juego por lo que se activa el final del juego
    while(usuario->energia > 0)
    {
        printf("Te quedan %i horas de investigación\n", usuario->energia);
        printf("Ingrese el número de la zona a la cual quiere investigar\n");
        mostrarZonas(grafo);
        scanf("%i", &ingreso);
        //cada opcion de este switch es una zona diferente restandole energia cada vez 
        //que el usuario ingresa a una zona y activando la funcion final random
        switch (ingreso)
        {
        case 0:
            printf("Menú de Opciones\n");
            menuOpciones(usuario, grafo);
            break;
        case 1:
            usuario->energia--;
            finalRandom();
            zonaHabitacionPri(usuario, grafo);//(vamos a comentar solo una funcion de las zonas porque tienen todas la misma estructura dentro)
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
            printf("Ha ingresado un número inválido\n");
            break;
        }
    }
}
/*-------------------Funcion iniciarpartida---------------*/
//funcion que inicializa todos los datos que seran utilizados a lo largo del juego
//como el grafo de zonas y los datos del personaje
void iniciarPartida(personaje* usuario,Map* lugaresZona)
{
    //mostrarInicio(usuario->nombre); //descomentar funcion para mostrar historia
    usuario->energia = 7;
    usuario->pistas = createList();
    usuario->pis = 0;
    definirGrafo(lugaresZona);
}
/*--------------Funcion mostrarpersonajes-----------*/
//funcion que muestra los personajes disponibles para poder cargar la partida
void mostrarPersonajes(FILE* carga)
{
    char *token;
    char aux[1024];
    char* aux2;
    printf("PERSONAJES DISPONIBLES\n"); 
    
    while(fgets(aux,1024,carga))
    {
        token = strtok(aux,":");
        aux2 = strdup(token);
        if(strcmp(aux2,"personaje") == 0)
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
/*--------------Funcion cargarpartida(no funciona)----------*/
//funcion que carga los datos del juego de un previo juego
void cargarPartida(personaje* usuario,Map* lugaresZona)
{
    FILE* carga;
    char aux[100] = "texto/guardado.txt";
    carga = fopen(aux, "rt");
    definirGrafo(lugaresZona);
    mostrarPersonajes(carga);
    getchar();
    fgets(aux,100,stdin);
    fseek(carga,0,SEEK_SET);
    //cargardatos(carga,aux,usuario);
}
/*---------------------Funcion mostrar info------------------*/
//funcion que muestra como jugar al juego, la opciones que tiene y como saltar 
//los comentarios del juego
void mostrarInfo()
{
    printf("Se te van a dar una cierta cantidad de opciones en las cuales tienes que estar constantemente eligiendo segun el\n");
    printf("número que tenga la opción, tambien para poder avanzar los diálogos al inicio y final del juego\n");
}
/*------------------Funcion menu principal------------------*/
//funcion que contienen las opciones de nueva partida, cargar partida(no funciona)
//ver info, al ingresar a una nueva partida,te pedira que ingreses tu nombre para 
//poder hacer juego con el resto del juego
void menuInicial(personaje *usuario, Map* lugaresZona)
{
    char aux[101];
    int opcion;
    printf("Bienvenido a un juego\n");//inicio
    printf("Menú\n 1.- Nueva Partida\n 2.- Cargar Partida\n 3.- Info del Juego\n");
    scanf("%i", &opcion);
    switch (opcion)
    {
    case 1 :
        printf("Como te llamas?: \n");
        getchar();
        fgets(aux,100,stdin);
        usuario->nombre = strdup(aux);
        usuario->nombre[strcspn(usuario->nombre, "\n" )] = '\0';
        iniciarPartida(usuario, lugaresZona);
        break;
    case 2 :
        cargarPartida(usuario, lugaresZona);
        break;
    case 3 :
        mostrarInfo();//mostrar informacion de como jugar el juego
        menuInicial(usuario, lugaresZona);//volver a elegir una opcion 
        break;
    case 4:
        exit(0);
    default:
        exit(0);
        break;
    }
}
/*--------------------Funcion main-----------------------*/
//funcion que contiene todas las funciones del juego
int main()
{
    personaje *usuario = (personaje*) malloc (sizeof(personaje));//tipo de dato que guarda los datos del juegador a lo largo de la partida
    Map* lugaresZona = createMap(is_equal_string);//grafo con las zonas del juego
    menuInicial(usuario, lugaresZona);
    menuInicial(usuario, lugaresZona);
    comienzoJuego(usuario, lugaresZona);
    if(usuario->energia == 0)
    {
        finalJuego(usuario);
        exit(0);
    }
    exit(0);
}


