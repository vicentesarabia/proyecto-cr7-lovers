#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "Map.h"

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

    argv = "intro1.txt";
    fichero = fopen (argv, "rb");
    while (fgets(c, 1024, fichero)) {
       printf ("%s", c);
       check++;
       if (check != 17) getchar();
    }
    printf(" %s", nombre);
    getchar();
   
    fclose (fichero);
    argv = "intro2.txt";
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
                argv = "malfinal1.txt";
                fichero = fopen (argv, "rb");
                while (fgets(c, 1024, fichero)) {
                    printf("%s", c);
                    getchar();
                }
            }
            break;
       }
    }
    fclose (fichero);
}

void insertpist(Map* grafo)
{
    FILE* pistas = fopen("pistas.txt", "rt");
    char aux[1024];
    char* token;
    while(fgets(aux,1024,pistas)!=NULL)
    {
        pista* p = (pista*) malloc (sizeof(pista));
        token = strtok(aux,",");
        p->nombre = strdup(token);
        token = strtok(NULL, ",");
        p->menu = strdup(token);
        p->vista = 0;
        token = strtok(NULL, "\n");
        habitacion * res = searchMap(grafo, token);
        pushBack(res->pistas, p);
    }
}

void DefinirGrafo(Map* grafo)
{
    FILE* zonas = fopen("zonas.txt", "rt");
    char aux[1024];
    char* token;
    habitacion* principal;
    while(fgets(aux,1024,zonas)!=NULL)
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
    insertpist(grafo);
}

void mostrarzonas(Map* grafo)
{
    habitacion* a=searchMap(grafo,"Living");
    habitacion* test=(firstList(a->caminos));
    printf("0-menu de opciones\n");
    int num=1;
    while(test!=NULL)
    {
        
        printf("%i-%s\n",num,test->nombre);
        num++;
        test=(nextList(a->caminos));
    }
}
void mostrarpistas(personaje* usuario)
{
    printf("listas encontradas\n");
    pista* test=firstList(usuario->pistas);
    while(test)
    {
        printf("%s\n",test->nombre);
        printf("%s\n",test->info);
    }

}
void menuOpciones(personaje* usuario,Map* grafo)
{
    int opciones;
    printf("seleccione la opcion\n");
    printf("1-mostrar pistas\n2-Guardar partida\n3-Salir del juego\n");
    scanf("%i",&opciones);
    switch (opciones)
    {
    case 1:
        mostrarpistas(usuario);
        break;
    case 2:
        //guardarpartida(usuario,grafo);
        break;  
    case 3:
        exit(0);
        break;  
    default:
        break;
    }

    

}
void comienzojuego(personaje* usuario, Map* grafo)
{
    int ingreso;
    usuario->energia=2;//acuerdate sacar esta variable
    
    while(usuario->energia>0)
    {
        printf("ingrese el numero de la zona a la cual quiere investigar\n");
        mostrarzonas(grafo);
        scanf("%i",&ingreso);
        
        switch (ingreso)
        {
        case 0:
            printf("menu de opciones\n");
            menuOpciones(usuario,grafo);
            break;
        case 1:
            usuario->energia--;
            //zonahabitacionpri(usuario,grafo);
            break;
        case 2:
            usuario->energia--;
            //zonabanyo(usuario,grafo);
            break;
        case 3:
            usuario->energia--;
            //zonacocina(usuario,grafo);
            break;
        case 4:
            usuario->energia--;
            //zonagaraje(usuario,grafo);
            break;            
        case 5:
            usuario->energia--;
            //zonahabitacionhijo(usuario,grafo);
            break;
        case 6:
            usuario->energia--;
            //zonapatio(usuario,grafo);
            break;
        case 7:
            usuario->energia--;
            //zonasotano(usuario,grafo);
            break;    
        default:
            printf("a ingresado un numero invalido\n");
            break;
        }
    }
}


void iniciarpartida(personaje* usuario,Map* lugaresZona)
{
    //mostrarInicio(usuario->nombre); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    usuario->energia=8;
    usuario->pistas=createList();
    usuario->pistasEspeciales=0;
    DefinirGrafo(lugaresZona);
   
}

void mostrapersonajes(FILE* carga)
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
            printf("personaje: ");
            token=strtok(NULL,"\n");
            printf("%s\n",token);

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
void cargarpartida(personaje* usuario,Map* lugaresZona)
{
    FILE*carga;
    char aux[100]="guardado.txt";
    carga=fopen(aux,"rt");
    DefinirGrafo(lugaresZona);
    mostrapersonajes(carga);
    getchar();
    fgets(aux,100,stdin);
    fseek(carga,0,SEEK_SET);
    //cargardatos(carga,aux,usuario);
}

void menuInicial(personaje *usuario, Map* lugaresZona)
{
    char aux[101];
    int opcion;
    printf("bienvenido a un juego ql malo\n");//inicio
    printf("menu\n 1.nueva partida\n 2.cargar partida\n 3.info del juego\n");
    scanf("%i",&opcion);
    switch (opcion)
    {
    case 1 :
        printf("Como te llamas?: \n");
        getchar();
        fgets(aux,100,stdin);
        usuario->nombre=strdup(aux);
        iniciarpartida(usuario,lugaresZona);
        
        break;
    case 2 :
        cargarpartida(usuario,lugaresZona);
        break;
    case 3 :
        printf("weas friki de info");
        //mostrarinfo();
        break;
    
    default:
        break;
    }
}

int main()
{
    personaje *usuario=(personaje*)malloc(sizeof(personaje));
    Map* lugaresZona=createMap(is_equal_string);
    menuInicial(usuario,lugaresZona);
    comienzojuego(usuario,lugaresZona);
    printf("a");
    //resto de funciones
    
}


