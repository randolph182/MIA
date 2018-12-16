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

int crear_archivo_users(FILE *archivo,int size_particion,int inicio_particion,char *name_particion)
{
    double calculo_bloque = (size_particion - sizeof(SB))/(1 + sizeof(LOG)+3+sizeof(TI)+3*sizeof(BC));
    int n = floor(calculo_bloque);

    //obteniendo super bloque
    fseek(archivo,inicio_particion,SEEK_SET);
    SB sb_tmp[1];
    fread(sb_tmp,sizeof(SB),1,archivo);
    //creando el primer inodo
    TI inodo_carp;
    //inicializando el puntero de los bloques directos e indirectos
    for(int i = 0; i < 15; i++)
        inodo_carp.i_block[i] = -1;

    //creando el primer bloque_carpeta
    BC bloque_carp;
    //inicializando el contenido del bloque de carpeta
    for(int i = 0; i < 4; i++)
    {
        bloque_carp.b_content[i].b_name[0] = '0'; //mas adelante me servira compararlo con ascii 48 para denotar que esta vacio
        bloque_carp.b_content[i].b_inodo = -1; //vacio
    }
    //configurando la primer tabla de inodos;
    inodo_carp.i_uid = 0;
    inodo_carp.i_gid = 0;
    inodo_carp.i_size = 0;
    inodo_carp.i_atime[0] = '0';


    /*calculando el time de creacion de la tabla de inodos*/
     time_t tiempo = time(0);
     struct tm *tlocal = localtime(&tiempo);
     char fecha[16];
    strftime(fecha,16,"%d/%m/%y",tlocal);


    strcpy(inodo_carp.i_ctime,fecha);
    strcpy(inodo_carp.i_mtime,fecha);
    inodo_carp.i_type = '0';
    inodo_carp.i_perm = 000000000;
    inodo_carp.i_block[0] = sb_tmp[0].s_block_start;
    //escribiendo la tabla de inodos y actualizando su bitmap
    int espacio_inodos_libre = sb_tmp[0].s_first_ino;
    fseek(archivo,espacio_inodos_libre,SEEK_SET);
    fwrite(&inodo_carp,sizeof(TI),1,archivo);
    //actualizando bitmap
    int size_ocupado_ti = sb_tmp[0].s_first_ino - sb_tmp[0].s_inode_start;
    int bm_inodo_libre = espacio_inodos_libre - size_ocupado_ti - (3 * n) - n;
    fseek(archivo,bm_inodo_libre,SEEK_SET);
    fwrite("1",sizeof(char),1,archivo);
    sb_tmp[0].s_first_ino = espacio_inodos_libre + sizeof(TI); //actualizando SB

    //escribiendo el primer bloque de carpetas y actualizando su bitmap
    char nombre_padre[12];
    strcpy(nombre_padre,".");
    strcpy(bloque_carp.b_content[0].b_name,nombre_padre);
    bloque_carp.b_content[0].b_inodo = espacio_inodos_libre;
    char nombre_block_actual[12];
    strcpy(nombre_block_actual,"..");
    strcpy(bloque_carp.b_content[1].b_name,nombre_block_actual);
    bloque_carp.b_content[1].b_inodo = sb_tmp[0].s_block_start;
    char nombre_carpeta_raiz[12];
    strcpy(nombre_carpeta_raiz,"/");
    strcpy(bloque_carp.b_content[2].b_name,nombre_carpeta_raiz);
    //escribiendo
    fseek(archivo,sb_tmp[0].s_block_start,SEEK_SET);
    fwrite(&bloque_carp,sizeof(BC),1,archivo);
    int size_ocupado_block = sb_tmp[0].s_first_blo - sb_tmp[0].s_block_start;
    int bm_block_free = sb_tmp[0].s_first_blo - size_ocupado_block -(n*sizeof(TI)) - (3*n) - n;
    fseek(archivo,bm_block_free,SEEK_SET);
    fwrite("1",sizeof(char),1,archivo);
    sb_tmp[0].s_first_blo = sb_tmp[0].s_block_start + sizeof(BC);  //actualizando SB
    //actualizando el primer puntero de la tabla de indos carpeta;


    sb_tmp[0].s_free_inodes_count = sb_tmp[0].s_free_inodes_count  -1;
    sb_tmp[0].s_free_blocks_count = sb_tmp[0].s_free_blocks_count -1;
    //creando un inodo archivo
    TI inodo_archivo;
    inodo_archivo.i_uid = 0;
    inodo_archivo.i_gid = 0;
    inodo_archivo.i_size = 29;
    inodo_archivo.i_atime[0] = '0';
    strcpy(inodo_archivo.i_ctime,fecha);
    strcpy(inodo_archivo.i_mtime,fecha);
    inodo_archivo.i_type = '1';
    inodo_archivo.i_perm = 000000000;
    inodo_archivo.i_block[0] = sb_tmp[0].s_first_blo;
    fseek(archivo,sb_tmp[0].s_first_ino,SEEK_SET);
    fwrite(&inodo_archivo,sizeof(TI),1,archivo);
    //actualizando su bitmap de tabla de inodos
    size_ocupado_ti = sb_tmp[0].s_first_ino - sb_tmp[0].s_inode_start;
    bm_inodo_libre = sb_tmp[0].s_first_ino - size_ocupado_ti - (3 * n) - n;
    fseek(archivo,sb_tmp[0].s_bm_inode_start,SEEK_SET);
    fwrite("1",sizeof(char),1,archivo);
    //actualizando sb
    sb_tmp[0].s_first_ino = sb_tmp[0].s_first_ino + sizeof(TI);
    sb_tmp[0].s_free_inodes_count = sb_tmp[0].s_free_inodes_count -1;
    
    //crando bloque de archivos
    BA bloque_archivo;
    char users[64];
    strcpy(users,"1,G,root\n1,U,root,root,123\n");
    strcpy(bloque_archivo.b_content,users);
    fseek(archivo,sb_tmp[0].s_first_blo,SEEK_SET);
    fwrite(&bloque_archivo,sizeof(BA),1,archivo);
    //actualizando su bitmap
    size_ocupado_block = sb_tmp[0].s_first_blo - sb_tmp[0].s_block_start;
    bm_block_free = sb_tmp[0].s_first_blo - size_ocupado_block -(n*sizeof(TI)) - (3*n) - n;
    fseek(archivo,bm_block_free,SEEK_SET);
    fwrite("1",sizeof(char),1,archivo);
    //actualizando sb
    sb_tmp[0].s_first_blo = sb_tmp[0].s_first_blo + sizeof(BA);
    sb_tmp[0].s_free_blocks_count = sb_tmp[0].s_free_blocks_count -1;

    //escribiendo sb
    fseek(archivo,inicio_particion,SEEK_SET);
    fwrite(sb_tmp,sizeof(SB),1,archivo);
    
    fseek(archivo,inicio_particion,SEEK_SET);
    fread(sb_tmp,sizeof(SB),1,archivo);
    int i;
    int b;
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


