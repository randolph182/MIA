#include "sistema_archivos.h"


int crear_ext3(FILE *archivo,int size_particion,int inicio_particion,char *name_particion)
{
    double calculo_bloque = (size_particion - sizeof(SB))/(1 + sizeof(LOG)+3+sizeof(TI)+3*sizeof(BC));
    if(calculo_bloque > 0)
    {
        int n = floor(calculo_bloque);
        //configurando y creando SUPER BLOQUE
        SB sb;
        //reservado espacio para el journaling
        int inicio_log = inicio_particion + sizeof(SB);
        int size_reservado_log = inicio_log + (n*sizeof(LOG));
        //posicionando puntero en bitmap de inodos & llenado de ceros
        int ini_bm_inodo = size_reservado_log ;
        int limite_bm_i = ini_bm_inodo + n;
        int count_inode =0;
        for(int i = ini_bm_inodo; i < limite_bm_i; i++)
        {
            fseek(archivo,i,SEEK_SET);
            fwrite("0",sizeof(char),1,archivo);
            count_inode++;
        }
        //posicionando puntero en bitmap de bloques & llenado de ceros
        int ini_bm_block = limite_bm_i;
        int limite_bm_b = ini_bm_block + ( 3 * n);
        int count_block;
        for(int i = ini_bm_block; i < limite_bm_b; i++)
        {
            fseek(archivo,i,SEEK_SET);
            fwrite("0",sizeof(char),1,archivo);
            count_block++;
        }
        //en adelante se reserva espacio para inodos y bloques
        //como es el inicio no se escriben structs por el momento teniendo en cuenta que
        //inodos reserva n * sizeof(inodos) ;  bloques reserva 3*n*sizeof(bloques)


        //ACTUALIZANDO EL SUPER BLOQUE
        sb.s_filesystem_type = 3;
        sb.s_inodes_count = count_inode;
        sb.s_blocks_count = count_block;
        sb.s_free_blocks_count = count_block;
        sb.s_free_inodes_count = count_inode;
        //no lleva los s_mtime & s_umtime porque no han sido montadas
        //memset(&sb.s_mtime,0,sizeof(sb.s_mtime));
        //memset(&sb.s_umtime,0,sizeof(sb.s_umtime));
        sb.s_mtime[0] = '0';
        sb.s_umtime[0] = '0';
        sb.s_mnt_count =0;  //cero veces montado
        sb.s_magic = 61267;
        sb.s_inode_size = sizeof(TI);
        sb.s_block_size = sizeof(BC);
        sb.s_first_ino = limite_bm_b;
        sb.s_first_blo = limite_bm_b + n * sizeof(TI);
        sb.s_bm_inode_start = ini_bm_inodo;
        sb.s_bm_block_start = ini_bm_block;
        sb.s_inode_start = limite_bm_b;
        sb.s_block_start = limite_bm_b + n * sizeof(TI);

        //cuardando el SUPER BLOQUE
        fseek(archivo,inicio_particion,SEEK_SET);
        fwrite(&sb,sizeof(SB),1,archivo);

        return 1;
    }
    else
    {
        printf("ERROR: No se puede generar un sistema de archivos ext3 porque en la particion: %s no hay espacio\n",name_particion);
        return 0;
    }
}

int crear_archivo_users(FILE *archivo,int inicio_particion,char *name_particion)
{
    //obteniendo super bloque
    fseek(archivo,inicio_particion,SEEK_SET);
    SB sb_tmp[1];
    fread(sb_tmp,sizeof(SB),1,archivo);

    if(sb_tmp[0].s_mtime[0] == 48)
    {
        //creando el primer inodo
        TI inodo_carp;

        //creando el primer bloque_carpeta
        BC bloque_carp;
        int i;
        printf("Hola Mundo");
    }


}

int generar_ext3_2(FILE *archivo,char *type_mkfs,char *name_particion,int *inicio_particion)
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
        return 0;
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
        int inicio_log = particion.part_start + sizeof(SB);
        int size_reservado_log = inicio_log + (n*sizeof(LOG));
        //posicionando puntero en bitmap de inodos & llenado de ceros
        int ini_bm_inodo = size_reservado_log ;
        int limite_bm_i = ini_bm_inodo + n;
        int count_inode =0;
        for(int i = ini_bm_inodo; i < limite_bm_i; i++)
        {
            fseek(archivo,i,SEEK_SET);
            fwrite("0",sizeof(char),1,archivo);
            count_inode++;
        }
        //posicionando puntero en bitmap de bloques & llenado de ceros
        int ini_bm_block = limite_bm_i;
        int limite_bm_b = ini_bm_block + ( 3 * n);
        int count_block;
        for(int i = ini_bm_block; i < limite_bm_b; i++)
        {
            fseek(archivo,i,SEEK_SET);
            fwrite("0",sizeof(char),1,archivo);
            count_block++;
        }
        //en adelante se reserva espacio para inodos y bloques
        //como es el inicio no se escriben structs por el momento teniendo en cuenta que
        //inodos reserva n * sizeof(inodos) ;  bloques reserva 3*n*sizeof(bloques)
        *inicio_particion = particion.part_start;

        //ACTUALIZANDO EL SUPER BLOQUE
        sb.s_filesystem_type = 3;
        sb.s_inodes_count = count_inode;
        sb.s_blocks_count = count_block;
        sb.s_free_blocks_count = count_inode;
        sb.s_free_inodes_count = count_block;

        return 1;
    }
    else
    {
        printf("ERROR: No se puede generar un sistema de archivos ext3 porque en la particion: %s no hay espacio\n",name_particion);
        return 0;
    }
}

int inicializando_users(FILE *archivo, int inicio_particion)
{

}


