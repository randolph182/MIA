
#include "lista_comando.h"

void inicializar_lst_comando(LST_COMANDO *const ini)
{
    ini->primero = 0;
    ini->ultimo =0;
    ini->size =0;
}

void add_lst_comando(LST_COMANDO *const add,char valor[250])
{
    NODO_COMANDO *nuevo = (NODO_COMANDO*)malloc(sizeof(NODO_COMANDO));
    inicializar_nodo_comando(nuevo,valor);
    mayus_cnv_minus(nuevo->valor);

    if(add->primero == 0) //si es igual a null
    {
        add->primero = nuevo;
        add->ultimo = nuevo;
        add->size++;
    }
    else //agregamos al ultimo
    {
        add->ultimo->siguiente =  nuevo;
        add->ultimo = nuevo;
        add->size++;
    }

}

void listar_comando(LST_COMANDO *const pt)
{
    if(pt->primero !=0)
    {
        NODO_COMANDO *tmp = pt->primero;
        while(tmp !=0)
        {
            printf("%s\n",tmp->valor);
            tmp = tmp->siguiente;
        }
    }
    else{
        printf("No hay valores en la lista de comandos");
    }
}


void  inicializar_nodo_comando(NODO_COMANDO *const ini,char valor[250])
{
    ini->siguiente =0;
    strcpy(ini->valor,valor);
}


void mayus_cnv_minus(char *cadena)
{
        int i;
        for(i=0; cadena[i];i++)
            cadena[i] = tolower(cadena[i]);
}
