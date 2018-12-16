
#include "admin_disco.h"

void crearDisco(int size,char unit,char *path,char *disk_fit)
{
    if(size != 0 && path != NULL) //obligatorias
    {
        if(size > 0)
        {
            int index_path = 0;
            char *carpeta_actual = (char*)malloc(sizeof(char) * 100);
            memset(carpeta_actual,0,sizeof(carpeta_actual));
            char *caracter = (char*)malloc(sizeof(char) * 1); //sirve de concatenador
            memset(caracter,0,sizeof(caracter));

            if(path[0] == 34) //comilla doble
            {
                /*La diferencia de carpeta_funcional_mkdir y carpeta_actual es:
                    Que carpeta actual intentara concatenar toda la cadena incluyendo el nombre del disco .disk
                    y carpeta_funcional_mkdir intentara concatenar solo las carpetas para crearlas sino existen*/
                char *carpeta_funcional_mkdir = (char*)malloc(sizeof(char) * 100); //esta variable me sirve para crear la carpeta
                memset(carpeta_funcional_mkdir,0,sizeof(carpeta_funcional_mkdir));

                int index_path = 0;
                index_path = 1;  //quitamos las comillas dobles
                int salir = 0;

                while(salir != 1) //con este while tratamos de quitar los espacios en blanco que puedan haber al principio
                {
                    if(path[index_path] == '/')
                    {
                        salir = 1;
                        break;
                    }
                    else if(path[index_path] == NULL)
                    {
                        printf("ERROR: analisis critico en path con comillas dobles\n");
                        free(carpeta_funcional_mkdir);
                        carpeta_funcional_mkdir=NULL;
                        return;
                    }
                    index_path++;
                }

                caracter[0] = path[index_path];
                strcat(carpeta_actual,caracter);
                strcat(carpeta_funcional_mkdir,caracter);
                index_path++;

                while(path[index_path] != 34)
                {
                    if(path[index_path] == '/')
                    {
                        if(verif_carpeta(carpeta_actual) == 0) // si no existe la carpeta
                        {
                            if(crear_carpeta(carpeta_funcional_mkdir) == 1 ) //creamos la carpeta y retorna un booleano
                                printf("\nSe ha creado la carpeta con path:  %s\n\n",carpeta_actual);
                            else{
                                printf("\nNo se ha podido crear la carpeta con direccion: %s\n\n",carpeta_actual);
                                free(carpeta_funcional_mkdir);
                                carpeta_funcional_mkdir=NULL;
                                return;
                            }
                        }
                    }
                    else if(path[index_path] == 32) //espacio en blanco
                    {
                        caracter[0] = 92;  //ascii que identifica a " \ "
                        strcat(carpeta_funcional_mkdir,caracter);
                    }
                    caracter[0] = path[index_path];
                    strcat(carpeta_actual,caracter);
                    strcat(carpeta_funcional_mkdir,caracter);
                    index_path++;
                } //Fin del while
                if(path[index_path]== 34) //comillas dobles
                {
                    ejec_mkdisk(size,carpeta_actual,unit,disk_fit);
                }
                free(carpeta_funcional_mkdir);
                carpeta_funcional_mkdir=NULL;
            }
            else if(path[0] == '/')
            {
                carpeta_actual[0] = '/';
                index_path =1;
                while(path[index_path] != NULL)
                {
                    if(path[index_path] == '/')
                    {

                        if(verif_carpeta(carpeta_actual) == 0) // si no existe la carpeta
                        {
                            if(crear_carpeta(carpeta_actual) == 1 ) //creamos la carpeta y retorna un booleano
                                printf("Se ha creado la carpeta con :v path:  %s\n",carpeta_actual);
                            else{
                                printf("No se ha podido crear la carpeta con direccion: %s\n\n",carpeta_actual);
                                free(carpeta_actual);
                                carpeta_actual =NULL;
                                return;
                            }
                        }
                    }
                    else if(path[index_path] == 32) //no deben haber espacios en blanco dentro de este tipo de path
                    {
                        printf("ERROR: no pueden haber Espacios en blanco en el path, si los desea declarelos dentro de comillas dobles\n");
                        free(carpeta_actual);
                        carpeta_actual =NULL;
                        return;
                    }
                    caracter[0] = path[index_path];
                    strcat(carpeta_actual,caracter);
                    index_path++;
                }

                if(path[index_path] == NULL || path[index_path] == '\n' || path[index_path] == '\r')
                {
                    ejec_mkdisk(size,path,unit,disk_fit);
                }
            }
            free(carpeta_actual);
            carpeta_actual =NULL;
            free(caracter);
            caracter=NULL;
        }
        else{
            printf("ERROR: size debe ser mayor a Cero");
            return;
        }
    }
    else
    {
        printf("ERROR: debe de existir porlomenos el comando size y path para crear el disco\n");
        return;
    }

}

void eliminar_disco(char *path)
{
    if(limpiar_path(path) == 1 )
    {
        char comando[2];
        printf("\nPresione (s/n) si desea o no eliminar el Disco con direccion: ");
        printf("%s\n",path);
        fgets(comando,2,stdin);
        if(comando[0] == 's' || comando[0] == 'S')
        {
            if(remove(path) == 0)
            {
                printf("el path: %s",path);
                printf(" SI pudo eliminar el disco \n");
            }
            else
            {
                printf("el path: %s",path);
                printf(" NO  pudo eliminar el disco \n");
            }
        }
        else
            printf("\nAbortado!!\n");

    }
    else
        printf("Corregir los errores que hay en el path: %s\n",path);
}

void crear_particion(int size,char unit,char type,char fit[],char *path, char *name)
{
    FILE *archivo = fopen(path,"rb"); //archivo en modo lectura
    /* obteniendo mbr */
    fseek(archivo,0,SEEK_SET);
    MBR mbr_tmp[1];
    fread(mbr_tmp,sizeof(MBR),1,archivo);
    fclose(archivo);
    if(type == 'p'|| type == 'e' )
    {
        crear_particion_(&mbr_tmp[0],size, unit, type, fit,path,name);
    }
    else if(type == 'l')
    {
        //printf("Particion logica aun no definida\n");
        crear_particion_logica(&mbr_tmp[0],size,unit,fit,path,name);
    }
}

void crear_particion_logica(MBR *mbr,int size,char unit,char fit[],char *path,char *name)
{
    PTR particiones[4];
    particiones[0] = mbr->mbr_partition_1;
    particiones[1] = mbr->mbr_partition_2;
    particiones[2] = mbr->mbr_partition_3;
    particiones[3] = mbr->mbr_partition_4;

    int inicio_extendida =0;
    int size_extendida =0;
    char fit_ext[3];
    int size_bytes = numero_bytes(unit,size);
    int si_hay_ext = 0;
    for(int i = 0; i < 4; i++)
    {
        if(particiones[i].part_status == '1')
        {
            if(particiones[i].part_type == 'e')
            {
                inicio_extendida = particiones[i].part_start;
                size_extendida = particiones[i].part_size;
                strcpy(fit_ext,particiones[i].part_fit);
                si_hay_ext = 1;
                break;
            }
        }
    }

    if(si_hay_ext == 1)
    {
        FILE *archivo = fopen(path,"rb+");
        if(archivo != NULL)
        {
            fseek(archivo,inicio_extendida,SEEK_SET);
            EBR ebr_raiz[1];
            fread(ebr_raiz,sizeof(EBR),1,archivo);
            if(ebr_raiz[0].part_status == '1') //si esta activa
            {
                if(ebr_raiz[0].part_next != -1)
                {
                    EBR ebr_tmp[1];
                    fseek(archivo,ebr_raiz[0].part_next,SEEK_SET);
                    fread(ebr_tmp,sizeof(EBR),1,archivo);
                    while(ebr_tmp[0].part_next !=-1)
                    {
                        fseek(archivo,ebr_tmp[0].part_next,SEEK_SET);
                        fread(ebr_tmp,sizeof(EBR),1,archivo);
                    }

                    int ini_log = sizeof(EBR);
                    int esp_total_log = ebr_tmp[0].part_start + ini_log + size_bytes;
                    if((inicio_extendida + size_extendida - esp_total_log) > 0 ) //es que si cabe
                    {
                        EBR ebr_nuevo;
                        ebr_nuevo.part_status = '1';
                        ebr_nuevo.part_start = esp_total_log + sizeof(EBR);
                        ebr_nuevo.part_size = size_bytes;
                        ebr_nuevo.part_next = -1;
                        strcpy(ebr_nuevo.part_name,name);
                        strcpy(ebr_nuevo.part_fit,fit);
                        fseek(archivo,esp_total_log,SEEK_SET);
                        fwrite(&ebr_nuevo,sizeof(EBR),1,archivo);

                        ebr_tmp[0].part_next = esp_total_log;
                        fseek(archivo,ebr_tmp[0].part_start - sizeof(EBR),SEEK_SET);
                        fwrite(&ebr_tmp[0],sizeof(EBR),1,archivo);

                    }
                    else
                    {
                        printf("ERROR: No hay espacio para una logica dentro de la particion extendida\n\n");
                    }

                }
                else
                {
                    int ini_log = sizeof(EBR);
                    int esp_total_log = ebr_raiz[0].part_start + ini_log + size_bytes;
                    if((inicio_extendida + size_extendida - esp_total_log) > 0 ) //es que si cabe
                    {
                        EBR ebr_nuevo;
                        ebr_nuevo.part_status = '1';
                        ebr_nuevo.part_start = esp_total_log + sizeof(EBR);
                        ebr_nuevo.part_size = size_bytes;
                        ebr_nuevo.part_next = -1;
                        strcpy(ebr_nuevo.part_name,name);
                        strcpy(ebr_nuevo.part_fit,fit);
                        fseek(archivo,esp_total_log,SEEK_SET);
                        fwrite(&ebr_nuevo,sizeof(EBR),1,archivo);

                        ebr_raiz[0].part_next = esp_total_log;
                        fseek(archivo,ebr_raiz[0].part_start - sizeof(EBR),SEEK_SET);
                        fwrite(&ebr_raiz[0],sizeof(EBR),1,archivo);
                    }
                    else
                    {
                        printf("ERROR: No hay espacio para una logica dentro de la particion extendida\n\n");
                    }
                }
            }
            else
            {
                int ini_log = sizeof(EBR);
                int esp_total_log = inicio_extendida + ini_log + size_bytes;
                if((inicio_extendida + size_extendida - esp_total_log) > 0 ) //es que si cabe
                {
                    ebr_raiz[0].part_status = '1';
                    ebr_raiz[0].part_start = inicio_extendida + sizeof(EBR);
                    ebr_raiz[0].part_size = size_bytes;
                    ebr_raiz[0].part_next = -1;
                    strcpy(ebr_raiz[0].part_name,name);
                    strcpy(ebr_raiz[0].part_fit,fit);
                    fseek(archivo,inicio_extendida,SEEK_SET);
                    fwrite(&ebr_raiz,sizeof(EBR),1,archivo);
                }
                else
                {
                    printf("ERROR: No hay espacio para una logica dentro de la particion extendida\n\n");
                }
            }
            fclose(archivo);
        }
    }
}

void buscar_espacio_logica(EBR *ebr_actual,char *path,int inicio_disponible,int size_nuevo,int size_ext,LISTA_AJUSTE *const ptr_ajuste)
{
    if(ebr_actual->part_status != '0')
    {
        if(inicio_disponible < ebr_actual->part_start)
        {
            int esp_total = inicio_disponible + size_nuevo;
            if(esp_total <= ebr_actual->part_start)
                add_lst_ajuste(ptr_ajuste,inicio_disponible,esp_total);
            inicio_disponible = ebr_actual->part_start + ebr_actual->part_size;
            if(ebr_actual->part_next != -1)
            {
                EBR ebr_tmp[1];
                FILE *archivo = fopen(path,"rb");
                fseek(archivo,ebr_actual->part_next,SEEK_SET);
                fread(ebr_tmp,sizeof(EBR),1,archivo);
                fclose(archivo);
                buscar_espacio_logica(&ebr_tmp,path,inicio_disponible,size_nuevo,size_ext,ptr_ajuste);
            }
        }
        else
        {
            int espacio_total = inicio_disponible + size_nuevo;
            if(espacio_total <= size_ext)
                add_lst_ajuste(ptr_ajuste,inicio_disponible,size_ext);
        }
    }
}


void crear_particion_(MBR *mbr,int size,char unit,char type,char fit[],char *path,char *name)
{
    PTR *particion;
    int hay_particion = 0; //bandera

    int ini_part = sizeof(*mbr);    //acordarse que se utiliza el n-1 en los tamanios
    int size_bytes = numero_bytes(unit,size);

    //si retorna 1 == la direccion de la particion debe ir contenida
    //si retorna 0 = no hay particiones disponibles
    int flag_ptr_dispo = buscar_particion_disponible(*&mbr,name,&particion,type);

    //mbr->referencia ; particion -> referencia ;hay_particion->referencia ; ini_part->lo que ocupa mbr; name-> por si se repite
    //buscar_particion_dispo(*&mbr,&particion,size_bytes,&hay_particion,&ini_part,name,type);

    if(flag_ptr_dispo == 1) //ahora que si hay ptr disponible debemos buscar el tamanio
    {
        int flag_espacio = buscar_espacio_adecuado(*&mbr,size_bytes,&ini_part);
        if(flag_espacio == 1)
        {
            particion->part_status = '1';
            particion->part_type = type;
            strcpy(particion->part_fit,fit);
            particion->part_start = ini_part;
            int size_particion = numero_bytes(unit,size);
            particion->part_size = size_particion;
            strcpy(particion->part_name,name);

            FILE *archivo = fopen(path,"r+b");
            fseek(archivo,0,SEEK_SET);
            fwrite(mbr,sizeof(MBR),1,archivo);
            fclose(archivo);

            if(type == 'e')
            {
                FILE *archivo2 = fopen(path,"rb+");  //escritura (machaca datos) y lectura
                fseek(archivo2,0,SEEK_SET);
                MBR mbr_ex[1];
                fread(mbr_ex,sizeof(MBR),1,archivo2);
                PTR ptr_extend[4];
                ptr_extend[0] = mbr_ex[0].mbr_partition_1;
                ptr_extend[1] = mbr_ex[0].mbr_partition_2;
                ptr_extend[2] = mbr_ex[0].mbr_partition_3;
                ptr_extend[3] = mbr_ex[0].mbr_partition_4;

                PTR extend;
                for (int i = 0; i < 4; ++i) {
                    if(ptr_extend[i].part_status =='1')
                    {
                        if(ptr_extend[i].part_type == 'e')
                        {
                            extend = ptr_extend[i];
                            break;
                        }
                    }
                }

                if(extend.part_status !='0')
                {
                    //EBR *mi_ebr = (EBR*)malloc(sizeof(EBR));
                    EBR mi_ebr;
                    /*comprobando*/
                    mi_ebr.part_status = '0';
                    int comp = sizeof(EBR) + extend.part_start;
                    int inicio_ebr = extend.part_start;
                    if(comp <= (extend.part_start + extend.part_size))
                    {
                        fseek(archivo2,inicio_ebr,SEEK_SET);
                        fwrite(&mi_ebr,sizeof(EBR),1,archivo2);

                        printf("se ha creado una particion Extendida\n\n");
                    }
                    else
                        printf("ERROR:No hay suficiente espacio para escribir un EBR en la particion extendida\n\n");
                }
                else
                    printf("ERROR: hubo problemas con encontrara la particon extendida pra escribir el EBR inicial.\n\n");

                fclose(archivo2);
            }
            else
            {
                printf("Se ha Creado una particion Primaria\n\n");
                FILE *archivo = fopen(path,"r+b");
                if(crear_ext3(archivo,size_particion,ini_part,name) == 1)
                {
                    //printf("Se ha creado un sistema de archivos EXT3 en la particion %s\n\n",name);
                    crear_archivo_users(archivo,ini_part,name);
                }
                else
                    printf("ERROR: No se ha podido crear un EXT3 en la particion %s\n\n",name);
                fclose(archivo);
            }

        }
        else
            printf("ERROR: no se ha podido crear la particion\n\n");
    }
}

int buscar_particion_disponible(MBR *mbr,char *name,PTR **particion,char type)
{
    PTR particiones[4];
    particiones[0] = mbr->mbr_partition_1;
    particiones[1] = mbr->mbr_partition_2;
    particiones[2] = mbr->mbr_partition_3;
    particiones[3] = mbr->mbr_partition_4;

    //::::verificando que no hayan nombres repetidos ni mas de una extendida
    for (int i = 0; i < 4; ++i)
    {
        if(particiones[i].part_status == '1')//si esta activa
        {
            if(strcasecmp(particiones[i].part_name,name) == 0)
            {
                printf("ERROR: No se puede Crear la particion porque el nombre ya existe\n\n");
                return 0;
            }
            if(particiones[i].part_type == 'e' && type == 'e')
            {
                printf("ERROR: No se puede Crear mas de una  particion extendida \n\n");
                return 0;
            }
            if(particiones[i].part_type == 'p')
            {
                int conteo_primarias = 0;
                for(int j = 0; j < 4; j++)
                {
                    if(particiones[j].part_type == 'p')
                        conteo_primarias++;
                }
                if(conteo_primarias == 3)
                {
                    if(type == 'p')
                    {
                        printf("ERROR: No se puede Crear mas de 3 particiones primarias \n\n");
                        return 0;
                    }

                }
            }

        }
    }
    // si todas estan vacias
    if(particiones[0].part_status == '0' && particiones[1].part_status == '0' && particiones[2].part_status =='0' && particiones[3].part_status == '0')
    {
       (*particion) =  &mbr->mbr_partition_1; //paso la direccion
       return 1;
    }
    else if(particiones[0].part_status == '1' && particiones[1].part_status == '1' && particiones[2].part_status =='1' && particiones[3].part_status == '1')
    {
        //si estan todas ocupadas
        printf("ERROR: no hay particiones disponibles dentro del disco para mas particiones\n\n");
        return 0;
    }
    else
    {
        int flag_particion_encontrada = 0;
        ptr_libre(*&mbr,*&particion,&flag_particion_encontrada);

        if(flag_particion_encontrada == 1)
            return 1;
        return 0;
    }
}


int buscar_espacio_adecuado(MBR *mbr,int size_buscado,int *ini_ptr)
{
    //Ojo particiones solo serviran para encontrar la posicion adecuada y no
    //alterar las particiones dentro del mbr real
    PTR particiones[4];
    particiones[0] = mbr->mbr_partition_1;
    particiones[1] = mbr->mbr_partition_2;
    particiones[2] = mbr->mbr_partition_3;
    particiones[3] = mbr->mbr_partition_4;

    ordenar_particiones(&particiones);

    if(strcasecmp(mbr->disk_fit,"ff") == 0)
    {
        int itera_ff =0;
        int flag_ff= buscar_espacio_ff_disco(particiones,itera_ff,*&ini_ptr,size_buscado,*&mbr);
        if(flag_ff == 1)
            return 1;
        else
        {
            printf("ERROR: no se pudo encontrar espacio en el primer ajuste(FF)\n\n");
            return 0;
        }
    }
    else if(strcasecmp(mbr->disk_fit,"bf") == 0)
    {
        LISTA_AJUSTE *ptr_ajuste = (LISTA_AJUSTE*)malloc(sizeof(LISTA_AJUSTE));
        inicializar_lst_ajuste(ptr_ajuste);
        int itera =0;
        buscar_espacio_disco(particiones,itera,*ini_ptr,size_buscado,*&mbr,ptr_ajuste);
        if(ptr_ajuste->size != 0)
        {
            if(ptr_ajuste->size == 1)
            {
                *ini_ptr = ptr_ajuste->primero->tamanio_inicio;
                return 1;
            }
            else
            {
                ordenar_menor_mayor(ptr_ajuste);
                *ini_ptr = ptr_ajuste->primero->tamanio_inicio;
                return 1;
            }
        }
        else
        {
            printf("ERROR: No se pudo encontrar Espacio en le mejor ajuste para la particion\n\n");
            return 0;
        }
    }
    else if(strcasecmp(mbr->disk_fit,"wf") == 0)
    {
        LISTA_AJUSTE *ptr_ajuste = (LISTA_AJUSTE*)malloc(sizeof(LISTA_AJUSTE));
        inicializar_lst_ajuste(ptr_ajuste);
        int itera =0;
        buscar_espacio_disco(particiones,itera,*ini_ptr,size_buscado,*&mbr,ptr_ajuste);
        if(ptr_ajuste->size != 0)
        {
            if(ptr_ajuste->size == 1)
            {
                *ini_ptr = ptr_ajuste->primero->tamanio_inicio;
                return 1;
            }
            else
            {
                ordenar_mayor_menor(ptr_ajuste);
                *ini_ptr = ptr_ajuste->primero->tamanio_inicio;
                return 1;
            }
        }
        else
        {
            printf("ERROR: No se pudo encontrar Espacio en le mejor ajuste para la particion\n\n");
            return 0;
        }
    }

}

void buscar_espacio_disco(PTR particion_mbr[],int itera,int inicio_disponible,int size_nuevo,MBR *mbr,LISTA_AJUSTE *const ptr_ajuste)
{
    if(itera < 4)
    {
        if(particion_mbr[itera].part_status != '0' && itera != 3)
        {
            if(inicio_disponible < particion_mbr[itera].part_start) //si el inicio de todo esta disponible
            {
                int esp_total = inicio_disponible + size_nuevo;
                if(esp_total <= particion_mbr[itera].part_start)
                    add_lst_ajuste(ptr_ajuste,inicio_disponible,esp_total);

                inicio_disponible = particion_mbr[itera].part_start + particion_mbr[itera].part_size; //verificar a fondo esta linea
                itera++;
                buscar_espacio_disco(particion_mbr,itera,inicio_disponible,size_nuevo,*&mbr,ptr_ajuste);

            }
            else if(itera + 1 < 4) //me adelanto uno
            {
                if(particion_mbr[itera+1].part_status != '0')
                {
                    inicio_disponible = particion_mbr[itera].part_start + particion_mbr[itera].part_size;
                    int esp_total = inicio_disponible + size_nuevo;

                    if(esp_total <= particion_mbr[itera+1].part_start) //entra dentro del espacio
                        add_lst_ajuste(ptr_ajuste,inicio_disponible,esp_total);
                    itera++;
                    buscar_espacio_disco(particion_mbr,itera,inicio_disponible,size_nuevo,*&mbr,ptr_ajuste);
                }
                else
                {
                    inicio_disponible = particion_mbr[itera].part_start + particion_mbr[itera].part_size;
                    itera++;
                    buscar_espacio_disco(particion_mbr,itera,inicio_disponible,size_nuevo,*&mbr,ptr_ajuste);
                }
            }
        }
        else
        {
            int esp_total = inicio_disponible + size_nuevo;
             //significa que estoy en una particion nula o part_status = '0'
            //como los status iran al final de la lista de particiones  si un part_status = 0
            //signifca que estos en las posiciones finales
            if(esp_total <= mbr->mbr_tamanio_disk)
            {
                add_lst_ajuste(ptr_ajuste,inicio_disponible,mbr->mbr_tamanio_disk);
            }

        }
    }
}
int buscar_espacio_ff_disco(PTR particion_mbr[],int itera,int *inicio_disponible,int size_nuevo,MBR *mbr)
{
    if(itera < 4)
    {

        if(particion_mbr[itera].part_status != '0' && itera != 3)
        {
            if(*inicio_disponible < particion_mbr[itera].part_start) //si el inicio de todo esta disponible
            {
                int esp_total = *inicio_disponible + size_nuevo;
                if(esp_total <= particion_mbr[itera].part_start)
                    return 1;
            }
            if(itera + 1 < 4) //me adelanto uno
            {
                if(particion_mbr[itera+1].part_status != '0')
                {
                    *inicio_disponible = particion_mbr[itera].part_start + particion_mbr[itera].part_size;
                    int esp_total = *inicio_disponible + size_nuevo;
                    if(esp_total <= particion_mbr[itera+1].part_start) //entra dentro del espacio
                        return 1;
                    else
                    {
                        itera++;
                        buscar_espacio_ff_disco(particion_mbr,itera,*&inicio_disponible,size_nuevo,*&mbr);
                    }
                }
                else
                {
                    *inicio_disponible = particion_mbr[itera].part_start + particion_mbr[itera].part_size;
                    itera++;
                    buscar_espacio_ff_disco(particion_mbr,itera,*&inicio_disponible,size_nuevo,*&mbr);
                }
            }
        }
        else
        {
            int esp_total = *inicio_disponible + size_nuevo;
             //significa que estoy en una particion nula o part_status = '0'
            //como los status iran al final de la lista de particiones  si un part_status = 0
            //signifca que estos en las posiciones finales
            if(esp_total <= mbr->mbr_tamanio_disk)
                return 1;
            else{
            printf("ERROR: el espacio que desea almacenar sobrepasa el espacio libre del disco \n\n");
            return 0;
            }
        }
    }
    else
    {
        printf("ERROR: no hay espacio para asignar la nueva particion \n\n");
        return 0;
    }
}

void buscar_particion_dispo(MBR *mbr,PTR **particion,int size_buscado,int *flag_ptr,int *ini_ptr,char *name,char type)
{
    int mbr_tamanio_disk = mbr->mbr_tamanio_disk;
    int mbr_disk_segnature = mbr->mbr_disk_segnature;

    PTR particiones[4];
    particiones[0] = mbr->mbr_partition_1;
    particiones[1] = mbr->mbr_partition_2;
    particiones[2] = mbr->mbr_partition_3;
    particiones[3] = mbr->mbr_partition_4;

    //::::verificacion de errores
    for (int i = 0; i < 4; ++i) {
        if(particiones[i].part_status == '1')//si esta activa
        {
            if(strcasecmp(particiones[i].part_name,name) == 0)
            {
                printf("ERROR: No se puede Crear la particion porque el nombre ya existe\n\n");
                *flag_ptr =0;
                return;
            }
            if(particiones[i].part_type == 'e' && type == 'e')
            {
                printf("ERROR: No se puede Crear mas de una  particion extendida \n\n");
                *flag_ptr =0;
                return;
            }

        }

    }
    // si todas estan vacias
    if(particiones[0].part_status == '0' && particiones[1].part_status == '0' && particiones[2].part_status =='0' && particiones[3].part_status == '0')
    {
       (*particion) =  &mbr->mbr_partition_1; //paso la direccion
       *flag_ptr =1;
    }
    else if(particiones[0].part_status == '1' && particiones[1].part_status == '1' && particiones[2].part_status =='1' && particiones[3].part_status == '1')
    {
        printf("ERROR: no hay particiones disponibles dentro del disco para mas particiones\n\n");
        *flag_ptr = 0;
        return;
    }
    else
    {
        ordenar_particiones(&particiones);
        int itera = 0;

        buscar_espacio(*&particion,particiones,itera,*&ini_ptr,size_buscado,*&mbr,*&flag_ptr);
    }

}

void ordenar_particiones(PTR *particion)
{
    PTR tmp[4];
    int i =0;
    for(int j = 0; j < 4; j++)
    {
        if(particion[j].part_status != '0')
        {
            tmp[i] = particion[j];
            i++;
        }
    }
    for(int k = i; k < 4; k++)
    {
        tmp[k].part_status = '0';
    }
    for(int pos = 0; pos < 4; pos++)
    {
        particion[pos] = tmp[pos];
    }

    //ordenacion por burbuja
    PTR aux;
    for(int i =0; i < 3 ;i++)
    {
        if(particion[i].part_status != '0')
        {
            for(int j = i+1; j < 4;j++ )
            {
                if(particion[j].part_status != '0')
                {
                    if(particion[i].part_start > particion[j].part_start )
                    {
                        aux = particion[i];
                        particion[i] = particion[j];
                        particion[j] = aux;
                    }
                }
            }
        }

    }
}

void buscar_espacio(PTR **particion,PTR particion_mbr[],int itera,int *inicio_libre,int size_nuevo,MBR *mbr,int *flag)
{
    if(itera < 4)
    {
        if(particion_mbr[itera].part_status != '0' && itera != 3)
        {
            if(*inicio_libre < particion_mbr[itera].part_start)
            {
                int espacio_total = *inicio_libre + size_nuevo;
                if(espacio_total > *inicio_libre && espacio_total <= particion_mbr[itera].part_start)
                {
                    ptr_libre(*&mbr,*&particion,*&flag);
                    return;
                }
            }
            *inicio_libre = particion_mbr[itera].part_start + particion_mbr[itera].part_size;
            int i_l = *inicio_libre;
            itera++;
            ptr_libre(*&mbr,*&particion,*&flag);
            buscar_espacio(*&particion,particion_mbr,itera,*&inicio_libre,size_nuevo,*&mbr,*&flag);
        }
        else
        {
            int esp_total = *inicio_libre + size_nuevo;

            if(itera == 3) //es el ultimo del vector
            {
                if(esp_total > mbr->mbr_tamanio_disk)
                {
                    *flag = 0;
                    printf("ERROR: el espacio que desea almacenar sobrepasa el espacio libre del diso duro\n\n");
                }
                else
                {
                    if(particion_mbr[itera].part_status != '0')
                    {
                        *inicio_libre = particion_mbr[itera].part_start + particion_mbr[itera].part_size;
                    }
                    ptr_libre(*&mbr,*&particion,*&flag);
                }
            }
            else //significa que estoy en una particion nula o part_status = '0'
            {
                if(itera + 1 <= 3)
                {
                    if(esp_total <= particion_mbr[itera + 1].part_start)
                    {
                        ptr_libre(*&mbr,*&particion,*&flag);
                    }
                    else
                    {
                        itera++;
                        buscar_espacio(*&particion,particion_mbr,itera,*&inicio_libre,size_nuevo,*&mbr,*&flag);
                    }
                }
                else
                    printf("ERROR: Hay problemas para asignar la particon en el disco!!\n\n" );
            }
        }
    }
    else
        *flag = 0;
}

void ptr_libre(MBR *mbr,PTR **particion,int *flag)
{
//int mbr_tamanio_disk = mbr->mbr_tamanio_disk;
   // char mbr_fecha_creacion[16];
  //  int mbr_disk_segnature = mbr->mbr_disk_segnature;
    if(mbr->mbr_partition_1.part_status == '0')
    {
        (*particion) = &mbr->mbr_partition_1;
        *flag = 1;
    }
    else if(mbr->mbr_partition_2.part_status == '0')
    {
        (*particion) = &mbr->mbr_partition_2;
        *flag = 1;
    }
    else if(mbr->mbr_partition_3.part_status == '0')
    {
        (*particion) = &mbr->mbr_partition_3;
        *flag = 1;
    }
    else if(mbr->mbr_partition_4.part_status == '0')
    {
        (*particion) = &mbr->mbr_partition_4;
        *flag = 1;
    }
    else
        *flag = 0;
}

void delete_particion(char *tipoDel,char *name,char *path_archivo)
{
    FILE *archivo = fopen(path_archivo,"rb");
    if(archivo)
    {
        fseek(archivo,0,SEEK_SET);
        MBR mbr[1];
        int eliminado =0;
        fread(mbr,sizeof(MBR),1,archivo);

        PTR *partition[4];
        partition[0] = &mbr[0].mbr_partition_1;
        partition[1] = &mbr[0].mbr_partition_2;
        partition[2] = &mbr[0].mbr_partition_3;
        partition[3] = &mbr[0].mbr_partition_4;
        fclose(archivo);

        for (int i = 0; i < 4; ++i) {
            if(partition[i]->part_status == '1')
            {
                if(strcmp(partition[i]->part_name,name) ==0)
                {
                    if(strcasecmp(tipoDel,"fast") ==0) //limpiar tabla de particiones
                    {
                        partition[i]->part_status ='0';
                        partition[i]->part_type = ' ';
                        partition[i]->part_size =0;
                        partition[i]->part_start =0;
                        memset(partition[i]->part_name,0,sizeof(partition[i]->part_name));
                        memset(partition[i]->part_fit,0,sizeof(partition[i]->part_fit));
                        eliminado =1;
                        break;
                    }
                    else if(strcasecmp(tipoDel,"full") ==0) //limpia tabla de particiones y su contenido en laparticion
                    {
                        FILE *archivo2 = fopen(path_archivo,"rb+");
                        //int tmp1 = partition[i]->part_start;
                        //int tmp2 = partition[i]->part_size;
                        for(int j = partition[i]->part_start; j < (partition[i]->part_start + partition[i]->part_size); j++)
                        {
                            fseek(archivo2,j,SEEK_SET);
                            fwrite("\0",1,1,archivo2);
                        }
                        fclose(archivo2);

                        partition[i]->part_status ='0';
                        partition[i]->part_type = ' ';
                        partition[i]->part_size =0;
                        partition[i]->part_start =0;
                        memset(partition[i]->part_name,0,sizeof(partition[i]->part_name));
                        memset(partition[i]->part_fit,0,sizeof(partition[i]->part_fit));
                        eliminado =1;

                        break;
                    }
                    else //por defecto una fast
                    {
                        partition[i]->part_status ='0';
                        partition[i]->part_type = ' ';
                        partition[i]->part_size =0;
                        partition[i]->part_start =0;
                        memset(partition[i]->part_name,0,sizeof(partition[i]->part_name));
                        memset(partition[i]->part_fit,0,sizeof(partition[i]->part_fit));
                        eliminado =1;
                        break;
                    }

                }
            }
        }


        if(eliminado == 1)
        {
            /*actualizando el mbr*/
            /*Escritura del MBR en el disco*/
            FILE *archivo = fopen(path_archivo,"r+b");
            fseek(archivo,0,SEEK_SET); //nos hubicamos al principio  del archivo
            fwrite(&mbr,sizeof(MBR),1,archivo);
            fclose(archivo);
            printf("la particion: %s",name);
            printf(" ha sido eliminada\n\n");

        }
        else
        {
            printf("ERROR:No se encontro la particion en el disco con el nombre : %s",name);
        }

    }
    else
        printf("ERROR: problemas con la lectura del archivo para eliminar\n\n");
}

void montar_disco(char *path,char *name,LISTA_MOUNT *const ptr_mount)
{
    if(limpiar_path(path) == 1 )
    {
        FILE *archivo = fopen(path,"rb");
        if(archivo != NULL)
        {
            fseek(archivo,0,SEEK_SET);
            MBR mbr[1];
            fread(mbr,sizeof(MBR),1,archivo);
            int flag_montar = 0;
            if(strcmp(mbr[0].mbr_partition_1.part_name,name) ==0)
                flag_montar =1;
            else if(strcmp(mbr[0].mbr_partition_2.part_name,name) ==0)
                flag_montar = 1;
            else if(strcmp(mbr[0].mbr_partition_3.part_name,name) ==0)
                flag_montar = 1;
            else if(strcmp(mbr[0].mbr_partition_4.part_name,name) ==0)
                flag_montar = 1;

            if(flag_montar == 1)
            {
                if(ptr_mount->primero == NULL) //como es primerizo se inicia por la letra a y numero 1;
                {
                    montar_particion(ptr_mount,'a',name,1,path);
                }
                else
                {
                    char letra = get_letra_mount(path,ptr_mount);
                    int numero = get_numero_mount(path,ptr_mount);
                    montar_particion(ptr_mount,letra,name,numero,path);
                }
            }else
                printf("ERROR: el nombre de la particion no existe en el disco para montarla");
        }
        else
            printf("ERROR: no se puede acceder al directorio: %s\n\n",path);
    }
    else
        printf("Corregir los errores que hay en el path: %s\n",path);
}


void ejec_mkdisk(int size, char *path, char unit,char *disk_fit)
{
        FILE *archivo;
        archivo = fopen(path ,"wb");
        if(archivo)
        {
            char *nulo[1024];

            if(unit == 'k') //el archivo en kilobytes
            {
                int enKilo = size * 1024;
                fseek(archivo,0,SEEK_SET);
                fwrite("\0",1,1,archivo);
                fseek(archivo,enKilo,SEEK_SET);
                fwrite("\0",1,1,archivo);
                fclose(archivo);

                /* creacion de su mbr */
                crear_mbr(path,disk_fit);
            }
            else if(unit == 'm')
            {
                int enMegabyte = size * 1024*1024;
                fseek(archivo,0,SEEK_SET);
                fwrite("\0",1,1,archivo);
                fseek(archivo,enMegabyte,SEEK_SET);
                fwrite("\0",1,1,archivo);
                fclose(archivo);

                /* creacion de su mbr */
                /* creacion de su mbr */
                crear_mbr(path,disk_fit);
            }
            else
                printf("los parametros de unit deben ser en k (kilobytes) o m (megabytes) \n");
        }
        else
            printf("no se pudo crear el disco porque la direccion del path no se encontro \n");
}

void crear_mbr(char *path,char *d_fit)
{
    //MBR *mi_mbr =(MBR*)malloc(sizeof(MBR));
    MBR mi_mbr;
    /* Calculando el tamanio del disco por medio de path*/
    long noByte = 0;
    int numAleatorio =0;
    FILE *arch = fopen(path,"r");//fopen(path,"rb");
    fseek(arch,0L,SEEK_END); //nos hubicamos al final del archivo
    noByte = ftell(arch);
    fclose(arch);

    mi_mbr.mbr_tamanio_disk = noByte; //tamanio del disco

    /*calculando el numero aleatorio */
     srand(time(NULL));
     numAleatorio = rand() % 11;

     mi_mbr.mbr_disk_segnature = numAleatorio;

     /*calculando el time de creacion de mbr*/
     time_t tiempo = time(0);
     struct tm *tlocal = localtime(&tiempo);
     char fecha[16];
     //strftime(fecha,16,"%d/%m/%y %H:%M:%S",tlocal);
        strftime(fecha,16,"%d/%m/%y",tlocal);

    /*configurando disk_fit*/
    strcpy(mi_mbr.disk_fit,d_fit);

    /* configurando la inicializacion de las particiones*/
     strcpy(mi_mbr.mbr_fecha_creacion ,fecha);
     mi_mbr.mbr_partition_1.part_status = '0';
     mi_mbr.mbr_partition_2.part_status = '0';
     mi_mbr.mbr_partition_3.part_status = '0';
     mi_mbr.mbr_partition_4.part_status = '0';

    /*Escritura del MBR en el disco*/
    FILE *archivo = fopen(path,"r+b");
    fseek(archivo,0,SEEK_SET); //nos hubicamos al principio  del archivo
    fwrite(&mi_mbr,sizeof(MBR),1,archivo);
    fclose(archivo);
    printf(" creo el Disco con el MBR Satisfacoriamente!!\n");

}

/* 0 -> no existe ; 1-> si existe*/
int verif_carpeta(char *path)
{
//    DIR *carpeta;
//    /* se verifica si existe la carpeta con NULL para posteriormente crearla sino existe*/
//    if(carpeta = opendir(path) == NULL)
//        return 0;
//    else
//        return 1;

    struct stat s;
    int err = stat(path,&s);

    if(-1 == err) {
        if(ENOENT == errno) {
            /* does not exist */
            return 0;
        } else {
            perror("stat");
            exit(1);
        }
    } else {
        if(S_ISDIR(s.st_mode)) {
            /* it's a dir */
            return 1;
        } else {
            /* exists but is no dir */
            return 0;
        }
    }
}

/* 0 -> Error ; 1-> exito*/
int crear_carpeta(char *path)
{
    /* creacion de la carpeta */
    char *nueva_carpeta;
    char permidos_sudo[100] = "echo r@ndolph2012|sudo -S  mkdir ";
    nueva_carpeta = permidos_sudo;
    strcat(nueva_carpeta, path);
    int flag = system(nueva_carpeta);

    /* adicionando permisos a la carpeta */
    if(flag == 0)
    {
        char *permiso;
        char permiso_usuarios[100] = "echo r@ndolph2012|sudo -S chmod 777 ";
        permiso = permiso_usuarios;
        strcat(permiso,path);
        int flag2 = system(permiso);
        if(flag2 != 0)
        {
            printf("\nERROR: No se puedieron dar permisos a la carpeta %S\n",path);
            return 0;
        }
        return 1;
    }
    else
        printf("ERROR: en la creacion de una nueva Carpeta ");
    return 0;
}

int verificar_extension(char *path, int index_path)
{
    int j =0 ;
    char *tmp = (char*)malloc(sizeof(char) * 4);
    while(j < 3)
    {
        if(path[index_path] ==NULL)
        {
            printf("\nERROR: problemas con la extension del archivo: %s\n",path);
            return 0;
        }
        tmp[j] = path[index_path];
        index_path++;
        j++;
    }
    if(strcasecmp(tmp,"dsk") != 0)
    {
        printf("\nERROR: problemas con la extension del archivo: %s\n",path);
        return 0;
    }
    return 1;
}

int limpiar_path(char *path)
{
    int index_path = 0;
    char *comando_archivo = (char*)malloc(sizeof(char) * 150);
    memset(comando_archivo,0,sizeof(comando_archivo));

    char *caracter = (char*)malloc(sizeof(char) * 1);
    memset(caracter,0,sizeof(caracter));

    if(path[index_path] == '/')
    {
        while(path[index_path]!=NULL)
        {
            caracter[0] = path[index_path];
            strcat(comando_archivo,caracter);

            if(path[index_path] == 32) //espacio en blanco
            {
                printf("ERROR: No debe existir espacios en blanco en un path sin comillas dobles\n");
                printf("error en : %s\n\n",path);
                free(comando_archivo);
                comando_archivo =NULL;
                free(caracter);
                caracter=NULL;
                return 0;
            }
            index_path++;
        }
    }
    else if(path[index_path] == 34) //si posee comillas dobles
    {
        index_path++; //aumento 1 para no agarrar la comilla doble
        if(path[index_path] == '/')
        {
            while((path[index_path] != 34))
            {
                caracter[0] = path[index_path];
                strcat(comando_archivo,caracter);
                index_path++;
            }
        }
        else{
            free(comando_archivo);
            comando_archivo =NULL;
            free(caracter);
            caracter=NULL;
            return 0;
        }


    }

    memset(path,0,sizeof(path));
    strcpy(path,comando_archivo);
    free(comando_archivo);
    comando_archivo =NULL;
    free(caracter);
    caracter=NULL;

    return 1;
}

int numero_bytes(char unit,int size)
{
    if(unit == 'k') //el archivo en kilobytes
    {
        int enKilo = size * 1024;
        return enKilo;
    }
    else if(unit == 'm')
    {
        int enMegabyte = size * 1024*1024;
        return enMegabyte;
    }
    else if(unit == 'b') // bytes
    {
        return size;
    }
    return -1;
}
