#ifndef REPORTE_GRAFICO_H_INCLUDED
#define REPORTE_GRAFICO_H_INCLUDED
#include <string.h>
#include <stdio.h>
#include "admin_disco.h"


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

#endif // REPORTE_GRAFICO_H_INCLUDED
