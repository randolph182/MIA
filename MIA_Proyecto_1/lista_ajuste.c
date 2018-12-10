#include "lista_ajuste.h"

void inicializar_lst_ajuste(LISTA_AJUSTE *const ptr_ajuste)
{
    ptr_ajuste->primero = NULL;
    ptr_ajuste->size =0;
    ptr_ajuste->ultimo = NULL;
}

void inicializar_nodo_ajuste(NODO_AJUSTE *const ini, int t_inicio,int t_total)
{
    ini->siguiente = NULL;
    ini->tamanio_inicio = t_inicio;
    ini->tamanio_total = t_total;
}

void add_lst_ajuste(LISTA_AJUSTE *const add,int t_inicio,int t_total)
{
    NODO_AJUSTE *nuevo = (NODO_AJUSTE*)malloc(sizeof(NODO_AJUSTE));
    inicializar_nodo_ajuste(nuevo,t_inicio,t_total);

    if(add->primero ==NULL)
    {
        add->primero = nuevo;
        add->ultimo = nuevo;
        add->size++;
    }
    else
    {
        add->ultimo->siguiente = nuevo;
        add->ultimo = nuevo;
        add->size++;
    }
}

void ordenar_menor_mayor(LISTA_AJUSTE *const lista)
{
    if(lista->size > 0)
    {
        if(lista->size != 1)
        {

            for(int i = 0; i < lista->size; ++i)
            {
                NODO_AJUSTE *tmp = lista->primero;
                while(tmp !=NULL)
                {
                    if(tmp->siguiente !=NULL)
                    {
                        int result = tmp->tamanio_total - tmp->siguiente->tamanio_total;
                        if(result > 0) //es porque el valor del nodo siguiente es menor que el actual
                        {
                            int ini_tmp = tmp->tamanio_inicio;
                            int t_tmp = tmp->tamanio_total;

                            NODO_AJUSTE *aux = (NODO_AJUSTE*)malloc(sizeof(NODO_AJUSTE));
                            aux->tamanio_inicio = tmp->siguiente->tamanio_inicio;
                            aux->tamanio_total = tmp->siguiente->tamanio_total;

                            tmp->siguiente->tamanio_inicio = tmp->tamanio_inicio;
                            tmp->siguiente->tamanio_total = tmp->tamanio_total;

                            tmp->tamanio_inicio = aux->tamanio_inicio;
                            tmp->tamanio_total = aux->tamanio_total;

                            int ini_tmp2 = tmp->tamanio_inicio;
                            int t_tmp2 = tmp->tamanio_total;

                            continue;
                        }
                    }
                    tmp = tmp->siguiente;
                }
            }
        }
    }
}

void ordenar_mayor_menor(LISTA_AJUSTE *const lista)
{
    if(lista->size > 0)
    {
        if(lista->size != 1)
        {

            for(int i = 0; i < lista->size; ++i)
            {
                NODO_AJUSTE *tmp = lista->primero;
                while(tmp !=NULL)
                {
                    if(tmp->siguiente !=NULL)
                    {
                        int result = tmp->tamanio_total - tmp->siguiente->tamanio_total;
                        if(result < 0) //es porque el valor del nodo siguiente es mayor que el actual
                        {
                            int ini_tmp = tmp->tamanio_inicio;
                            int t_tmp = tmp->tamanio_total;

                            NODO_AJUSTE *aux = (NODO_AJUSTE*)malloc(sizeof(NODO_AJUSTE));
                            aux->tamanio_inicio = tmp->siguiente->tamanio_inicio;
                            aux->tamanio_total = tmp->siguiente->tamanio_total;

                            tmp->siguiente->tamanio_inicio = tmp->tamanio_inicio;
                            tmp->siguiente->tamanio_total = tmp->tamanio_total;

                            tmp->tamanio_inicio = aux->tamanio_inicio;
                            tmp->tamanio_total = aux->tamanio_total;

                            int ini_tmp2 = tmp->tamanio_inicio;
                            int t_tmp2 = tmp->tamanio_total;

                            continue;
                        }
                    }
                    tmp = tmp->siguiente;
                }
            }
        }
    }
}
