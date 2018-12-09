#ifndef LISTA_COMANDO_H_INCLUDED
#define LISTA_COMANDO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>  //tolower

typedef struct LST_COMANDO LST_COMANDO;
typedef struct NODO_COMANDO NODO_COMANDO;


struct NODO_COMANDO{
    char valor[250];
    NODO_COMANDO *siguiente;
};

struct LST_COMANDO{
    NODO_COMANDO *primero;
    NODO_COMANDO *ultimo;
    int size;
};

/* metodos para la lista de comandos*/
void inicializar_lst_comando(LST_COMANDO *const ini);
void add_lst_comando(LST_COMANDO *const add,char valor[250]);
void listar_comando(LST_COMANDO *const pt);

/* metodos para el nodo de comandos*/
void inicializar_nodo_comando(NODO_COMANDO *const ini,char valor[250]);


/* metodo que sirve para convertir  las cadenas a minusculas */
void Mayus_cnv_Minus(char *cadena);

#endif // LISTA_COMANDO_H_INCLUDED
