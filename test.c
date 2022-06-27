#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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

int is_equal_string(void * key1, void * key2) 
{
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

void creacionGrafo(Map* grafo, FILE* zonas)
{
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
}

void creacionPistas(FILE* pistas, Map* grafo)
{
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

int main()
{
    FILE* creacion = fopen("zonas.txt", "rt");
    Map* grafo = createMap(is_equal_string);
    creacionGrafo(grafo, creacion);
    fclose(creacion);
    creacion = fopen("pistas.txt", "rt");
    creacionPistas(creacion, grafo);
    
}