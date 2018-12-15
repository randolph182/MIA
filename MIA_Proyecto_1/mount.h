#ifndef MOUNT_H_INCLUDED
#define MOUNT_H_INCLUDED

#include <string.h>
#include <stdio.h>

typedef struct NODO_MOUNT NODO_MOUNT;
typedef struct LISTA_MOUNT LISTA_MOUNT;

struct NODO_MOUNT
{
    NODO_MOUNT *siguiente;
    char id_mount[16];
    char path_mount[150];
    char name[16];
    char letra;
    int numero;
    int disponible;

};

struct LISTA_MOUNT
{
    NODO_MOUNT *primero;
    NODO_MOUNT *ultimo;
    int size;
};

void inicializar_lst_mount(LISTA_MOUNT *const ptr_mount);

void montar_particion( LISTA_MOUNT *const ptr_mount,char letra,char name[16],int numero,char *path);
void inicializar_nodo_mount(NODO_MOUNT *const nodo_mount,char name[16],char *path,char letra,int numero);

void mayus_cnv_minus_(char *cadena);

char get_letra_mount(char *path,LISTA_MOUNT *const ptr_mount);
int get_numero_mount(char *path,LISTA_MOUNT *const ptr_mount);

int desmontar_particion(char *id_mount, LISTA_MOUNT *ptr_mount);

char * get_path_mount(char *id_mount,LISTA_MOUNT *ptr_mount);

NODO_MOUNT *get_nodo_mount(char *id_mount,LISTA_MOUNT *ptr_mount);


#endif // MOUNT_H_INCLUDED
