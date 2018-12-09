#ifndef LISTA_SIMPLE_H_INCLUDED
#define LISTA_SIMPLE_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <ctype.h>  //tolower
#include <stdio.h>

typedef struct NODO_SIMPLE NODO_SIMPLE;
typedef struct LISTA_SIMPLE LISTA_SIMPLE;

struct NODO_SIMPLE
{
    NODO_SIMPLE *siguiente;
    char valor[16];
};

struct LISTA_SIMPLE
{
    NODO_SIMPLE *primero;
    NODO_SIMPLE *ultimo;
    int size;
};

void inicializar_lst_simple(LISTA_SIMPLE *const ini);

void add_lst_simple(LISTA_SIMPLE *const add,char valor[16]);

void inicializar_nodo_simple(NODO_SIMPLE *const ini, char valor[16]);

//void mayus_cnv_minus(char *cadena);

#endif // LISTA_SIMPLE_H_INCLUDED
