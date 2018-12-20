#include "sistema_archivos.h"

int crear_ext3(FILE *archivo, int size_particion, int inicio_particion, char *name_particion)
{
    double calculo_bloque = (size_particion - sizeof(SB)) / (1 + sizeof(LOG) + 3 + sizeof(TI) + 3 * sizeof(BC));
    if (calculo_bloque > 0)
    {
        int n = floor(calculo_bloque);
        //configurando y creando SUPER BLOQUE
        SB sb;
        //reservado espacio para el journaling
        int inicio_log = inicio_particion + sizeof(SB);
        int size_reservado_log = inicio_log + (n * sizeof(LOG));
        //posicionando puntero en bitmap de inodos & llenado de ceros
        int ini_bm_inodo = size_reservado_log;
        int limite_bm_i = ini_bm_inodo + n;
        int count_inode = 0;
        for (int i = ini_bm_inodo; i < limite_bm_i; i++)
        {
            fseek(archivo, i, SEEK_SET);
            fwrite("0", sizeof(char), 1, archivo);
            count_inode++;
        }
        //posicionando puntero en bitmap de bloques & llenado de ceros
        int ini_bm_block = limite_bm_i;
        int limite_bm_b = ini_bm_block + (3 * n);
        int count_block;
        for (int i = ini_bm_block; i < limite_bm_b; i++)
        {
            fseek(archivo, i, SEEK_SET);
            fwrite("0", sizeof(char), 1, archivo);
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
        sb.s_mnt_count = 0; //cero veces montado
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
        fseek(archivo, inicio_particion, SEEK_SET);
        fwrite(&sb, sizeof(SB), 1, archivo);

        return 1;
    }
    else
    {
        printf("ERROR: No se puede generar un sistema de archivos ext3 porque en la particion: %s no hay espacio\n", name_particion);
        return 0;
    }
}

void crear_archivo_users(FILE *archivo, int size_particion, int inicio_particion, char *name_particion)
{
    double calculo_bloque = (size_particion - sizeof(SB)) / (1 + sizeof(LOG) + 3 + sizeof(TI) + 3 * sizeof(BC));
    int n = floor(calculo_bloque);

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: FECHA
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char fecha[16];
    strftime(fecha, 16, "%d/%m/%y", tlocal);

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: SUPER BLOQUE
    fseek(archivo, inicio_particion, SEEK_SET);
    SB sb_tmp[1];
    fread(sb_tmp, sizeof(SB), 1, archivo);

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: INODO 1
    TI inodo_carp;
    //inicializando punteros
    for (int i = 0; i < 15; i++)
        inodo_carp.i_block[i] = -1;

    inodo_carp.i_uid = 1;   //usurio root
    inodo_carp.i_gid = 1;   //grupo root
    inodo_carp.i_size = 27; //total de bytes en archivos
    inodo_carp.i_atime[0] = '0';
    strcpy(inodo_carp.i_ctime, fecha);
    strcpy(inodo_carp.i_mtime, fecha);
    inodo_carp.i_type = '0';
    inodo_carp.i_perm = 777;
    inodo_carp.i_block[0] = 0; //bitmap
    //escribiendo la tabla de inodos y actualizando su bitmap
    fseek(archivo, sb_tmp[0].s_first_ino, SEEK_SET);
    fwrite(&inodo_carp, sizeof(TI), 1, archivo);
    //actualizando bitmap
    fseek(archivo, sb_tmp[0].s_bm_inode_start, SEEK_SET);
    fwrite("1", sizeof(char), 1, archivo);
    sb_tmp[0].s_first_ino = sb_tmp[0].s_first_ino + sizeof(TI); //actualizando SB

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: BLOQUE 1
    BC bloque_carp;
    //inicializando el contenido del bloque de carpeta
    for (int i = 0; i < 4; i++)
    {
        bloque_carp.b_content[i].b_name[0] = '0'; //mas adelante me servira compararlo con ascii 48 para denotar que esta vacio
        bloque_carp.b_content[i].b_inodo = -1;    //vacio
    }
    //escribiendo el primer bloque de carpetas y actualizando su bitmap
    char nombre_block_actual[12];
    strcpy(nombre_block_actual, ".");
    memset(bloque_carp.b_content[0].b_name, 0, sizeof(bloque_carp.b_content[0].b_name));
    strcpy(bloque_carp.b_content[0].b_name, nombre_block_actual);
    bloque_carp.b_content[0].b_inodo = 0; //actual
    char nombre_padre[12];
    strcpy(nombre_padre, "..");
    memset(bloque_carp.b_content[1].b_name, 0, sizeof(bloque_carp.b_content[1].b_name));
    strcpy(bloque_carp.b_content[1].b_name, nombre_padre);
    bloque_carp.b_content[1].b_inodo = 0; //padre
    char nombre_archivo[12];
    strcpy(nombre_archivo, "users.txt");
    memset(bloque_carp.b_content[2].b_name, 0, sizeof(bloque_carp.b_content[2].b_name));
    strcpy(bloque_carp.b_content[2].b_name, nombre_archivo);
    bloque_carp.b_content[2].b_inodo = 1; //inodo
    //escribiendo structura
    fseek(archivo, sb_tmp[0].s_block_start, SEEK_SET);
    fwrite(&bloque_carp, sizeof(BC), 1, archivo);
    //escribiendo bitmap
    fseek(archivo, sb_tmp[0].s_bm_block_start, SEEK_SET);
    fwrite("1", sizeof(char), 1, archivo);
    sb_tmp[0].s_first_blo = sb_tmp[0].s_block_start + sizeof(BC); //actualizando SB
    //actualizando el primer puntero de la tabla de indos carpeta;

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACTUALIZANDO SUPER BLOQUE
    sb_tmp[0].s_free_inodes_count = sb_tmp[0].s_free_inodes_count - 1;
    sb_tmp[0].s_free_blocks_count = sb_tmp[0].s_free_blocks_count - 1;

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: INODO 2
    TI inodo_archivo;
    for (int i = 0; i < 15; i++)
        inodo_archivo.i_block[i] = -1;
    inodo_archivo.i_uid = 1;
    inodo_archivo.i_gid = 1;
    inodo_archivo.i_size = 27;
    inodo_archivo.i_atime[0] = '0';
    strcpy(inodo_archivo.i_ctime, fecha);
    strcpy(inodo_archivo.i_mtime, fecha);
    inodo_archivo.i_type = '1';
    inodo_archivo.i_perm = 777;
    inodo_archivo.i_block[0] = 1; //bm bloque
    fseek(archivo, sb_tmp[0].s_first_ino, SEEK_SET);
    fwrite(&inodo_archivo, sizeof(TI), 1, archivo);
    //actualizando su bitmap de tabla de inodos
    fseek(archivo, sb_tmp[0].s_bm_inode_start + 1, SEEK_SET);
    fwrite("1", sizeof(char), 1, archivo);
    //actualizando sb
    sb_tmp[0].s_first_ino = sb_tmp[0].s_first_ino + sizeof(TI);
    sb_tmp[0].s_free_inodes_count = sb_tmp[0].s_free_inodes_count - 1;
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: BLOQUE 2
    //crando bloque de archivos
    BA bloque_archivo;
    char users[64];
    strcpy(users, "1,G,root\n1,U,root,root,123\n");
    strcpy(bloque_archivo.b_content, users);
    fseek(archivo, sb_tmp[0].s_first_blo, SEEK_SET);
    fwrite(&bloque_archivo, sizeof(BA), 1, archivo);
    //actualizando su bitmap
    fseek(archivo, sb_tmp[0].s_bm_block_start + 1, SEEK_SET);
    fwrite("1", sizeof(char), 1, archivo);
    //actualizando sb
    sb_tmp[0].s_first_blo = sb_tmp[0].s_first_blo + sizeof(BA);
    sb_tmp[0].s_free_blocks_count = sb_tmp[0].s_free_blocks_count - 1;

    //escribiendo sb
    fseek(archivo, inicio_particion, SEEK_SET);
    fwrite(sb_tmp, sizeof(SB), 1, archivo);
}

void full_particion(FILE *archivo, int particion_start, int particion_size)
{
    fseek(archivo, particion_start, SEEK_SET);
    SB sb_tmp[1];
    fread(sb_tmp, sizeof(SB), 1, archivo);

    //limpiando bitmap de inodos y bloques
    for (int i = sb_tmp[0].s_inode_start; i < particion_start + particion_size; i++)
        fwrite("\0", sizeof(char), 1, archivo);
}

void consultar_usuarios(FILE *archivo, int ini_particion, int size_particion, char *path, LISTA_USR *const lst_usr)
{
    //sacando el sb
    SB sb_tmp[1];
    fseek(archivo, ini_particion, SEEK_SET);
    fread(sb_tmp, sizeof(SB), 1, archivo);
    //nos hubicamos en el archivo carpeta de la raiz
    fseek(archivo, sb_tmp[0].s_bm_inode_start + 1, SEEK_SET);
    char ti_verif;
    fread(&ti_verif, sizeof(char), 1, archivo);
    if (ti_verif == '1') //es porque si hay un inodo
    {
        int n = sb_tmp[0].s_inodes_count;                                        // ya que los inodos solo pesan n
        int pos_byte_ti = sb_tmp[0].s_bm_inode_start + sizeof(TI) + n + (3 * n); //aqui ya se que el bm esta en la pos 1 entonces solo se hace un sizeof
        fseek(archivo, pos_byte_ti, SEEK_SET);                                   //+1 porque es el segundo 1 del bitmap de inodos donde esta el archivo
        TI ti_tmp[1];
        fread(ti_tmp, sizeof(TI), 1, archivo);
        //consultando punteros archivos
        int bytes_archivo = ti_tmp[0].i_size;
        char *acum_archivo = (char *)malloc(sizeof(char) * ti_tmp[0].i_size); //maximo por cadena
        memset(acum_archivo, 0, sizeof(acum_archivo));
        char *acum = (char *)malloc(sizeof(char));
        memset(acum, 0, sizeof(acum));
        for (int i = 0; i < 12; i++)
        {
            if (ti_tmp[0].i_block[i] != -1)
            {
                //posicion del bitmap * 64 que pesan todos los bloques  + espacio del bm de bloques + espacio de la tabla de inodos
                int pos_byte_block = sb_tmp[0].s_bm_block_start + ti_tmp[0].i_block[i] * 64 + (3 * n) + n * sizeof(TI);
                BA block_archivo;
                fseek(archivo, pos_byte_block, SEEK_SET);
                fread(&block_archivo, sizeof(BA), 1, archivo);

                int i = 0;
                while (i < 64)
                {
                    acum[0] = block_archivo.b_content[i];
                    if (acum[0] == NULL)
                        break;
                    strcat(acum_archivo, acum);
                    i++;
                }
                //strcpy(acum_archivo2,&block_archivo.b_content);
                //strcat(acum_archivo,acum_archivo2);
                //if(bytes_archivo > 64)
                //    bytes_archivo = bytes_archivo - 64;
            }
        }
        listar_usuarios(ini_particion, size_particion, acum_archivo, ti_tmp[0].i_size, path, lst_usr);
    }
}

void listar_usuarios(int inicio_particion, int size_particion, char *acum_usr, int size_bytes, char *path, LISTA_USR *const lst_usr)
{
    char *acum2 = (char *)malloc(sizeof(char) * 40);
    memset(acum2, 0, sizeof(acum2));
    char *caracter = (char *)malloc(sizeof(char) * 1);
    memset(caracter, 0, sizeof(caracter));
    int pos = 0;
    int salir = 0;
    while (salir != 1)
    {
        caracter[0] = acum_usr[pos];
        if (caracter[0] == NULL)
        {
            salir = 1;
            break;
        }
        if (caracter[0] == '\n')
        {
            add_lst_usr_cadena(lst_usr, acum2, inicio_particion, size_particion, path);
            memset(acum2, 0, sizeof(acum2));
        }
        else
            strcat(acum2, caracter);
        pos++;
    }
}

int registrar_en_archivo(FILE *archivo, int ini_particion, char *nomb_usr, char *nomb_grp, char *pass_usr,char tipo)
{
    //sacando el sb
    SB sb_tmp[1];
    fseek(archivo, ini_particion, SEEK_SET);
    fread(sb_tmp, sizeof(SB), 1, archivo);
    //nos hubicamos en el archivo carpeta de la raiz
    fseek(archivo, sb_tmp[0].s_bm_inode_start + 1, SEEK_SET);
    char ti_verif;
    fread(&ti_verif, sizeof(char), 1, archivo);
    if (ti_verif == '1') //es porque si hay un inodo
    {
        int n = sb_tmp[0].s_inodes_count;                                        // ya que los inodos solo pesan n
        int pos_byte_ti = sb_tmp[0].s_bm_inode_start + sizeof(TI) + n + (3 * n); //aqui ya se que el bm esta en la pos 1 entonces solo se hace un sizeof
        fseek(archivo, pos_byte_ti, SEEK_SET);                                   //+1 porque es el segundo 1 del bitmap de inodos donde esta el archivo
        //tabla de archivos
        TI ti_tmp[1];
        fread(ti_tmp, sizeof(TI), 1, archivo);
        int size_actual_achivo = ti_tmp[0].i_size;
        //la idea es encontrar un espacio para guardar el nuevo usuario
        //Primero consultamos sus apuntadores directos
        float ap_block = size_actual_achivo / 64.00; //ya que 64 mide cada bloque
        int ap_block_int = floor(ap_block);
        if (ap_block == ap_block_int) //el bloque esta lleno necesito moverme una posicion adelante
        {
            ap_block_int++;
            if (ap_block_int > 11)
            {
                printf("ADVERTENCIA: aun no se ha declarado bloques indirectos\n\n");
                return 0;
            }
        }

        if(ti_tmp[0].i_block[ap_block_int] == -1) //el bloque anterior debio haber estado lleno
        {
                int new_bm_block = -1;
                int new_byte_block = -1;
                crear_bloque_archivo(archivo, ini_particion, &new_bm_block, &new_byte_block);

                if (new_bm_block != -1)
                    ti_tmp[0].i_block[ap_block_int] = new_bm_block;
                else
                {
                    printf("ERROR: No se puede crear otro bloque en el bitmap\n\n");
                    return 0;
                }

        }
        int pos_byte_block = sb_tmp[0].s_bm_block_start + ti_tmp[0].i_block[ap_block_int] * 64 + (3 * n) + n * sizeof(TI);


        int numero_mayor_bloque =0 ;

        if(tipo == 'U')
        {
            numero_mayor_bloque = numero_mayor_file_usr(archivo, ini_particion, 'U');
        }
        else if(tipo == 'G')
            numero_mayor_bloque = numero_mayor_file_usr(archivo, ini_particion, 'G');

        if (numero_mayor_bloque != 0)
        {
            //calculando el numero de bytes que llevara la siguiente cadena en el archivo
            int numero_total_bytes = 0;
            numero_mayor_bloque++; //ya seria el dato que le toca

            if (numero_mayor_bloque > 9)
                numero_total_bytes += 2; //dos digitos
            else if (numero_mayor_bloque > 99)
                numero_total_bytes += 3;
            else
                numero_total_bytes += 1;

            if(tipo == 'U')
            {
                numero_total_bytes += strlen(nomb_usr);
                numero_total_bytes += strlen(nomb_grp);
                numero_total_bytes += strlen(pass_usr);
                numero_total_bytes += 6; //incluye 4 comas y el tipo y saltolinea
            }
            else
            {
                numero_total_bytes += strlen(nomb_grp);
                numero_total_bytes += 4; //incluye 2 comas y el tipo y saltolinea
            }

            char *info_final = (char *)malloc(sizeof(char) * numero_total_bytes);
            memset(info_final, 0, sizeof(info_final));
            char *acum_char = (char *)malloc(sizeof(char) * 1);
            memset(acum_char, 0, sizeof(acum_char));
            char str[12];
            sprintf(str, "%d", numero_mayor_bloque);
            strcat(info_final, str);

            if(tipo == 'U')
            {
                strcat(info_final, ",");
                strcat(info_final, "U");
                strcat(info_final, ",");
                strcat(info_final, nomb_grp);
                strcat(info_final, ",");
                strcat(info_final, nomb_usr);
                strcat(info_final, ",");
                strcat(info_final, pass_usr);
                strcat(info_final, "\n");
            }
            else
            {
                strcat(info_final, ",");
                strcat(info_final, "G");
                strcat(info_final, ",");
                strcat(info_final, nomb_grp);
                strcat(info_final, "\n");
            }


            BA bloque_archivo;
            fseek(archivo, pos_byte_block, SEEK_SET);
            fread(&bloque_archivo, sizeof(BA), 1, archivo);

            int numero_total_bytes_final = size_actual_achivo + numero_total_bytes;
            int bytes_block = contar_bytes_block(archivo,pos_byte_block);
            int total_bytes = bytes_block + numero_total_bytes;
            if (total_bytes > 64) //significa que se necesita otro bloque porque en este ya no cabe
            {
                //llenamos el bloque actual hasta donde le quepa la informacion
                int no_caract_restantes = total_bytes - 64;
                int no_caract_escribir = numero_total_bytes - no_caract_restantes;
                for (int i = 0; i < no_caract_escribir; i++)
                {
                    acum_char[0] = info_final[i];
                    strcat(&bloque_archivo.b_content, acum_char);
                }
                //escribiendo el bloque actualizado
                fseek(archivo, pos_byte_block, SEEK_SET);
                fwrite(&bloque_archivo, sizeof(BA), 1, archivo);
                //actualizando y escribiendo tabla de archivos
                int new_bm_block = -1;
                int new_byte_block = -1;
                crear_bloque_archivo(archivo, ini_particion, &new_bm_block, &new_byte_block);

                if (new_bm_block != -1)
                {
                    //ahora ingresamos la informacion restante;
                    BA new_block;
                    fseek(archivo, new_byte_block, SEEK_SET);
                    fread(&new_block, sizeof(BA), 1, archivo);

                    for (int i = no_caract_escribir; i < numero_total_bytes; i++)
                    {
                        acum_char[0] = info_final[i];
                        strcat(&new_block.b_content, acum_char);
                    }
                    //escribiendo el nuevo bloque
                    fseek(archivo, new_byte_block, SEEK_SET);
                    fwrite(&new_block, sizeof(BA), 1, archivo);

                    //buscando nueva posicino en la tabla de inodos para el bloque

                    for (int i = 0; i < 12; i++)
                    {
                        if (ti_tmp[0].i_block[i] == -1)
                        {
                            ti_tmp[0].i_block[i] = new_bm_block;
                            break;
                        }
                    }
                    //preguntar si luego se necesita utilizar los extendidos

                    //actualizando datos en inodo de archivos
                    ti_tmp[0].i_size = numero_total_bytes_final;
                    fseek(archivo, pos_byte_ti, SEEK_SET);
                    fwrite(ti_tmp, sizeof(TI), 1, archivo);
                }
                else
                {
                    printf("ERROR: No se puede crear otro bloque en el bitmap\n\n");
                    return 0;
                }
            }
            else //aun cabe la informacion en el bloque
            {
                strcat(&bloque_archivo.b_content, info_final);
                //escribiendo el bloque actualizado
                fseek(archivo, pos_byte_block, SEEK_SET);
                fwrite(&bloque_archivo, sizeof(BA), 1, archivo);
                //actualizando y escribiendo tabla de archivos
                ti_tmp[0].i_size = numero_total_bytes_final;
                fseek(archivo, pos_byte_ti, SEEK_SET);
                fwrite(ti_tmp, sizeof(TI), 1, archivo);
                TI ti_tmp2[1];
                fseek(archivo, pos_byte_ti, SEEK_SET);
                fread(ti_tmp2, sizeof(TI), 1, archivo);
                int i = 0;
                int j = 2;
                return 1;
            }
        }
        else
            return 0;
    }
}

int numero_mayor_file_usr(FILE *archivo, int ini_particion, char tipo)
{
    //sacando el sb
    SB sb_tmp[1];
    fseek(archivo, ini_particion, SEEK_SET);
    fread(sb_tmp, sizeof(SB), 1, archivo);
    //nos hubicamos en el archivo carpeta de la raiz
    fseek(archivo, sb_tmp[0].s_bm_inode_start + 1, SEEK_SET);
    char ti_verif;
    fread(&ti_verif, sizeof(char), 1, archivo);
    if (ti_verif == '1') //es porque si hay un inodo
    {
        int n = sb_tmp[0].s_inodes_count;                                        // ya que los inodos solo pesan n
        int pos_byte_ti = sb_tmp[0].s_bm_inode_start + sizeof(TI) + n + (3 * n); //aqui ya se que el bm esta en la pos 1 entonces solo se hace un sizeof
        fseek(archivo, pos_byte_ti, SEEK_SET);                                   //+1 porque es el segundo 1 del bitmap de inodos donde esta el archivo
        TI ti_tmp[1];
        fread(ti_tmp, sizeof(TI), 1, archivo);
        //consultando punteros archivos
        char *acum_archivo = (char *)malloc(sizeof(char) * ti_tmp[0].i_size); //maximo por cadena
        memset(acum_archivo, 0, sizeof(acum_archivo));
        char *acum = (char *)malloc(sizeof(char));
        memset(acum, 0, sizeof(acum));

        for (int i = 0; i < 12; i++)
        {
            if (ti_tmp[0].i_block[i] != -1)
            {
                //posicion del bitmap * 64 que pesan todos los bloques  + espacio del bm de bloques + espacio de la tabla de inodos
                int pos_byte_block = sb_tmp[0].s_bm_block_start + ti_tmp[0].i_block[i] * 64 + (3 * n) + n * sizeof(TI);
                BA block_archivo;
                fseek(archivo, pos_byte_block, SEEK_SET);
                fread(&block_archivo, sizeof(BA), 1, archivo);

                int i = 0;
                while (i < 64)
                {
                    acum[0] = block_archivo.b_content[i];
                    if (acum[0] == NULL)
                        break;
                    strcat(acum_archivo, acum);
                    i++;
                }
            }
        }
        char *token_salto;
        char *token_coma;

        int pos_actual = 0;
        int id_mayor = 0;
        int id_actual = 0;
        while ((token_salto = strtok_r(acum_archivo, "\n", &acum_archivo)))
        {
            pos_actual = 1;
            while ((token_coma = strtok_r(token_salto, ",", &token_salto)))
            {
                if (pos_actual == 1)
                    id_actual = atoi(token_coma);
                else if (pos_actual == 2)
                {
                    if (tipo == token_coma[0])
                    {
                        if (id_actual > id_mayor)
                            id_mayor = id_actual;
                    }
                }
                pos_actual++;
            }
        }
        return id_mayor;
    }
    return 0;
}

/* su responsabilidad es encontrar un espacion en el bm de blckes y crear el bloque luego retornar la informacion*/
void crear_bloque_archivo(FILE *archivo, int ini_particion, int *new_pos_bm, int *new_pos_byte)
{
    //sacando super_bloque
    SB sb[1];
    fseek(archivo, ini_particion, SEEK_SET);
    fread(sb, sizeof(sb), 1, archivo);

    int limite_bm_block = sb[0].s_bm_block_start + 3 * sb[0].s_inodes_count;
    int ini_bm_block = sb[0].s_bm_block_start;

    int count_bm = 0;
    for (int i = ini_bm_block; i < limite_bm_block; i++)
    {
        char bit;
        fseek(archivo, i, SEEK_SET);
        fread(&bit, sizeof(char), 1, archivo);
        if (bit == '0')
        {
            fseek(archivo, i, SEEK_SET);
            fwrite("1", sizeof(char), 1, archivo);
            int posicion_bloque = ini_bm_block + (3 * sb[0].s_inodes_count) + (sizeof(TI) * sb[0].s_inodes_count) + 64 * count_bm;
            fseek(archivo, posicion_bloque, SEEK_SET);
            BA nuevo_bloque;
            memset(&nuevo_bloque.b_content, 0, sizeof(BA));
            fwrite(&nuevo_bloque, sizeof(BA), 1, archivo);

            *new_pos_bm = count_bm;
            *new_pos_byte = posicion_bloque;
            break;
        }
        count_bm++;
    }
}

int contar_bytes_block(FILE *archivo,int pos_byte_block)
{
    BA block_archivo;
    fseek(archivo,pos_byte_block,SEEK_SET);
    fread(&block_archivo,sizeof(BA),1,archivo);

    int contador = 0;


    for(int i = 0; i < 64; i++)
    {
        if(block_archivo.b_content[i] == NULL)
            break;
        contador++;
    }
    return contador;
}

int ejecutar_mkdir(FILE *archivo,int ini_particion,char *path,int p)
{
    //primero listamos todas las carpetas
    CHAR_ARRAY carpetas[30];

    int contador_carpeta = 0;
    char *path_tmp ;
    char *nombre_carpeta;
    while ((nombre_carpeta = strtok_r(path, "/", &path))) //nos movemos carpeta por carpeta
    {
        strcpy(carpetas[contador_carpeta].info,nombre_carpeta);
        carpetas[contador_carpeta].estado = 1;
        contador_carpeta++;
    }

    int crear = estado_crear_carpeta(archivo,ini_particion,carpetas,contador_carpeta,p);

    if(crear == 1)
    {
        int padre = 0; //padre siempre inicia en el inodo 1
        int hijo = -1;

        for(int i = 0; i < contador_carpeta; i++)
        {
            verificar_carpeta(archivo,ini_particion,&carpetas[i],padre,&hijo);
            if(hijo != -1 ) //consultamos si existe la carpeta
            {
                padre = hijo;
                hijo = -1;
            }
            else if(hijo == -1) //no existe entonces la creamos
            {
                int new_bm = 0;
                int exito = crear_carpeta_mkdir(archivo,ini_particion,&carpetas[i], padre,&new_bm);
                if(exito !=0 )
                {
                    padre = new_bm;
                    hijo = -1;
                }
                else
                {
                    printf("ERROR: no se pudo seguir con la secuencia de creacion de carpetas con path: %s\n",path);
                    break;
                }
            }
        }
    }
    else
        printf("Problemas con el path  %s\n",path);

}

void verificar_carpeta(FILE *archivo,int ini_particion,char *nombre_c,int bm_padre,int *bm_hijo)
{
    //sacando el sb
    SB sb_tmp[1];
    fseek(archivo, ini_particion, SEEK_SET);
    fread(sb_tmp, sizeof(SB), 1, archivo);

    //posicion en byte en inodo padre
    int pos_byte_ip = sb_tmp[0].s_bm_inode_start + sb_tmp[0].s_inodes_count + (3 * sb_tmp[0].s_inodes_count) + bm_padre * sizeof(TI);
    TI inodo_padre[1];
    fseek(archivo,pos_byte_ip,SEEK_SET);
    fread(inodo_padre,sizeof(TI),1,archivo);

    //recorremos sus apuntadores directos
    int pos_byte_bh; //BLOQUE HIJO
    for(int i = 0; i < 12; i++)
    {
         if(inodo_padre[0].i_block[i] != -1) //accedemos a su apuntador de contenido
         {
            //accedems al bloque hijo verificando si existe
            pos_byte_bh = sb_tmp[0].s_bm_block_start + (3 * sb_tmp[0].s_inodes_count) + ( sb_tmp[0].s_inodes_count * sizeof(TI)) + inodo_padre[0].i_block[i] * 64;
            BC bloque_carpeta;
            fseek(archivo,pos_byte_bh,SEEK_SET);
            fread(&bloque_carpeta,sizeof(BC),1,archivo);

            for(int j = 0; j < 4; j++)
            {
                if(strcmp(bloque_carpeta.b_content[j].b_name,nombre_c) == 0)
                {
                    *bm_hijo = bloque_carpeta.b_content[j].b_inodo;
                    return ;
                }
            }

         }
    }

   *bm_hijo = -1;
}

int crear_carpeta_mkdir(FILE * archivo,int ini_particion,char *nombre,int bm_padre,int *new_bm)
{
     //sacando el sb
    SB sb_tmp[1];
    fseek(archivo, ini_particion, SEEK_SET);
    fread(sb_tmp, sizeof(SB), 1, archivo);

    int pos_byte_ip = sb_tmp[0].s_bm_inode_start + sb_tmp[0].s_inodes_count + (3 * sb_tmp[0].s_inodes_count) + bm_padre * sizeof(TI);
    //hubicacion en el bitmap padre
    TI inodo_padre[1];
    fseek(archivo,pos_byte_ip,SEEK_SET);
    fread(inodo_padre,sizeof(TI),1,archivo);

    //recorremos sus apuntadores directos
    int pos_byte_bh;
    for(int i = 0; i < 12; i++)
    {
         if(inodo_padre[0].i_block[i] != -1)
         {
            //accedems al bloque hijo verificando si existe
            pos_byte_bh = sb_tmp[0].s_bm_block_start + (3 * sb_tmp[0].s_inodes_count) + ( sb_tmp[0].s_inodes_count * sizeof(TI)) + inodo_padre[0].i_block[i] * 64;
            BC bloque_carpeta;
            fseek(archivo,pos_byte_bh,SEEK_SET);
            fread(&bloque_carpeta,sizeof(BC),1,archivo);

            for(int j = 0; j < 4; j++)
            {
                if(bloque_carpeta.b_content[j].b_inodo == -1) //significa que hay espacio para un apuntador
                {
                    int ap_bm_inodo = crear_inodo_carpeta(archivo,ini_particion);
                    if(ap_bm_inodo != -1)
                    {
                        memset(bloque_carpeta.b_content[j].b_name,0,sizeof(bloque_carpeta.b_content[j].b_name));
                        strcpy(bloque_carpeta.b_content[j].b_name,nombre); //comenzamos escribiendo el nombre
                        bloque_carpeta.b_content[j].b_inodo = ap_bm_inodo;
                        *new_bm = ap_bm_inodo;
                        //RE-ESCRIBIENDO EL BLOQUE CARPETA
                        fseek(archivo,pos_byte_bh,SEEK_SET);
                        fwrite(&bloque_carpeta,sizeof(BC),1,archivo);
                        return 1;
                    }
                    else
                        return 0; //ya no hace los respectivos procesos de crear

                }
            }
         }
         else // significa que hay espacio para un nuevo bloque pero solo que este es distinto
         {
            int pos_bm_bloque = crear_bloque_carpeta_bm(archivo,ini_particion);
            if(pos_bm_bloque != -1)
            {
                int ap_bm_inodo = crear_inodo_carpeta(archivo,ini_particion);
                if(ap_bm_inodo == -1)
                    return 0;
                //CONFIGURANDO EL NUEVO BLQOUE DE CARPETA
                BC nuevo_bloque;
                memset(nuevo_bloque.b_content[0].b_name,0,sizeof(nuevo_bloque.b_content[0].b_name));
                strcpy(nuevo_bloque.b_content[0].b_name,nombre);
                nuevo_bloque.b_content[0].b_inodo = ap_bm_inodo;
                int posicion_bloque = sb_tmp[0].s_bm_block_start + (3 * sb_tmp[0].s_inodes_count) + (sizeof(TI) * sb_tmp[0].s_inodes_count) + 64 * pos_bm_bloque;
                nuevo_bloque.b_content[1].b_inodo = -1;
                nuevo_bloque.b_content[2].b_inodo = -1;
                nuevo_bloque.b_content[3].b_inodo = -1;
                fseek(archivo, posicion_bloque, SEEK_SET);
                fwrite(&nuevo_bloque, sizeof(BC), 1, archivo);

                return 1;
            }
            else
            {
                printf("ERROR: El bitmap de bloque se quedo sin espacio para uno nuevo\n");
                return 0;
            }
        }
    }
}

int estado_crear_carpeta(FILE *archivo,int ini_particion,CHAR_ARRAY carpetas[],int contador_carpetas,int p)
{
    int contador_2 =0;
    int padre = 0; //padre siempre inicia en el inodo 1
    int hijo = -1;

    for(int i = 0; i < contador_carpetas; i++)
    {
        verificar_carpeta(archivo,ini_particion,&carpetas[i],padre,&hijo);
        if(hijo != -1 ) //si es distinto  a  -1 es porque si encontro un hijo con el nombre
        {
            padre = hijo;
            hijo = -1;
            contador_2++;
        }
        else if(hijo == -1) //no existe la carpeta
            break;
    }

    if(p == 1) //si esta activa de crear todas las carpetas
    {
        if(contador_2  == contador_carpetas)
        {
            printf("ADVERTENCIA: no se puede crear la serie de carpetas porque ya existen\n\n");
            return 0;
        }
        return 1;
    }
    else{ //sino entonces significa que no esta activa
        int result = contador_carpetas - contador_2;
        if(result == 1) //si sobra 1 es porque la ultima carpeta no existe y como no esta activa p entonces si se puede crear
            return 1;
        printf("ERROR: el path no puede ser creado porque no existen todas las carpetas y no esta activa -p para crearlas\n");
        return 0;
    }

}

int crear_inodo_carpeta(FILE *archivo,int ini_particion)
{
    //sacando el sb
    SB sb[1];
    fseek(archivo, ini_particion, SEEK_SET);
    fread(sb, sizeof(SB), 1, archivo);

    int inicio_bm_inodos = sb[0].s_bm_inode_start;
    int fin_bm_inodos = inicio_bm_inodos + sb[0].s_inodes_count; //lo que abarca el bm de inodos
    int count_bm_inodo = 0;
    //buscando su posicion en el bitmap
    int hubo_bm_inodo = 0;
    for (int i = inicio_bm_inodos; i < fin_bm_inodos; i++)
    {
        char bit;
        fseek(archivo, i, SEEK_SET);
        fread(&bit, sizeof(char), 1, archivo);
        if (bit == '0')
        {
            //escribiendo en el bm de inodos
            fseek(archivo, i, SEEK_SET);
            fwrite("1", sizeof(char), 1, archivo); //inodo
            hubo_bm_inodo = 1;
            break;
        }
        count_bm_inodo++;
    }

    if(hubo_bm_inodo == 1)
    {

        //::::::::::::::::::::::AHORA SE HACE EL PROCESO DE CREAR UN BLOUE DE CARPETAS
        int pos_bm_bloque = crear_bloque_carpeta_bm(archivo,ini_particion);
        if(pos_bm_bloque != -1)
        {
                //CONFIGURANDO EL NUEVO BLQOUE DE CARPETA
                BC nuevo_bloque;
                memset(nuevo_bloque.b_content[0].b_name,0,sizeof(nuevo_bloque.b_content[0].b_name));
                strcpy(nuevo_bloque.b_content[0].b_name,".");
                nuevo_bloque.b_content[0].b_inodo = pos_bm_bloque; //apuntador a el mismo
                memset(nuevo_bloque.b_content[1].b_name,0,sizeof(nuevo_bloque.b_content[0].b_name));
                strcpy(nuevo_bloque.b_content[1].b_name,"..");
                nuevo_bloque.b_content[1].b_inodo = count_bm_inodo;
                nuevo_bloque.b_content[2].b_inodo = -1;
                nuevo_bloque.b_content[3].b_inodo = -1;
                int posicion_bloque = sb[0].s_bm_block_start + (3 * sb[0].s_inodes_count) + (sizeof(TI) * sb[0].s_inodes_count) + 64 * pos_bm_bloque;
                fseek(archivo, posicion_bloque, SEEK_SET);
                fwrite(&nuevo_bloque, sizeof(BC), 1, archivo);

        }
        else
        {
            printf("ERROR: El bitmap de bloque se quedo sin espacio para uno nuevo\n");
            return -1;
        }

        //::::::::::::::::::::::::: ESCRIBIENDO INFORMACION EN EL  INODO :::::::::::::::::::
        int pos_inodo = inicio_bm_inodos + sb[0].s_inodes_count + (3 * sb[0].s_inodes_count) +  count_bm_inodo * sizeof(TI);
        TI inodo_nuevo;
        inodo_nuevo.i_block[0] = pos_bm_bloque;
        fseek(archivo, pos_inodo, SEEK_SET);
        fwrite(&inodo_nuevo, sizeof(BA), 1, archivo);
        return count_bm_inodo; //retornamos el bitmap
    }
    else
    {
        printf("ERROR: El bitmap de inodos se quedo sin espacio para uno nuevo\n");
        return -1;
    }
}

int  crear_bloque_carpeta_bm(FILE *archivo,int ini_particion)
{
    SB sb[1];
    fseek(archivo, ini_particion, SEEK_SET);
    fread(sb, sizeof(SB), 1, archivo);

    int limite_bm_block = sb[0].s_bm_block_start + 3 * sb[0].s_inodes_count;
    int ini_bm_block = sb[0].s_bm_block_start;

    int count_bm_bloque = 0;
    int hubo_bm_bloque = 0;
    for (int i = ini_bm_block; i < limite_bm_block; i++)
    {
        char bit;
        fseek(archivo, i, SEEK_SET);
        fread(&bit, sizeof(char), 1, archivo);
        if (bit == '0')
        {
            fseek(archivo, i, SEEK_SET);
            fwrite("1", sizeof(char), 1, archivo); //bm bloque
            return count_bm_bloque;
        }
        count_bm_bloque++;
    }
    return -1;
}
