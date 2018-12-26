#include "lista_users.h"

void inicializar_lst_usr(LISTA_USR *const ptr_usr)
{
    ptr_usr->primero = NULL;
    ptr_usr->ultimo = NULL;
    ptr_usr->size = 0;
}

void inicializar_nodo_usr(NODO_USR *const ini,int id,int ini_part,int size_part,char tipo,char *nombre_grupo,char *nombre_usr,char *password_usr,char *path)
{
    ini->siguiente = NULL;
    ini->id = id;
    ini->id_grp =0;
    ini->inicio_particion = ini_part;
    ini->size_particion = size_part;
    ini->tipo = tipo;
    strcpy(ini->nombre_grupo,nombre_grupo);
    strcpy(ini->nombre_usr,nombre_usr);
    strcpy(ini->password_usr,password_usr);
    strcpy(ini->path_particion,path);
}

void add_lst_usr(LISTA_USR *const add,int id,int ini_part,int size_part,char tipo,char *nombre_grupo,char *nombre_usr,char *password_usr,char *path)
{
    NODO_USR *nuevo = (NODO_USR*)malloc(sizeof(NODO_USR));
    inicializar_nodo_usr(nuevo,id,ini_part,size_part,tipo,nombre_grupo,nombre_usr,password_usr,path);
    if(add->primero == NULL)
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

void add_lst_usr_cadena(LISTA_USR *const add,char *cadena,int ini_part, int size_part,char *path)
{
    //lso tipos de orden que hay
    //GID, Tipo, Grupo
    //UID, Tipo, Grupo, Usuario, Contraseña
    int id;
    int inicio_particion = 0;
    int size_particion = 0;
    char tipo = '0';
    char nombre_grupo[10] = "0";
    char nombre_usr[10] = "0";
    char password_usr[10] = "0";

    const char delim[2] = ",";
    char *token;
    int orden =1;
    //obteniendo el primer token //seria el numero
    token = strtok(cadena,delim);
    id = atoi(token);
    orden++;
    //obteniendo los demas tokens

    while(token !=NULL)
    {
        token = strtok(NULL,delim);
        if(token == NULL)
        {
            break;
        }
        if(orden == 2)
        {
            tipo = token[0];
            orden++;
        }
        else if(orden == 3)
        {
            strcpy(nombre_grupo,token);
            orden++;
        }
        else if(orden == 4)
        {
            strcpy(nombre_usr,token);
            orden++;
        }
        else if(orden == 5)
        {
            strcpy(password_usr,token);
            orden++;
        }
    }
    inicio_particion = ini_part;
    size_particion = size_part;
    add_lst_usr(add,id,inicio_particion,size_particion,tipo,nombre_grupo,nombre_usr,password_usr,path);
}

NODO_USR *usuario_login(LISTA_USR *const usrs,char *nombre,char *password)
{
    NODO_USR *tmp = usrs->primero;
    while(tmp !=NULL)
    {
        if(tmp->id !=0 && tmp->tipo == 'U') //si esta activo el usuario y es tipo usuario
        {
            if(strcmp(tmp->nombre_usr,nombre) == 0)
            {
                if(strcmp(tmp->password_usr,password) ==0)
                    return tmp;
            }
        }

        tmp = tmp->siguiente;
    }
    return NULL;
}

//si retorn 0 no existe si retorna 1 si existe
int verif_usr_rep_grp(LISTA_USR *const usrs,char *nombre_usr,char *nombre_grp)
{
    NODO_USR *tmp = usrs->primero;
    while(tmp !=NULL)
    {
        if(tmp->id !=0 && tmp->tipo == 'U') //si esta activo el usuario y es tipo usuario
        {
            if(strcmp(tmp->nombre_grupo,nombre_grp) ==0)
            {
                if(strcmp(tmp->nombre_usr,nombre_usr) == 0)
                    return 1; //si existe
            }
        }

        tmp = tmp->siguiente;
    }
    return 0; //no existe
}

int verificar_grp_existe(LISTA_USR *const usrs,char *nombre_grp)
{
    NODO_USR *tmp = usrs->primero;
    while(tmp !=NULL)
    {
        if(tmp->id !=0 && tmp->tipo == 'G') //si esta activo el usuario y es tipo usuario
        {
            if(strcmp(tmp->nombre_grupo,nombre_grp) ==0)
                return 1; //si existe
        }

        tmp = tmp->siguiente;
    }
    return 0; //no existe
}

int get_id_grp(LISTA_USR *const usrs,char * grp_usr)
{
    NODO_USR *tmp = usrs->primero;
    while(tmp !=NULL)
    {
        if(tmp->id !=0 && tmp->tipo == 'G') //si esta activo el usuario y es tipo usuario
        {
            if(strcmp(tmp->nombre_grupo,grp_usr)==0)
                return tmp->id;
        }
        tmp = tmp->siguiente;
    }
    return 0; //no existe
}

//retorna 1 o 0 si jala
int get_nombre_grp_by_id(LISTA_USR *const lista, int idgrp,char *name_grupo)
{
    NODO_USR *tmp = lista->primero;
    while(tmp !=NULL)
    {
        if(tmp->id !=0 && tmp->tipo == 'G') //si esta activo el usuario y es tipo usuario
        {
            if(tmp->id == idgrp)
            {
                strcpy(&*name_grupo,tmp->nombre_grupo);
                return 1;
            }
        }
        tmp = tmp->siguiente;
    }
    return 0; //no existe
}

//retorna 1 o 0 si jala
int get_nombre_usr_by_id(LISTA_USR *const lista, int idusr,char *name_usr)
{
    NODO_USR *tmp = lista->primero;
    while(tmp !=NULL)
    {
        if(tmp->id !=0 && tmp->tipo == 'U') //si esta activo el usuario y es tipo usuario
        {
            if(tmp->id == idusr)
            {
                strcpy(&*name_usr,tmp->nombre_usr);
                return 1;
            }
        }
        tmp = tmp->siguiente;
    }
    return 0; //no existe
}
