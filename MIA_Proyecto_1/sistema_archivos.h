#ifndef SISTEMA_ARCHIVOS_H_INCLUDED
#define SISTEMA_ARCHIVOS_H_INCLUDED

typedef struct SUPER_BLOQUE SB;
typedef struct TABLA_INODOS TI;


struct SUPER_BLOQUE
{
    int s_filesystem_type; //Guarda el numero que identifica el sistema de archivos utilizado
    int s_inodes_count;     //Guarda el número total de inodos
    int s_blocks_count;     //Guarda el número total de bloques
    int s_free_blocks_count; //Contiene el número de bloques libres
    int s_free_inodes_count; //Contiene el número de inodos libres
    int s_mtime;             //Ultima fecha en el que el sistema fue montado
    int s_umtime;            //Ultima fecha en que el sistema fue desmontado
    int s_mnt_count;         //Indica cuantas veces se ha montado el sistema
    int s_magic;             //Valor que identifica al sistema de archivos, tendrá el valor 0xEF53
    int s_inode_size;        //Tamaño del inodo
    int s_block_size;        //Tamaño del bloque
    int s_first_ino;         //Primer inodo libre
    int s_first_blo;         //Primer bloque libre
    int s_bm_inode_start;      //Guardará el inicio del bitmap de inodos
    int s_bm_block_start;    //Guardará el inicio del bitmap de bloques
    int s_inode_start;        //Guardará el inicio de la tabla de inodos
    int s_block_start;       //Guardará el inico de la tabla de bloques
};

struct TABLA_INODOS
{
    int i_uid;              //UID del usuario propietario del archivo o carpeta
    int i_gid;              //GID del grupo al que pertenece el archivo o carpeta.
    int i_size;             //Tamaño del archivo en bytes
    int i_atime;            //Última fecha en que se leyó el inodo sin modificarlo
    int i_ctime;            //Fecha en la que se creó el inodo
    int i_mtime;            //Úlitma fecha en la que se modificó el inodo
    int i_block[15];        /*Array en los que los primeros 12 registros son bloques directos.
                              *El 13 será el número del bloque simple indirecto
                              *El 14 será el número del bloque doble indirecto
                              *El 15 será el número del bloque triple indirecto
                              *Si no son utilizados tendrá el valor -1
                              */
   char i_type;             /*ndica si es archivo o carpeta. Tendrá los siguientes valores:
                            * 1 = archivo
                            * 0 = carpeta
                            */
    int i_perm;             /* Guardará los permisos del archivo o carpeta. Se trabajará a nivel de bits, estará dividido de la siguiente forma:
                             - Los primeros tres bits serán para el Usuario i_uid.
                             - Los siguientes tres bits serán para el Grupo al que pertenece el usuario.
                             - Y los últimos tres bits serán para los permisos de Otros usuarios.
                             * Cada grupo de tres bits significa lo siguiente:
                             - El primer bit indica el permiso de lectura R.
                             - El segundo bit indica el permiso de escritura W.
                             - El tercer bit indica el permiso de ejecución X.
                            */
};

#endif // SISTEMA_ARCHIVOS_H_INCLUDED
