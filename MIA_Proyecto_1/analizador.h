#ifndef ANALIZADOR_H_INCLUDED
#define ANALIZADOR_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "mount.h"
#include "reporte_grafico.h"
#include "sistema_archivos.h"
#include "lista_users.h"
/* convierte cadenas que contienen mayusculas a minusulas*/

void mayus_cnv_minus_lst(char *lista);

void iniciar_analisis(char *lista,LISTA_MOUNT *const ptr_mount,NODO_USR *const usuario_logeado);

//retorna 1->bin 0->mal
int ejecutar_cat(NODO_USR *usr_logeado,char * path_file);

#endif // ANALIZADOR_H_INCLUDED
