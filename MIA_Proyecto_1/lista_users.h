#ifndef LISTA_USERS_H_INCLUDED
#define LISTA_USERS_H_INCLUDED

#include <string.h>
#include <stdio.h>

typedef struct NODO_USR NODO_USR;
typedef struct LISTA_USR LISTA_USR;

struct NODO_USR
{
    int id;
    char tipo;
    char nombre_grupo[10];
    char nombre_usr[10];
    char password_usr[10];

    NODO_USR *siguiente;

};

struct LISTA_USR
{
    NODO_USR *primero;
    NODO_USR *ultimo;
    int size;
};
void inicializar_lst_usr(LISTA_USR *const ptr_usr);
void inicializar_nodo_usr(NODO_USR *const ini,int id,char tipo,char *nombre_grupo,char *nombre_usr,char *password_usr);

void add_lst_usr(LISTA_USR *const add,int id,char tipo,char *nombre_grupo,char *nombre_usr,char *password_usr);

#endif // LISTA_USERS_H_INCLUDED
