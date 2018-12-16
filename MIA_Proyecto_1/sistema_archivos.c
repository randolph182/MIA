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

void crear_archivo_users(FILE *archivo,int size_particion,int inicio_particion,char *name_particion)
{
    double calculo_bloque = (size_particion - sizeof(SB))/(1 + sizeof(LOG)+3+sizeof(TI)+3*sizeof(BC));
    int n = floor(calculo_bloque);

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: FECHA
     time_t tiempo = time(0);
     struct tm *tlocal = localtime(&tiempo);
     char fecha[16];
    strftime(fecha,16,"%d/%m/%y",tlocal);

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: SUPER BLOQUE
    fseek(archivo,inicio_particion,SEEK_SET);
    SB sb_tmp[1];
    fread(sb_tmp,sizeof(SB),1,archivo);

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: INODO 1
    TI inodo_carp;
    //inicializando punteros
    for(int i = 0; i < 15; i++)
        inodo_carp.i_block[i] = -1;

    inodo_carp.i_uid = 1; //usurio root
    inodo_carp.i_gid = 1; //grupo root
    inodo_carp.i_size = 29; //total de bytes en archivos
    inodo_carp.i_atime[0] = '0';
    strcpy(inodo_carp.i_ctime,fecha);
    strcpy(inodo_carp.i_mtime,fecha);
    inodo_carp.i_type = '0';
    inodo_carp.i_perm = 777;
    inodo_carp.i_block[0] = 0; //bitmap
    //escribiendo la tabla de inodos y actualizando su bitmap
    fseek(archivo,sb_tmp[0].s_first_ino,SEEK_SET);
    fwrite(&inodo_carp,sizeof(TI),1,archivo);
    //actualizando bitmap
    fseek(archivo,sb_tmp[0].s_bm_inode_start,SEEK_SET);
    fwrite("1",sizeof(char),1,archivo);
    sb_tmp[0].s_first_ino = sb_tmp[0].s_first_ino + sizeof(TI); //actualizando SB

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: BLOQUE 1
    BC bloque_carp;
    //inicializando el contenido del bloque de carpeta
    for(int i = 0; i < 4; i++)
    {
        bloque_carp.b_content[i].b_name[0] = '0'; //mas adelante me servira compararlo con ascii 48 para denotar que esta vacio
        bloque_carp.b_content[i].b_inodo = -1; //vacio
    }
    //escribiendo el primer bloque de carpetas y actualizando su bitmap
    char nombre_block_actual[12];
    strcpy(nombre_block_actual,".");
    memset(bloque_carp.b_content[0].b_name,0,sizeof(bloque_carp.b_content[0].b_name));
    strcpy(bloque_carp.b_content[0].b_name,nombre_block_actual);
    bloque_carp.b_content[0].b_inodo = 0; //actual
    char nombre_padre[12];
    strcpy(nombre_padre,"..");
     memset(bloque_carp.b_content[1].b_name,0,sizeof(bloque_carp.b_content[1].b_name));
    strcpy(bloque_carp.b_content[1].b_name,nombre_padre);
    bloque_carp.b_content[1].b_inodo = 0; //padre
    char nombre_archivo[12];
    strcpy(nombre_archivo,"users.txt");
    memset(bloque_carp.b_content[2].b_name,0,sizeof(bloque_carp.b_content[2].b_name));
    strcpy(bloque_carp.b_content[2].b_name,nombre_archivo);
    bloque_carp.b_content[2].b_inodo = 1; //inodo
    //escribiendo structura
    fseek(archivo,sb_tmp[0].s_block_start,SEEK_SET);
    fwrite(&bloque_carp,sizeof(BC),1,archivo);
    //escribiendo bitmap
    fseek(archivo,sb_tmp[0].s_bm_block_start,SEEK_SET);
    fwrite("1",sizeof(char),1,archivo);
    sb_tmp[0].s_first_blo = sb_tmp[0].s_block_start + sizeof(BC);  //actualizando SB
    //actualizando el primer puntero de la tabla de indos carpeta;

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACTUALIZANDO SUPER BLOQUE
    sb_tmp[0].s_free_inodes_count = sb_tmp[0].s_free_inodes_count  -1;
    sb_tmp[0].s_free_blocks_count = sb_tmp[0].s_free_blocks_count -1;

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: INODO 2
    TI inodo_archivo;
    inodo_archivo.i_uid = 1;
    inodo_archivo.i_gid = 1;
    inodo_archivo.i_size = 29;
    inodo_archivo.i_atime[0] = '0';
    strcpy(inodo_archivo.i_ctime,fecha);
    strcpy(inodo_archivo.i_mtime,fecha);
    inodo_archivo.i_type = '1';
    inodo_archivo.i_perm = 777;
    inodo_archivo.i_block[0] = 1; //bm bloque
    fseek(archivo,sb_tmp[0].s_first_ino,SEEK_SET);
    fwrite(&inodo_archivo,sizeof(TI),1,archivo);
    //actualizando su bitmap de tabla de inodos
    fseek(archivo,sb_tmp[0].s_bm_inode_start + 1 ,SEEK_SET);
    fwrite("1",sizeof(char),1,archivo);
    //actualizando sb
    sb_tmp[0].s_first_ino = sb_tmp[0].s_first_ino + sizeof(TI);
    sb_tmp[0].s_free_inodes_count = sb_tmp[0].s_free_inodes_count -1;
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: BLOQUE 2
    //crando bloque de archivos
    BA bloque_archivo;
    char users[64];
    strcpy(users,"1,G,root\n1,U,root,root,123\n");
    strcpy(bloque_archivo.b_content,users);
    fseek(archivo,sb_tmp[0].s_first_blo,SEEK_SET);
    fwrite(&bloque_archivo,sizeof(BA),1,archivo);
    //actualizando su bitmap
    fseek(archivo,sb_tmp[0].s_bm_block_start+1,SEEK_SET);
    fwrite("1",sizeof(char),1,archivo);
    //actualizando sb
    sb_tmp[0].s_first_blo = sb_tmp[0].s_first_blo + sizeof(BA);
    sb_tmp[0].s_free_blocks_count = sb_tmp[0].s_free_blocks_count -1;

    //escribiendo sb
    fseek(archivo,inicio_particion,SEEK_SET);
    fwrite(sb_tmp,sizeof(SB),1,archivo);

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

void full_particion(FILE *archivo,int particion_start,int particion_size)
{
    fseek(archivo,particion_start,SEEK_SET);
    SB sb_tmp[1];
    fread(sb_tmp,sizeof(SB),1,archivo);

    //limpiando bitmap de inodos y bloques
    for(int i = sb_tmp[0].s_inode_start; i < particion_start + particion_size; i++)
        fwrite("\0",sizeof(char),1,archivo);
    
}
