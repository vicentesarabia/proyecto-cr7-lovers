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
    char* nombre;
    char* menu;
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

void mostrarinicio(personaje* usuario)
{  //mostrar inicio del juego(de que trata)
     char respuesta[1];
    printf("AHHHHHHHHHHH!!.........\nUn grito alerto a todos los habitantes de la casa.\n");
    printf("Viendose atormentados con la escena, un hombre muerto... el hombre, el duenyo de la familia, esposo de la senyora Buren y hermano del senyor, Thomson.\n");
    printf("Nadie habia entrado, nadie habia salido. No es una alternativa dentro de su inmensa mansion de cientos de hectareas, y llena de seguridad.\n");
    printf("Cuantas personas se encontraban ahi?, el hombre yacia con un cuchillo enterrado en su pecho.\nY cualquiera diria, \"Solo basta con tomar una muestra de ADN para descubrir quien de ellos puede haber sido\", No?.\n");
    printf("Lastima que estamos en 1941 y esas cosas solo aparecen en libros de ciencia ficcion.\nEntonces habria de esperar que la policia hiciera un gran trabajo no?\n");
    printf("pffff... Vivimos en Chile de 1941, no es mucho lo que se puede esperar de estos queridos funcionarios publicos\nAl menos que tu problema sea que tu vecino quiere robarte tu gallina, no cuentes con ellos.\n");
    printf("Ademas, son la familia mas rica y conocida de Valparaiso, no es una noticia la cual la familia Buren quisiera que fuera de conocimiento.\nMas importante que el fallecimiento de ese hombre, es la reputacion como familia.\n");
    printf("Por todo esto, la familia Buren lo ha contactado a usted detective %s.\nEl detective privado mas cotizado y prestigioso de esta region del mundo, para que resuelvas este caso, y descubras al asesino.\n", usuario->nombre);
    printf("Entonces..\nAceptaras este curioso caso?...\nPsdt.. La paga sera bastante grata ;)\nAceptas? s/n: ");
    scanf("%s", respuesta);
    if (strcmp(respuesta, "s") == 0)
    {
        printf("Tienes a los 3 sospechosos adelante tuyo, el primero es el hermano de la victima, Thomson\nSu testimonio es que la noche en la que murio su hermano estaba en el patio trasero arreglando su motocicleta\nUn hombre alto y corpulento, con cabello castanyo, de unos 25 anyos, vive a costa de su hermano por lo que se sospecha que puedo haberlo matado por su fortuna.\n");
        printf("El testimonio de la sirvienta fue que estaba en la cocina haciendo la cena y limpiando la ropa, una bella mujer joven, con pelo negro\nSe sospecha que estaba en un amorio con el senyor Buren por lo que se sospecha que lo pudo haber matado por preferir a su esposa antes que escaparse con ella.\n");
        printf("La ultima sospechosa era la esposa la cual su testimonio fue que ella estaba en la habitacion principal, durmiendo la siesta\nUna mujer de unos 40 anyos, rubia, se sospecha que pudo ser ella para quedarse con toda la herencia.\n");
        printf("Ninguno es testigo de que lo que dicen los sospechoso es real por lo que tendras que dar lo mejor de ti para poder encontrar al asesino sin mandar a un inocente a prision.\n");
        printf("Te pagaron para investigar 8 horas, cada vez que visitas una zona de la casa te toma 1 hora investigar\nSi vuelves a la misma zona te volvera a tomar 1 hora, cuando se te acabe el tiempo tendras que indicar quien es el asesino entre estos 3 sospechos\nVe andando, que el tiempo corre.\n");
    }
    else if (strcmp(respuesta, "n") == 0)
    {
        printf("Decidiste no aceptar el caso, dejando a un asesino libre...\nSeguro que fue la decision correcta...?\n");
        printf("Su reputacion como detective ha sufrido por esto, Detective %s.\nAhora nadie confia en su habilidad...\n",usuario->nombre);
        printf("--------------------FINAL MALO 1: DETECTIVE FLOJO--------------------");
    }
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
    int num=1;
    while(test!=NULL)
    {
        
        printf("%i %s\n",num,test->nombre);
        num++;
        test=(nextList(a->caminos));
    }
}
void comienzojuego(personaje* usuario, Map* grafo)
{
    int ingreso;
    while(usuario->energia!=0)
    {
        printf("ingrese el numero de la zona a la cual quiere investigar\n");
        mostrarzonas(grafo);
        scanf("%i",ingreso);
        
        switch (ingreso)
        {
        case 1:
            printf("nada");
            break;
        
        default:
            break;
        }
    }
}


void iniciarpartida(personaje* usuario,Map* lugaresZona)
{
    mostrarinicio(usuario);
    usuario->energia=8;
    usuario->pistas=createList();
    usuario->pistasEspeciales=0;
    DefinirGrafo(lugaresZona);
    comienzojuego(usuario,lugaresZona);
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
        printf("como te llamas: \n");
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
    //resto de funciones
    
}


