#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"
#include "Map.h"

typedef struct{
    char nombre[30];
    List* caminos;
}Node;

typedef struct{
    char* nombre;
    int visitado;
    int pista1;
    int pista2;
}habitacion;

typedef struct{
    char* nombre;
    char* menu;
}pista;

Node * create(void * data) {
    Node * new = (Node *)malloc(sizeof(Node));
    assert(new != NULL);
    strcpy(new->nombre, data);
    new->caminos = createList();
    return new;
}

int is_equal_string(void * key1, void * key2) 
{
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

int main()
{
    FILE* zonas = fopen("zonas.txt", "rt");
    Map* grafo = createMap(is_equal_string);
    char aux[1024];
    char* token;
    habitacion* principal;
    while(fgets(aux,1024,zonas)!=NULL)
    {
        habitacion* hab=(habitacion*) malloc (sizeof(habitacion));
        token = strtok(aux,"\n");
        hab->nombre = strdup(token);
        hab->visitado = 0;
        hab->pista1 = 0;
        hab->pista2 = 0;
        List* lista = createList();
        if (strcmp(hab->nombre, "Living") == 0)
        {
            insertMap(grafo, hab->nombre, lista);
            principal = hab;
        }
        else
        {
            insertMap(grafo, hab->nombre, lista);
            pushBack(searchMap(grafo, "Living"), hab);
            pushBack(searchMap(grafo, hab->nombre), principal);
        }
    }
    habitacion* test = firstList(searchMap(grafo, "Living"));
    while(test)
    {
        printf("%s\n", test->nombre);
        test = nextList(searchMap(grafo, "Living"));
    }
    test = firstList(searchMap(grafo, "Sotano"));
    printf("%s", test->nombre);
}