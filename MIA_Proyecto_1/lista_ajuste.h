#ifndef LISTA_AJUSTE_H_INCLUDED
#define LISTA_AJUSTE_H_INCLUDED

#include <string.h>
#include <stdio.h>

typedef struct NODO_AJUSTE NODO_AJUSTE;
typedef struct LISTA_AJUSTE  LISTA_AJUSTE;

struct NODO_AJUSTE
{
    NODO_AJUSTE *siguiente;
    int tamanio_inicio;
    int tamanio_total;
};

struct LISTA_AJUSTE
{
    NODO_AJUSTE *primero;
    NODO_AJUSTE *ultimo;
    int size;
};

void inicializar_lst_ajuste(LISTA_AJUSTE *const ptr_ajuste);
void inicializar_nodo_ajuste(NODO_AJUSTE *const ini, int t_inicio,int t_total);

void add_lst_ajuste(LISTA_AJUSTE *const add,int t_inicio,int t_total);

void ordenar_menor_mayor(LISTA_AJUSTE *const lista);
void ordenar_mayor_menor(LISTA_AJUSTE *const lista);

#endif // LISTA_AJUSTE_H_INCLUDED
