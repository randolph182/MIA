#include "sistema_archivos.h"

void generar_ext3(FILE *archivo,char *type_mkfs,char *name_particion)
{

    //sacado el mbr
    fseek(archivo,0,SEEK_SET);
    MBR mbr_tmp[1];
    fread(mbr_tmp,sizeof(MBR),1,archivo);
    //buscando la particion
    PTR particion;
    //buscando el iterador que contiene la particion
    if(strcmp(mbr_tmp[0].mbr_partition_1.part_name,name_particion) ==0 )
        particion = mbr_tmp[0].mbr_partition_1;
    else if(strcmp(mbr_tmp[0].mbr_partition_2.part_name,name_particion) ==0 )
        particion = mbr_tmp[0].mbr_partition_2;
    else if(strcmp(mbr_tmp[0].mbr_partition_3.part_name,name_particion) ==0 )
        particion = mbr_tmp[0].mbr_partition_3;
    else if(strcmp(mbr_tmp[0].mbr_partition_3.part_name,name_particion) ==0 )
        particion = mbr_tmp[0].mbr_partition_4;
    else
    {
        printf("ERROR: Hay problemas con encontrar el nombre de la particion en el ambito de generar_ext3");
        return;
    }
    //si vamos por aqui la particion ya se encontro
    //======= AHORA ES MOMENTO DE CONFIGURAR EL SISTEMA DE ARCHIVOS ==================
    //sacando n
    double calculo_bloque = (particion.part_size - sizeof(SB))/(1 + sizeof(LOG)+3+sizeof(TI)+3*sizeof(BC));
    if(calculo_bloque > 0)
    {
        int n = floor(calculo_bloque);

        //escribiendo SUPER BLOQUE
        SB sb;
        fseek(archivo,particion.part_start,SEEK_SET);
        fwrite(&sb,sizeof(SB),1,archivo);
        //reservado espacio para el journaling
        int inicio_log = particion.part_start + sizeof(SB)+1;
        int size_reservado_log = inicio_log + (n*sizeof(LOG));
        //posicionando puntero en bitmap de inodos & llenado de ceros
        int ini_bm_inodo = size_reservado_log + 1;
        int limite_bm_i = ini_bm_inodo + n;

        for(int i = ini_bm_inodo; i < limite_bm_i; i++)
        {
            fseek(archivo,i,SEEK_SET);
            fwrite("0",sizeof(char),1,archivo);
        }
        //posicionando puntero en bitmap de bloques & llenado de ceros
        int ini_bm_block = limite_bm_i +1;
        int limite_bm_b = ini_bm_block + ( 3 * n);
        
        for(int i = ini_bm_block; i < limite_bm_b; i++)
        {
            fseek(archivo,i,SEEK_SET);
            fwrite("0",sizeof(char),1,archivo);
        }

        


    }
    else
    {
        printf("ERROR: No se puede generar un sistema de archivos ext3 porque en la particion: %s no hay espacio\n",name_particion);
        return;
    }

}
