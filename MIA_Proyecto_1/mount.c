
#include "mount.h"

void inicializar_lst_mount(LISTA_MOUNT *const ptr_mount)
{
    ptr_mount->primero = NULL;
    ptr_mount->size =0;
    ptr_mount->ultimo = NULL;
}


void montar_particion( LISTA_MOUNT *const ptr_mount,char letra,char name[16],int numero,char *path)
{
    NODO_MOUNT *nuevo = (NODO_MOUNT*)malloc(sizeof(NODO_MOUNT));
    inicializar_nodo_mount(nuevo,name,path,letra,numero);

    if(ptr_mount->primero == NULL)
    {
        ptr_mount->primero = nuevo;
        ptr_mount->ultimo = nuevo;
        ptr_mount->size++;
        printf("Se ha montado la particion con id: %s\n\n",nuevo->id_mount);
    }
    else
    {
        ptr_mount->ultimo->siguiente = nuevo;
        ptr_mount->ultimo = nuevo;
        ptr_mount->size++;
        printf("Se ha montado la particion con id: %s\n\n",nuevo->id_mount);
    }
}

void inicializar_nodo_mount(NODO_MOUNT *const nodo_mount,char name[16],char *path,char letra,int numero)
{
    nodo_mount->siguiente = NULL;
    strcpy(nodo_mount->name,name);
    strcpy(nodo_mount->path_mount,path);
    nodo_mount->letra = letra;
    nodo_mount->numero = numero;
    nodo_mount->disponible = 1;

    memset(nodo_mount->id_mount,0,sizeof(nodo_mount->id_mount));
//    nodo_mount->id_mount[16] = '\0';
    strcat(nodo_mount->id_mount,"vd");
    char *let = (char*)malloc(sizeof(char)*1);
    memset(let,'\0',sizeof(let));
    //strcpy(let,&letra);
    let[0] = letra;
    //strcat(nodo_mount->id_mount,&letra);
    strcat(nodo_mount->id_mount,let);

   // char tmp1 = numero +'0';  //conversion de digito a caracter
   // strcat(nodo_mount->id_mount,&tmp1);

    char tmp1[2];
    sprintf(tmp1,"%d",numero);// + '0';
    strcat(nodo_mount->id_mount,tmp1);
    strcat(nodo_mount->id_mount,"\0");
    free(let);
    let =NULL;
   // char *tmp = nodo_mount->id_mount;
    //char tmp3 = nodo_mount->id_mount;
}

void mayus_cnv_minus_(char *cadena)
{
    int i;
    for(i=0; cadena[i]; i++)
        cadena[i] = tolower(cadena[i]);
}

char get_letra_mount(char *path,LISTA_MOUNT *const ptr_mount)
{
    char letra_mayor = 'a';
    NODO_MOUNT *tmp = ptr_mount->primero;

    while(tmp != NULL)
    {
        if(strcmp(tmp->path_mount,path) == 0)
        {
            return tmp->letra;
        }
        //else if(strcmp(tmp->letra,letra_mayor) > 0)
        else if(tmp->letra > letra_mayor);
            letra_mayor = tmp->letra;
        tmp = tmp->siguiente;
    }
    letra_mayor++;
    return letra_mayor;
}

int get_numero_mount(char *path,LISTA_MOUNT *const ptr_mount)
{
    int numero_mayor = 1;
    NODO_MOUNT *tmp = ptr_mount->primero;

    while(tmp != NULL)
    {
        if(strcmp(tmp->path_mount,path) == 0)
        {
            if(tmp->numero == numero_mayor)
                numero_mayor++;
            else if(tmp->numero > numero_mayor)
                numero_mayor = tmp->numero;
        }
        tmp = tmp->siguiente;
    }
    return numero_mayor;
}

int  desmontar_particion(char *id_mount, LISTA_MOUNT *ptr_mount)
{
    if(ptr_mount->primero !=NULL)
    {
        printf("%s\n",ptr_mount->primero->id_mount );
        printf("%s\n",id_mount);
        char *tmp5 = ptr_mount->primero->id_mount;
        NODO_MOUNT *aux;

        if(strcasecmp(ptr_mount->primero->id_mount,id_mount)==0)
        {
            if(ptr_mount->primero->siguiente == NULL)
            {
                ptr_mount->primero = NULL;
                ptr_mount->size--;
                return 1;
            }
            else
            {
                ptr_mount->primero = ptr_mount->primero->siguiente;
                ptr_mount->size--;
                return 1;
            }
        }
        NODO_MOUNT *tmp = ptr_mount->primero;
        while(tmp->siguiente != NULL)
        {
            if(strcasecmp(tmp->siguiente->id_mount,id_mount) == 0)
            {
                if(tmp->siguiente->siguiente == NULL)
                {
                    tmp->siguiente = NULL;
                    ptr_mount->size--;
                    return 1;
                }
                else
                {
                    aux = tmp->siguiente->siguiente;
                    tmp->siguiente = aux;
                    ptr_mount->size--;
                    return 1;
                }

            }
            tmp = tmp->siguiente;
        }
    }

    return 0;
        //printf("ERROR: no se encontro la particion que desea desmontar con id:  %s\n\n",id_mount);
}

char * get_path_mount(char *id_mount,LISTA_MOUNT *ptr_mount)
{
    NODO_MOUNT *tmp = ptr_mount->primero;
    while(tmp != NULL)
    {
        if(strcasecmp(tmp->id_mount,id_mount) == 0)
        {
            return tmp->path_mount;
        }
        tmp = tmp->siguiente;
    }
    return NULL;
}

NODO_MOUNT *get_nodo_mount(char *id_mount,LISTA_MOUNT *ptr_mount)
{
    NODO_MOUNT *tmp = ptr_mount->primero;
    while(tmp !=NULL)
    {
        if(strcasecmp(tmp->id_mount,id_mount)== 0)
        {
            char *pt = tmp->path_mount;
            char *nm = tmp->name;
            return  tmp;
        }
        tmp = tmp->siguiente;
    }
    return NULL;
}
