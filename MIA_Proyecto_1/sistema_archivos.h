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
#include "vector_lst.h"

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
    char Journal_nombre[200];        //Nombre archivo o directorio
    char Journal_contenido[200];    //Si hay datos contenidos
    char Journal_fecha[16];         //Fecha de la transacción
    char Journal_propietario;        //Es el propietario archivo o directorio
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
//========================================================== JOURNAL =======================
void inicializar_journal(FILE *archivo,int ini_particion);
int registrar_journal(FILE *archivo,int ini_particion,char tipo_op,char tipo_log,char *nombre,char *contenido,char propietario,int permisos);
void listar_log(FILE *archivo,int ini_particion);
void ejecutar_recovery(FILE *archivo,NODO_USR *usr_logeado);

//=================================================================================
int crear_ext3(FILE *archivo,int size_particion,int inicio_particion,char *name_particion);
void crear_archivo_users(FILE *archivo,int size_particion,int inicio_particion,char *name_particion);

void full_particion(FILE *archivo,int particion_start,int particion_size);

void consultar_usuarios(FILE *archivo,int ini_particion,int size_particion,char *path,LISTA_USR *const lst_usr);

void listar_usuarios(int inicio_particion,int size_particion,char *acum_usr,int size_bytes,char *path,LISTA_USR *const lst_usr);

int registrar_en_archivo(FILE *archivo,int ini_particion,char *nomb_usr,char *nomb_grp,char *pass_usr,char tipo);
int numero_mayor_file_usr(FILE *archivo,int ini_particion,char tipo);

void crear_bloque_archivo(FILE *archivo,int ini_particion,int *new_pos_bm,int *new_pos_byte);

int contar_bytes_block(FILE *archivo,int pos_byte_block);

int ejecutar_mkdir(FILE *archivo,NODO_USR *usr_logeado,char *path,int p,int log); //falta agregar id usr & id grp

void verificar_carpeta(FILE *archivo,int ini_particion,char *nombre_c,int bm_padre,int *bm_hijo);

int crear_carpeta_mkdir(FILE * archivo,int ini_particion,char *nombre,int bm_padre,int *new_bm,int id_usr,int id_grp);

// DEVUELVE  1->CORRECTO ;  0-> ERROR
int estado_crear_carpeta(FILE *archivo,int ini_particion,CHAR_ARRAY carpetas[],int contador_carpetas,int p);

int crear_inodo_carpeta(FILE *archivo,int ini_particion,int id_usr,int id_grp,int bm_padre);

int crear_bloque_carpeta_bm(FILE *archivo,int ini_particion);

void  buscar_hijo_carpeta_apid(FILE *archivo,int ini_particion,char *nombre_c,int bm_bap,int *bm_hijo,int nivel);

int crear_bc_bap(FILE *archivo,char *nombre_c,int ini_particion,int bm_bap_padre,int bm_padre,int id_usr,int id_grp,int *new_bm,int tipo_bap);

int crear_carpeta_bloque_ap(FILE *archivo,char *nombre_c,int ini_particion,int bm_padre,int id_usr,int id_grp,int *new_bm,int tipo_bap);

//========================================================== METODOS QUE SE UTILIZARON PARA MKFILE =======================
int ejecutar_mkfile(FILE *archivo,NODO_USR *usr_logeado,char *path,int p,int size,char *contenido,int log);

int crear_inodo_archivo(FILE *archivo,NODO_USR *usr_logeado);
int buscar_archivo(FILE *archivo,int ini_particion,int bm_inocarp,char *nombre_archivo);
int buscar_archivo_bapi(FILE *archivo,int ini_particion,char *nombre_archivo,int bm_bap,int nivel);
int insertar_info_archivo(FILE *archivo,int ini_particion,int bm_inodo,char *informacion);
int nuevo_apt_block_archivo(FILE *archivo,int ini_particion,int bm_ino_arch);
int nuevo_apt_ind_block_archivo(FILE *archivo,int ini_particion,int *new_bm_ba,int nivel);
int nuevo_ba_en_bapi(FILE *archivo,int ini_particion,int bm_bapi_padre,int nivel);
int buscar_apt_ino_carp_to_ino_arch(FILE *archivo,int ini_particion,int bm_inocarp,int bm_inoarch,char *name);
void listar_info_to_archivo(int size,char *path_contenido,int sin_path,char *str2);
//========================================================== CAT =======================
int mostrar_contenido_archivo(FILE *archivo,NODO_USR *usr_logeado,char *path_file);
void imp_block_archivo(FILE *archivo,int ini_particion,int bm_ino_arch);

//========================================================== MOVER =======================
int ejectuar_mv(FILE *archivo,NODO_USR *usr_logeado,char *path_origen,char *path_destino);
//bm  o -1
int buscar_elemento(FILE *archivo,int ini_particion,char *path_elem);
// retorna 1 o 0 ojo el destino tieen que ser un inodo tipo carpeta sino vale
int insert_in_apt_ino_carpeta(FILE *archivo,int ini_part,int bm_carp,int bm_nuevo,char *nombre_nuevo);
int get_last_name_path(char *path_elem,char *name_buscado);
//retorn 1 0
int delet_element(FILE *archivo,int ini_particion,char *path);
//retorn 1 - 0
int buscar_to_eliminar_carp(FILE *archivo,int ini_particion,int bm_padre,char *name_elem);

//========================================================== REM =======================
int ejecutar_rem(FILE *archivo,NODO_USR *usr_logeado,char *path);

//========================================================== PERMISOS =======================
int verificar_permisos(FILE *archivo,NODO_USR *usr_logeado,int bm_inodo,int tipo_permiso);



//======================================FUNCIONALIDADES DEL LOGIN =========================================
int eliminar_grupo_o_usuario(FILE *archivo,LISTA_USR *usuarios, int ini_particion,char *name,int tipo);

//======================================FUNCIONALIDADES DEL CHMOD =========================================
int ejecutar_chmod(FILE *archivo,NODO_USR *usr_logeado,int ugo,int r,char *path);
int cambiar_permiso_inodo(FILE *archivo,NODO_USR *usr_logeado,int ugo,int bm_inodo);

#endif // SISTEMA_ARCHIVOS_H_INCLUDED


