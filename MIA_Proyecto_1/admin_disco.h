#ifndef ADMIN_DISCO_H_INCLUDED
#define ADMIN_DISCO_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

#include "mount.h"
#include "lista_ajuste.h"

typedef struct MASTER_BOOT_RECORD MBR;
typedef struct PARTICION PTR;
typedef struct EXTENDED_BOOT_RECORD EBR;

struct PARTICION
{
    char part_status;// = '0';
    char part_type;
    char part_fit[3];
    int  part_start;
    int  part_size;
    char part_name[16];
};

struct MASTER_BOOT_RECORD
{
    int mbr_tamanio_disk;
    char mbr_fecha_creacion[16];
    int mbr_disk_segnature;
    char disk_fit[3];
    PTR mbr_partition_1;
    PTR mbr_partition_2;
    PTR mbr_partition_3;
    PTR mbr_partition_4;
};

struct EXTENDED_BOOT_RECORD
{
  char part_status;// = '0'; //1
  char part_fit[3];       //3
  int  part_start;        //4
  int  part_size;         //4
  int part_next;          //4
  char part_name[16];     //16
                         //-----
                         //31 bytes
};


void crearDisco(int size,char unit, char *path,char *disk_fit);
void eliminar_disco(char *path);

void crear_particion(int size,char unit,char type,char fit[],char *path, char *name);
void crear_particion_logica(MBR *mbr,int size,char unit,char fit[],char *path,char *name);
void buscar_espacio_logica(EBR *ebr_actual,char *path,int inicio_disponible,int size_nuevo,int size_ext,LISTA_AJUSTE *const ptr_ajuste);


void crear_particion_(MBR *mbr,int size,char unit,char type,char fit[],char *path,char *name);
void buscar_particion_dispo(MBR *mbr,PTR **particion,int size_buscado,int *flag_ptr,int *ini_ptr,char *name,char type);
int buscar_particion_disponible(MBR *mbr,char *name,PTR **particion,char type);
int buscar_espacio_adecuado(MBR *mbr,int size_buscado,int *ini_ptr);




void buscar_espacio_disco(PTR particion_mbr[],int itera,int inicio_disponible,int size_nuevo,MBR *mbr,LISTA_AJUSTE *const ptr_ajuste);
int buscar_espacio_ff_disco(PTR particion_mbr[],int itera,int *inicio_disponible,int size_nuevo,MBR *mbr);

void ordenar_particiones(PTR *particion);
void buscar_espacio(PTR **particion,PTR particion_mbr[],int itera,int *inicio_libre,int size_nuevo,MBR *mbr,int *flag);
void ptr_libre(MBR *mbr,PTR **particion,int *flag);

void delete_particion(char *tipoDel,char *name,char *path_archivo);

void montar_disco(char *path,char *name,LISTA_MOUNT *const ptr_mount);

void ejec_mkdisk(int size, char *path, char unit,char *disk_fit);
void crear_mbr(char *path,char *d_fit);
int verif_carpeta(char *path);
int crear_carpeta(char *path);
int verificar_extension(char *path, int index_path);
int limpiar_path(char *path);

int numero_bytes(char unit,int size);



#endif // ADMIN_DISCO_H_INCLUDED
