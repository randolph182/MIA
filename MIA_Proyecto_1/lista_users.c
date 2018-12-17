#include "lista_users.h"

void inicializar_lst_usr(LISTA_USR *const ptr_usr)
{
    ptr_usr->primero = NULL;
    ptr_usr->ultimo = NULL;
    ptr_usr->size = 0;
}

void inicializar_nodo_usr(NODO_USR *const ini,int id,char tipo,char *nombre_grupo,char *nombre_usr,char *password_usr)
{
    ini->siguiente = NULL;
    ini->id = id;
    ini->tipo = tipo;
    strcpy(ini->nombre_grupo,nombre_grupo);
    strcpy(ini->nombre_usr,nombre_usr);
    strcpy(ini->password_usr,password_usr);
}

void add_lst_usr(LISTA_USR *const add,int id,char tipo,char *nombre_grupo,char *nombre_usr,char *password_usr)
{
    NODO_USR *nuevo = (NODO_USR*)malloc(sizeof(NODO_USR));
    inicializar_nodo_usr(nuevo,id,tipo,nombre_grupo,nombre_usr,password_usr);
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
