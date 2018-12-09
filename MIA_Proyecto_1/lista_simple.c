
#include "lista_simple.h"

void inicializar_lst_simple(LISTA_SIMPLE *const ini)
{
    ini->primero = NULL;
    ini->ultimo = NULL;
    ini->size = 0;
}

void add_lst_simple(LISTA_SIMPLE *const add,char valor[16])
{
    NODO_SIMPLE *nuevo = (NODO_SIMPLE*)malloc(sizeof(NODO_SIMPLE));
    inicializar_nodo_simple(nuevo,valor);
    mayus_cnv_minus(nuevo->valor);

    if(add->primero == NULL)
    {
        add->primero = nuevo;
        add->ultimo = nuevo;
        add->size++;
    }
    else //agregamos al ultimo
    {
        add->ultimo->siguiente = nuevo;
        add->ultimo = nuevo;
        add->size++;
    }
}


void inicializar_nodo_simple(NODO_SIMPLE *const ini, char valor[16])
{
    ini->siguiente = NULL;
    strcpy(ini->valor,valor);
}


// void mayus_cnv_minus(char *cadena)
// {
//     int i;
//     for(i=0; cadena[i]; i++)
//         cadena[i] = tolower(cadena[i]);
// }
