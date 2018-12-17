#include <stdio.h>
#include <stdlib.h>
#include <string.h> //permite los NULLS
#include "analizador.h"

#include <unistd.h>

#include "mount.h"
#include "lista_users.h"
int main()
{

    LISTA_MOUNT *ptr_mount = (LISTA_MOUNT*)malloc(sizeof(LISTA_MOUNT));
    inicializar_lst_mount(ptr_mount);
    NODO_USR * usuario_logeado = (NODO_USR*)malloc(sizeof(NODO_USR));
    usuario_logeado->id =0;// = NULL;
    /* Entrada por teclado de las sentencias*/
    char *comando;
    int flag =0;
    while(flag != 1)
    {
        printf("\n\nIngrese un comando o ingrese 1 para salir: \n");
        comando = (char*)malloc(sizeof(char)*1024);
        memset(comando,0,sizeof(comando));
        fgets(comando,1024,stdin);
        if(comando[0] == '1'){
            flag = 1;
            break;
        }
        if(comando[0] == '#') // para los comentarios
        {
            int index_lst =0;
            while(comando[index_lst] != '\n')
            {
                index_lst++;
                continue;
            }
        }
        else
            iniciar_analisis(comando,ptr_mount,usuario_logeado);

        free(comando);
        comando =NULL;

    }

    return 0;
}
