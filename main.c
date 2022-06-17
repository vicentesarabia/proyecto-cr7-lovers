#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "Map.h"

typedef struct 
{
    char* nombre;
    int energia;
    List* pistas;
    int pistasEspeciales;
}personaje;

typedef struct
{
    char* idpis;
    char* texto;

}pista;

typedef struct{
    int idzona;
    char* nombreZona;
    short flagsZona;
}zona;


int is_equal_string(void * key1, void * key2) 
{
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}
void mostrarinicio()
{  //mostrar inicio del juego(de que trata)
    int a;

}
void definirzonas(Map* lugaresZonas)
{
    //: 1.habitación principal, 2.banyo de invitados,cocina, living, garaje, habitación del hijo, patio trasero y sótano.
    FILE *zonas;
    char aux[101]="infozonas.txt";
    char *token;
    char *aux2;
    
    zonas=fopen(aux,"rt");
    while(fgets(aux,100,zonas)!=NULL)
    {
        zona* lugar=(zona*)malloc(sizeof(zona));
        char*dump;
        token=strtok(aux,",");
        lugar->idzona=(int) strtol(token, &dump, 10);
        token=strtok(NULL,",");
        lugar->nombreZona=strdup(token);
        token=strtok(NULL,"\n");
        lugar->flagsZona=(int) strtol(token, &dump, 10);
        insertMap(lugaresZonas,lugar->nombreZona,lugar);
    }
}


void definirpistas(Map*mapaPista)
{
    FILE *pistas;
    char aux[1024]="infopistas.txt";
    char *token;
    char *aux2;
    
    pistas=fopen(aux,"rt");
    while(fgets(aux,1024,pistas)!=NULL)
    {
        pista* infopistas=(pista*)malloc(sizeof(pista));
        char*dump;
        token=strtok(aux,",");
        infopistas->idpis=strdup(token);
        token=strtok(NULL,"\n");
        infopistas->texto=strdup(token);
        insertMap(mapaPista,infopistas->idpis,infopistas);
    
    }
    /*
    pista* h=firstMap(mapaPista);
    h=nextMap(mapaPista);
    printf("%s",h->idpis);
    printf("%s",h->texto);
    */


}
void iniciarpartida(personaje* usuario,Map* lugaresZona,Map*mapaPistas)
{
    mostrarinicio();
    usuario->energia=8;
    usuario->pistas=createList();
    usuario->pistasEspeciales=0;
    definirzonas(lugaresZona);
    definirpistas(mapaPistas);
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
            aux2=srtdup(token);
            if(strcmp(aux2,persona)==0)
            {

            }

        }  
    }
    


}
void cargarpartida(personaje* usuario,Map* lugaresZona,Map*mapaPistas)
{
    FILE*carga;
    char aux[100]="guardado.txt";
    carga=fopen(aux,"rt");
    
    definirzonas(lugaresZona);
    definirpistas(mapaPistas);
    mostrapersonajes(carga);
    getchar();
    fgets(aux,100,stdin);
    fseek(carga,0,SEEK_SET);
    cargardatos(carga,aux,usuario);

}

void menuInicial(personaje *usuario, Map* lugaresZona,Map*mapaPista)
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
        iniciarpartida(usuario,lugaresZona,mapaPista);
        
        break;
    case 2 :
        cargarpartida(usuario,lugaresZona,mapaPista);
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
    Map* mapaPistas=createMap(is_equal_string);

    menuInicial(usuario,lugaresZona,mapaPistas);
    //resto de funciones
    
}


