#ifndef LISTA_USERS_H_INCLUDED
#define LISTA_USERS_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct NODO_USR NODO_USR;
typedef struct LISTA_USR LISTA_USR;

struct NODO_USR
{
    int id;
    int id_grp;
    int inicio_particion;
    int size_particion;
    char tipo;
    char nombre_grupo[10];
    char nombre_usr[10];
    char password_usr[10];
    char path_particion[200];
    NODO_USR *siguiente;

};

struct LISTA_USR
{
    NODO_USR *primero;
    NODO_USR *ultimo;
    int size;
};
void inicializar_lst_usr(LISTA_USR *const ptr_usr);
void inicializar_nodo_usr(NODO_USR *const ini,int id,int ini_part,int size_part,char tipo,char *nombre_grupo,char *nombre_usr,char *password_usr,char *path);

void add_lst_usr(LISTA_USR *const add,int id,int ini_part,int size_part,char tipo,char *nombre_grupo,char *nombre_usr,char *password_usr,char *path);

void add_lst_usr_cadena(LISTA_USR *const add,char *cadena,int ini_part, int size_part,char *path);

NODO_USR *usuario_login(LISTA_USR *const usrs,char *nombre,char *password);

int verif_usr_rep_grp(LISTA_USR *const usrs,char *nombre_usr,char *nombre_grp);
int verificar_grp_existe(LISTA_USR *const usrs,char *nombre_grp);

int get_id_grp(LISTA_USR *const usrs,char * grp_usr);
#endif // LISTA_USERS_H_INCLUDED
