#ifndef REPORTE_GRAFICO_H_INCLUDED
#define REPORTE_GRAFICO_H_INCLUDED
#include <string.h>
#include <stdio.h>
#include "admin_disco.h"
#include "lista_users.h"

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

//crear,verificar,limpiar path carpeta
int cvl_path_carpeta(char *path_archivo,char **archivo_dot,char**extension);

void reporte_mbr(char *path_archivo,char *path_disco,char *id);
void reporte_mbr_(char *path_archivo,char *path_disco, char *path_archivo_destino,char *id,char *extension);
void acum_ptr_mbr(FILE *archivo,PTR particion[],int itera);
void concat_2_val(FILE *archivo,char info1[],char info2[],int tipo);

void reporte_disk(char *path_archivo,char *path_disco,char *id);
void reporte_disk_(char *path_archivo,char *path_disco, char *path_archivo_destino,char *id,char *extension);
void acum_ptr_disk(FILE *archivo,PTR particion[],int itera,int t_dsk,int acumulador);

void concatenar_particion(int sizePtr,char *acum,PTR particiones[],int itera,int prtConcat,char *enlace,char *rs);

int sacar_porcentaje(int val_disco, int val_actual);

//::::::::::::::::::::::: METODOS USADOS PARA EL TREE:::::::::::::::::::::::
void reporteTree(FILE *archivo,int ini_particion, char *path_reporte);
void reporteTree_(FILE *archivo,int ini_particion,char *path_dot,char *path_reporte,char *extension);
void escribir_inodo(FILE * archivo,FILE *archivo_dot,int ini_particion,int bm_bloque,int bm_inodo);
void escribir_bloque_carpeta(FILE * archivo,FILE *archivo_dot,int ini_particion,int bm_inodo,int bm_bloque);
void escribir_bloque_archivo(FILE * archivo,FILE *archivo_dot,int ini_particion,int bm_inodo,int bm_bloque);

//::::::::::::::::::::::: METODOS USADOS PARA EL REPO INODE:::::::::::::::::::::::
void reporteInode(FILE *archivo,int ini_particion, char *path_reporte);
void reporteInode_(FILE *archivo,int ini_particion,char *path_dot,char *path_reporte,char *extension);
void escribir_inodo_tipo2(FILE * archivo,FILE *archivo_dot,int ini_particion,int bm_inodo_anterior,int bm_inodo_actual);

//::::::::::::::::::::::: METODOS USADOS PARA EL REPO DE BM DE INODO:::::::::::::::::::::::
void reporte_bm_inodo(FILE *archivo,int ini_particion, char *path_reporte);
void reporte_bm_inodo_(FILE *archivo,int ini_particion,char *path_reporte);

//::::::::::::::::::::::: METODOS USADOS PARA EL REPO DE BM DE BLOQUE:::::::::::::::::::::::
void reporte_bm_bloque(FILE *archivo,int ini_particion, char *path_reporte);
void reporte_bm_bloque_(FILE *archivo,int ini_particion,char *path_reporte);

//::::::::::::::::::::::: METODOS USADOS PARA EL REPO  DE JOURNALING:::::::::::::::::::::::
void reporteLOG(FILE *archivo,int ini_particion, char *path_reporte);
void reporteLOG_(FILE *archivo,int ini_particion,char *path_dot,char *path_reporte,char *extension);
void escribir_journal(FILE *archivo,FILE *archivo_dot,int ini_particion,int pos_log,int bit_anterior,int bit_actual);

//::::::::::::::::::::::: METODOS USADOS PARA EL REPO DE SUPERBLOQUE:::::::::::::::::::::::
void reporte_sb(FILE *archivo,int ini_particion, char *path_reporte);
void reporte_sb_(FILE *archivo,char *path_dot,int ini_particion,char *path_reporte,char * extension);

//::::::::::::::::::::::: METODOS USADOS PARA EL REPO FILE :::::::::::::::::::::::

void reporteFile(FILE *archivo,int ini_particion, char *path_reporte,int bm_ino_arch);
void reporteFile_(FILE *archivo,int ini_particion,char *path_reporte,int bm_ino_arch);

void escribir_cont_archivo(FILE *archivo,FILE *archivo_repo,int ini_particion,int bm_ino_arch);




#endif // REPORTE_GRAFICO_H_INCLUDED
