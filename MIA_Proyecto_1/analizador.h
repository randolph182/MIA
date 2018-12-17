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

void iniciar_analisis(char *lista , LISTA_MOUNT *const ptr_mount);


#endif // ANALIZADOR_H_INCLUDED
