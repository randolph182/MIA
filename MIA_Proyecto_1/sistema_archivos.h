#ifndef SISTEMA_ARCHIVOS_H_INCLUDED
#define SISTEMA_ARCHIVOS_H_INCLUDED

#include "admin_disco.h"
#include "lista_simple.h"
#include "lista_users.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

typedef struct SUPER_BLOQUE SB;
typedef struct TABLA_INODOS TI;
typedef struct JOURNALING LOG;
typedef struct BLOQUE_CARPETAS BC;
typedef struct CONTENT  CNT;
typedef struct BLOQUE_ARCHIVOS  BA;
typedef struct BLOQUE_APUNTADORES  BAP;


struct SUPER_BLOQUE
{
    int s_filesystem_type; //Guarda el numero que identifica el sistema de archivos utilizado
    int s_inodes_count;     //Guarda el número total de inodos
    int s_blocks_count;     //Guarda el número total de bloques
    int s_free_blocks_count; //Contiene el número de bloques libres
    int s_free_inodes_count; //Contiene el número de inodos libres
    char s_mtime[16];             //Ultima fecha en el que el sistema fue montado
    char s_umtime[16];            //Ultima fecha en que el sistema fue desmontado
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
    char i_atime[16];            //Última fecha en que se leyó el inodo sin modificarlo
    char i_ctime[16];            //Fecha en la que se creó el inodo
    char i_mtime[16];            //Úlitma fecha en la que se modificó el inodo
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

struct JOURNALING
{
    char Journal_Tipo_Operacion;    //El tipo de operación a realizarse
    char Journal_tipo;              //Si es Archivo(0), si es carpeta(1)
    char Journal_nombre[12];        //Nombre archivo o directorio
    char Journal_contenido[200];    //Si hay datos contenidos
    char Journal_fecha[16];         //Fecha de la transacción
    int Journal_propietario;        //Es el propietario archivo o directorio
    int Journal_permisos;           //Son los permisos que tiene el archivo o directorio
};

struct CONTENT
{
    char b_name[12];    //Nombre de la carpeta o archivo
    int b_inodo;        //Apuntador hacia un inodo asociado al archivo o carpeta
};

struct BLOQUE_CARPETAS
{
    CNT b_content[4];   //Array con el contenido de la carpeta
};

struct BLOQUE_ARCHIVOS
{
    char b_content[64];  //Array con el contenido del archivo
};

struct BLOQUE_APUNTADORES
{
    int b_pointers[16]; //Array con los apuntadores hacia bloques (de archivo o carpeta)
};

int crear_ext3(FILE *archivo,int size_particion,int inicio_particion,char *name_particion);
void crear_archivo_users(FILE *archivo,int size_particion,int inicio_particion,char *name_particion);

void full_particion(FILE *archivo,int particion_start,int particion_size);

void consultar_usuarios(FILE *archivo,int ini_particion,int size_particion,char *path,LISTA_USR *const lst_usr);
//void read_file_ap_ind(FILE *archivo,int pos_bm_ap_ind,SB sb,char tipo_ap_ind,char *acumlador);
void listar_usuarios(int inicio_particion,int size_particion,char *acum_usr,int size_bytes,char *path,LISTA_USR *const lst_usr);

int registrar_usuario(FILE *archivo,int ini_particion,char *nomb_usr,char *nomb_grp,char *pass_usr);
int numero_mayor_file_usr(FILE *archivo,int pos_byte_block,char tipo);

void crear_bloque_archivo(FILE *archivo,int ini_particion,int *new_pos_bm,int *new_pos_byte);

#endif // SISTEMA_ARCHIVOS_H_INCLUDED
