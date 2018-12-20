#include "analizador.h"
#include "admin_disco.h"


void mayus_cnv_minus_lst(char *lista)
{

    int k =0;

    while(lista[k] != NULL)
    {
        lista[k] = tolower(lista[k]);
        k++;
    }
}

void iniciar_analisis(char *lista,LISTA_MOUNT *const ptr_mount,NODO_USR *const usuario_logeado)
{
    int flag_mkdisk = 0;  //MKDISK
    int flag_exec =0;      //EXEC
    int flag_rmdisk =0;     //RMDISK
    int flag_fdisk =0;      //FDISK
    char flag_rep =0;       //REP
    char flag_mount =0;     //MOUNT
    char flag_unmount =0;   //UNMOUNT

    int flag_mkfs =0;   //MKFS -> formato completo
    int flag_login =0;
    int flag_logout =0;
    int flag_mkusr =0;
    int flag_mkgrp = 0;
    int flag_rmgrp = 0;
    int flag_rmusr = 0;
    int flag_mkdir = 0;
    int flag_p_mkdir = 0;

    int size = 0;
    char unit= 'k'; //en kilobytes por defecto
    char type= 'p'; //primaria por defecto
    int add =0;

    char *type_mkfs = (char*)malloc(sizeof(char)*5);
    memset(type_mkfs,0,sizeof(type_mkfs));

    char *fs = (char*)malloc(sizeof(char)*4);
    memset(fs,0,sizeof(fs));

    char *path = (char*)malloc(sizeof(char) * 250);
    memset(path,0,sizeof(path));

    char *del = (char*)malloc(sizeof(char)*5);
    memset(del,0,sizeof(del));

    char *name= (char*)malloc(sizeof(char)*50);
    memset(name,0,sizeof(name));

    char *id = (char*)malloc(sizeof(char) * 10);
    memset(id,0,sizeof(id));

    char *fit = (char*)malloc(sizeof(char)*3);
     memset(fit,0,sizeof(fit));

    //FASE2
    char *usr =(char*)malloc(sizeof(char)*10); //10 porque es el limite
    memset(usr,0,10);
    char *pwd = (char*)malloc(sizeof(char)*10); // 10 porque es el limite
    memset(pwd,0,10);
    char *grp = (char*)malloc(sizeof(char)*10);
    memset(grp,0,10);

/*=================== VARIABLES GENERALES ==================*/


    char *acum_comando = (char*)malloc(sizeof(char)*250);
    memset(acum_comando,0,sizeof(acum_comando));

    char *caracter = (char*)malloc(sizeof(char) * 1); //sirve de concatenador
    memset(caracter,0,sizeof(caracter));

    int index_lst = 0;

    int flag_error =0;
    int cont_sign_mnos_cmd = 0;
    int flag_size = 0;
    int flag_unit =0;
    int flag_path =0;
    int flag_type =0;
    int flag_fit = 0;
    int flag_delete = 0;
    int flag_name =0;
    int flag_add =0;
    int flag_id = 0;
    int flag_fs =0;

    int flag_sign_mayor = 0;
    int flag_cmlla_db = 0;
    //FASE2
    int flag_usr =0;
    int flag_pwd =0;
    int flag_grp =0;

    while((lista[index_lst] != NULL))    //:::::::::::::::::::::::::::::::::: INICIO DEL WHILE
   {
       if(lista[index_lst] == '#') // para los comentarios
       {
            while(lista[index_lst] != '\n'|| lista[index_lst] != NULL)
            {
                index_lst++;
                continue;        //repite el while apartir de aqui
            }
            if(lista[index_lst] == '\n' || lista[index_lst] == NULL) //para quitarlo de la lista
            {
                index_lst++;
                break;          //Salimos while
            }
       }

       if(lista[index_lst] == 32 || lista[index_lst] == '\n' || lista[index_lst] == '\r')  // \n -> delimitador ; 32 -> spacio en blanco ; return -> \r
       {
            if( strcasecmp(acum_comando,"mkdisk") == 0) /* strcasecmp compara cadenas pero lo hace sin case sensitive*/
            {
                flag_mkdisk = 1;    /* quitamos espacio en blanco con index_lst++ y activamos flag_mkdisk*/
                index_lst++;
                memset(acum_comando,0,sizeof(acum_comando));    /* reseteamos el acum_comando*/
            }
            else if(strcasecmp(acum_comando,"exec") == 0)
            {
                flag_exec = 1;
                index_lst++;
                memset(acum_comando,0,sizeof(acum_comando));
            }
            else if(strcasecmp(acum_comando,"rmdisk") == 0 )
            {
                flag_rmdisk = 1;
                index_lst++;
                memset(acum_comando,0,sizeof(acum_comando));
            }
            else if(strcasecmp(acum_comando,"fdisk") == 0)
            {
                flag_fdisk = 1;
                index_lst++;
                memset(acum_comando,0,sizeof(acum_comando));
            }
            else if(strcasecmp(acum_comando,"rep") == 0)
            {
                flag_rep = 1;
                index_lst++;
                memset(acum_comando,0,sizeof(acum_comando));
            }
            else if(strcasecmp(acum_comando,"mount") == 0)
            {
                flag_mount = 1;
                index_lst++;
                memset(acum_comando,0,sizeof(acum_comando));
            }
            else if(strcasecmp(acum_comando,"unmount") ==0)
            {
                flag_unmount = 1;
                index_lst++;
                memset(acum_comando,0,sizeof(acum_comando));
            }
            else if(strcasecmp(acum_comando,"mkfs")==0)
            {
                flag_mkfs =1;
                index_lst++;
                memset(acum_comando,0,sizeof(acum_comando));
            }
            else if(strcasecmp(acum_comando,"login")==0)
            {
                flag_login =1;
                index_lst++;
                memset(acum_comando,0,sizeof(acum_comando));
            }
            else if(strcasecmp(acum_comando,"logout")==0)
            {
                flag_logout =1;
                index_lst++;
                memset(acum_comando,0,sizeof(acum_comando));
                break; //porque ya no deberia de haber mas
            }
            else if(strcasecmp(acum_comando,"mkusr")==0)
            {
                flag_mkusr =1;
                index_lst++;
                memset(acum_comando,0,sizeof(acum_comando));
            }
            else if(strcasecmp(acum_comando,"mkgrp")==0)
            {
                flag_mkgrp =1;
                index_lst++;
                memset(acum_comando,0,sizeof(acum_comando));
            }
            else if(strcasecmp(acum_comando,"rmgrp")==0)
            {
                flag_rmgrp =1;
                index_lst++;
                memset(acum_comando,0,sizeof(acum_comando));
            }
            else if(strcasecmp(acum_comando,"rmusr")==0)
            {
                flag_rmusr =1;
                index_lst++;
                memset(acum_comando,0,sizeof(acum_comando));
            }
            else if(strcasecmp(acum_comando,"mkdir")==0)
            {
                flag_mkdir =1;
                index_lst++;
                memset(acum_comando,0,sizeof(acum_comando));
            }
            else if(strcasecmp(acum_comando,"p")==0)
            {
                flag_p_mkdir =1;
                index_lst++;
                memset(acum_comando,0,sizeof(acum_comando));
                cont_sign_mnos_cmd = 0;
            }
            else
            {
                if(flag_sign_mayor == 1) // ->  //agregar informacion a variables
                {
                    if(flag_size == 1)  //============================================== SIZE
                    {
                        size = atoi(acum_comando);
                        flag_size = 0 ;
                    }
                    else if(flag_unit == 1) //============================================== UNIT
                    {
                        if(strcasecmp(acum_comando,"m") == 0)
                            unit = 'm';
                        else if(strcasecmp(acum_comando,"k") == 0)
                            unit = 'k';
                        else if(strcasecmp(acum_comando,"b") == 0)
                            unit = 'b';
                        else
                            printf("Error en el unit ");

                         flag_unit =0;
                    }
                    else if(flag_type == 1) //============================================== TYPE
                    {
                        if(strcasecmp(acum_comando,"p") == 0)
                            type = 'p';
                        else if(strcasecmp(acum_comando,"e") == 0)
                            type = 'e';
                        else if(strcasecmp(acum_comando,"l") == 0)
                            type = 'l';
                        else if(strcasecmp(acum_comando,"fast") == 0)
                            strcpy(type_mkfs,"fast");
                        else if(strcasecmp(acum_comando,"full") == 0)
                        strcpy(type_mkfs,"full");
                        else
                        {
                            printf("ERROR: en Type de fdisk tiene que ser p(primaria) e(extendida) o l(logica)");
                            printf("\no full o fast en casod e de que se MKFS\n\n");
                        }
                         flag_type =0;
                    }
                    else if(flag_fit == 1) //============================================== FIT
                    {
                        if(strcasecmp(acum_comando,"bf") == 0)
                            strcpy(fit,"bf");
                        else if(strcasecmp(acum_comando,"ff") == 0)
                             strcpy(fit,"ff");
                        else if(strcasecmp(acum_comando,"wf") == 0)
                             strcpy(fit,"wf");
                        else
                            printf("ERROR: en Type tiene que ser bf(mejor ajuste) ff(primer ajuste) o wf(peor ajuste)");
                         flag_fit =0;
                    }
                    else if(flag_delete == 1) //============================================== DELETE
                    {
                        strcpy(del,acum_comando);
                        flag_delete = 0;
                    }
                    else if(flag_name == 1) //============================================== NAME
                    {
                        strcpy(name,acum_comando);
                        flag_name = 0;
                    }
                    else if(flag_add == 1) //============================================== ADD
                    {
                        add = atoi(acum_comando);
                        flag_add = 0;
                    }
                    else if(flag_id == 1)  //============================================== ID
                    {
                        strcpy(id,acum_comando);
                        flag_id = 0;
                    }
                    else if(flag_path == 1) //============================================== PATH (este path no contiene comillas)
                    {
                        strcpy(path,acum_comando);
                        flag_path = 0;
                    }
                    else if(flag_fs == 1) //============================================== FS
                    {
                        strcpy(fs,acum_comando);
                        flag_fs = 0;
                    }
                    else if(flag_usr == 1) //============================================== usr
                    {
                        strcpy(usr,acum_comando);
                        flag_usr = 0;
                    }
                    else if(flag_pwd == 1) //============================================== pwd
                    {
                        strcpy(pwd,acum_comando);
                        flag_pwd =0;
                    }
                    else if(flag_grp == 1) //============================================== grp
                    {
                        strcpy(grp,acum_comando);
                        flag_grp =0;
                    }
                    if((lista[index_lst] != '\n') && (lista[index_lst] != '\r')) //salto de linea o un return
                        index_lst++;

                    flag_sign_mayor = 0;        //la agregacion de informacion culmino
                    memset(acum_comando,0,sizeof(acum_comando));    //limpiamos
                }
                else if(flag_exec == 1)    //============================================== EXEC
                {
                    strcpy(path,acum_comando);
                    memset(acum_comando,0,sizeof(acum_comando));
                    index_lst++;

                    FILE *archivo;

                    if((archivo = fopen(path,"rb")))//lectura archivo binario
                    {
                        char linea[1024];
                        while(fgets(linea,1024,archivo)) //obtengo linea por linea con \n al final
                        {
                            if(linea[0] != '#')         //no tomamos en cuenta los comentarios
                                iniciar_analisis(linea,ptr_mount,usuario_logeado);
                        }
                    }
                    else
                        printf("ERROR: el archivo que Exec quiere ejecutar no Existe\n");
                    fclose(archivo);
                }
                else if(lista[index_lst] == 32) //solo espacio en blanco lo quitamos y continuamos
                    index_lst++;
            }
       }

       if(lista[index_lst] == '-') //signo menos NOTA: puede ser componente de la flecha o un numero negativo
       {
            if(flag_sign_mayor == 1) //esta activado la flecha para asignar valores
            {
                if(flag_add == 1)
                {
                    caracter[0] = '-';
                    strcat(acum_comando,caracter); //concatenamos el signo menos
                }
                else{
                    printf("ERROR: hay problemas con un signo negativo, ya que se esperaba que perteneciera a un add \n\n");
                }
            }
            else if(cont_sign_mnos_cmd == 0) //el objetivo -size- entonces inicia - tambien puede venir un p
            {
                cont_sign_mnos_cmd++;
            }
            else if(cont_sign_mnos_cmd == 1) //ya hay otro - entonces cierra -size-
            {
                if( strcasecmp(acum_comando,"size") == 0)
                     flag_size = 1;
                else if(strcasecmp(acum_comando,"unit") == 0 )
                     flag_unit = 1;
                else if(strcasecmp(acum_comando,"path") == 0 )
                     flag_path = 1;
                else if(strcasecmp(acum_comando,"type") == 0 )
                     flag_type = 1;
                else if(strcasecmp(acum_comando,"fit") == 0 )
                     flag_fit = 1;
                else if(strcasecmp(acum_comando,"delete") == 0 )
                     flag_delete = 1;
                else if(strcasecmp(acum_comando,"name") == 0 )
                     flag_name = 1;
                else if(strcasecmp(acum_comando,"add") == 0 )
                     flag_add = 1;
                else if(strcasecmp(acum_comando,"id") ==0)
                     flag_id = 1;
                else if(strcasecmp(acum_comando,"fs") == 0)
                     flag_fs = 1;
                else if(strcasecmp(acum_comando,"usr") == 0)
                     flag_usr = 1;
                else if(strcasecmp(acum_comando,"pwd") == 0)
                     flag_pwd = 1;
                else if(strcasecmp(acum_comando,"grp") == 0)
                     flag_grp = 1;
                else
                {
                    printf("ERROR: hubo problemas con el parametro: ");
                    printf("%s\n\n",acum_comando);
                    flag_error = 1;
                    break; //salimos del while concatenador
                }

                cont_sign_mnos_cmd =0;
                memset(acum_comando,0,sizeof(acum_comando));
            }
            else if(cont_sign_mnos_cmd > 1)
            {
                printf("ERROR con Signo negativo ya que hay mas de los que deberian de haber\n\n");
                flag_error  = 1;
                break; //salimos del while
            }

            index_lst++; //quitamos el signo " - " analizado
       }

       if(lista[index_lst] == '>')  //Flecha que determina los valores para las variables
       {
            flag_sign_mayor = 1;
            index_lst++;
            memset(acum_comando,0,sizeof(acum_comando)); //limpiamos
       }

       if(lista[index_lst] == 34) //============================================== PATH (este path SI contiene comillas dibles)
       {
            if(flag_path == 1 && flag_sign_mayor == 1 )
           {
                caracter[0] = lista[index_lst];
                strcat(acum_comando,caracter); //concatenamos la comilla doble
                index_lst++;

                while(lista[index_lst] != 34 ) //concatenar hasta que encuentre comillas doble
                {
                    if(lista[index_lst] == NULL) //por las moscas si llega al final
                        break;
                    caracter[0] = lista[index_lst];
                    strcat(acum_comando,caracter);
                    index_lst++;
                }
                if(lista[index_lst] == 34)
                {
                    caracter[0] = lista[index_lst]; //agregamos la comilla doble que cierra la cadena del path
                    strcat(acum_comando,caracter);
                    strcpy(path,acum_comando);
                    memset(acum_comando,0,sizeof(acum_comando));

                    flag_sign_mayor = 0;
                    flag_path = 0;
                    index_lst++; //quitamos las comillas dobles del analisis
                }
            }
            else if(flag_sign_mayor == 1 ) // > quitamos las comillas dobles
            {
                memset(acum_comando,0,sizeof(acum_comando)); //limpiamos
                index_lst++;
                while(lista[index_lst] != 34)
                {
                    if(lista[index_lst] == NULL) //por las moscas si llega al final
                    {
                        flag_error = 1;
                        break;
                    }
                    caracter[0] = lista[index_lst];
                    strcat(acum_comando,caracter);
                    index_lst++;
                }
                if(lista[index_lst] == 34)
                {   //:::::::::::::::::::::::::::agregamos a parametros con comillas dobles
                    flag_sign_mayor = 0;
                    if(flag_usr == 1)
                    {
                        strcpy(usr,acum_comando);
                        flag_usr = 0;
                    }
                    else if(flag_pwd == 1)
                    {
                        strcpy(pwd,acum_comando);
                        flag_pwd = 0;
                    }
                    else if(flag_name == 1)
                    {
                        strcpy(name,acum_comando);
                        flag_name == 0;
                    }
                    memset(acum_comando,0,sizeof(acum_comando));
                    index_lst++; //quitamos las comillas dobles del analisis
                }
            }
            else
            {
                printf("Error: viene una comilla doble sin utilizarse\n\n");
            }
       }

       if(lista[index_lst] == '#') // para los comentarios
       {
            while((lista[index_lst] != '\n') && (lista[index_lst] != NULL))
            {
                index_lst++;
                continue;
            }
            if((lista[index_lst] == '\n') && (lista[index_lst] != NULL))
            {
                index_lst++;
                break;
            }
       }
        //::::Concatenacion::::::
        if((lista[index_lst] != NULL) && (lista[index_lst] != 10) && (lista[index_lst] != '\r') && (lista[index_lst] != 32)) // 10 -> salto de linea ; No concatenamos lo que hay en la cond
        {
            caracter[0] = lista[index_lst];
            strcat(acum_comando,caracter);
        }

        index_lst++;
    }//::::::::::::::::::::::::::::::::::::::::::::::::::::::: FIN DEL WHILE

    if(flag_error !=1)
    {
        if(flag_mkdisk == 1)//==================== MKDISK
        {
           if(strcmp(fit,"") == 0)
           {
                memset(fit,0,sizeof(fit));
                strcpy(fit,"ff");
           }
           crearDisco(size,unit,path,fit);
        }
        else if(flag_rmdisk == 1)//===============RMDISK
        {
            eliminar_disco(path);
        }
        else if(flag_fdisk == 1)//===============FDISK
        {
            if(size > 0 && strcmp(path,"") != 0 && strcmp(name,"") != 0 )
            {
                if(add != 0) //hay una agregacion
                {
                    printf("El comando ADD no esta definido\n\n");
                }
                else //debe ser una creacion
                {
                    if(strcmp(fit,"") == 0)
                    {
                        memset(fit,0,sizeof(fit));
                        strcpy(fit,"wf");   //peor ajuste por defecto
                    }
                    crear_particion(size,unit,type,fit,path,name);
                    memset(fit,0,sizeof(fit));
                }
            }
            else if(strcmp(path,"") != 0 && strcmp(name,"") != 0 )
            {
                if(strcmp(del,"") != 0) //HAY UN DELETE
                {
                    delete_particion(del,name,path);
                }
                else
                    printf("ERROR: no se pudo eliminar la particion porque delete no esta activa\n\n");
            }
            else
            {
                printf("ERROR: para ejecutar fdisk se necesita valores de size, el path y el name \n\n");
            }
        }
        else if(flag_mount == 1)//===============MOUNT
        {
            if(strcmp(path,"") != 0 && strcmp(name,"") != 0)
            {
                montar_disco(path,name,ptr_mount);
            }
            else
            {
                NODO_MOUNT *tmp = ptr_mount->primero;

                while (tmp != NULL)
                {
                    printf("- %s\n",tmp->id_mount);
                    tmp = tmp->siguiente;
                }

            }
        }
        else if(flag_unmount == 1) //===============UNMOUNT
        {
            if(strcmp(id,"") != 0)
            {
                int confirm = desmontar_particion(id,ptr_mount);

                if(confirm == 1)
                    printf("Se desmonto la particion: %s\n\n",id);
                else
                    printf("ERROR: no se encontro la particion que desea desmontar con id:  %s\n\n",id);
            }
        }
        else if(flag_rep == 1) //=============== REP
        {
           char *path_disco = get_path_mount(id,ptr_mount);
            if(path_disco!=NULL)
            {
                if(strcasecmp(name,"mbr") ==0)
                    reporte_mbr(path,path_disco,id);
                else if(strcasecmp(name,"disk") == 0 )
                {
                    reporte_disk(path,path_disco,id);
                }
            }
            else
            {
                printf("ERROR:el id que desea generar reporte no se encontro\n\n");
            }
        }
        else if(flag_exec == 1)    //============================================== EXEC
        {
            FILE *archivo;

            if((archivo = fopen(path,"rb")))//lectura archivo binario
            {
                char linea[1024];
                while(fgets(linea,1024,archivo)) //obtengo linea por linea con \n al final
                {
                    if(linea[0] != '#')         //no tomamos en cuenta los comentarios
                        iniciar_analisis(linea,ptr_mount,usuario_logeado);
                }
            }
            else
                printf("ERROR: el archivo que Exec quiere ejecutar no Existe\n");
            fclose(archivo);
        }
        else if(flag_mkfs == 1) //============================================== mkfs
        {
            if(strcmp(id,"") != 0)
            {
                NODO_MOUNT *mount_particion = get_nodo_mount(id,ptr_mount);
                if(mount_particion != NULL)
                {
                    FILE *archivo =fopen(mount_particion->path_mount,"r+b");

                    if(archivo != NULL)
                    {
                        if(strcmp(type_mkfs,"") == 0)
                        {
                            memset(type_mkfs,0,sizeof(type_mkfs));
                            strcpy(type_mkfs,"full");
                        }
                        int inicio_particion =0;
                        PTR particion = buscar_particion(archivo,mount_particion->name);

                        if(particion.part_status == '1')
                        {
                            //crear_ext3 automaticamente realiza un Fast con los bitmap
                            if(crear_ext3(archivo,particion.part_size,particion.part_start,particion.part_name) == 1)
                             {
                                 if(strcasecmp(type_mkfs,"full") == 0)
                                     full_particion(archivo,particion.part_start,particion.part_size);
                                 else if(strcmp(type_mkfs,"") == 0)
                                    full_particion(archivo,particion.part_start,particion.part_size);
                                 crear_archivo_users(archivo,particion.part_size,particion.part_start,particion.part_name);
                                 printf("Se ha creado un sistema de archivos EXT3 en la particion: %s\n\n",mount_particion->name);
                             }
                        }
                        else
                        {
                            printf("ERROR:hay problemas con encontrar la particion con nombre: %s\n\n",mount_particion->name);
                        }
                    }
                    else
                    {
                     printf("ERROR: No se puede abrir el archivo de la particion:  ");
                     printf("%s\n\n",id);
                    }
                    fclose(archivo);
                }
                else
                {
                     printf("ERROR: no se encunetra montada la articion:  ");
                     printf("%s\n\n",id);
                }
            }
            else
            {
                 printf("ERROR: Para ejecutar MKFS es necesario tener un ID\n");
            }
        }
        else if(flag_login == 1) //========================================================= login
        {
            if(usuario_logeado->id != 0)
            {
                 printf("ERROR: No se puede Iniciar sesion porque hay una cuenta activa \n");
                 printf("La cuenta activa es: %s\n\n",usuario_logeado->nombre_usr);
            }
             else if(strcmp(usr,"") != 0 && strcmp(pwd,"")!=0 && strcmp(id,"")!=0)
            {
                LISTA_USR *lst_usr = (LISTA_USR*)malloc(sizeof(LISTA_USR));
                inicializar_lst_usr(lst_usr);
                 NODO_MOUNT *mount_particion = get_nodo_mount(id,ptr_mount);
                 if(mount_particion!=NULL)
                 {
                     FILE *archivo =fopen(mount_particion->path_mount,"r+b");
                     if(archivo !=NULL)
                     {
                         PTR particion = buscar_particion(archivo,mount_particion->name);
                         if(particion.part_status == '1')
                         {
                            consultar_usuarios(archivo,particion.part_start,particion.part_size,mount_particion->path_mount,lst_usr);
                            if(lst_usr->size !=0)
                            {
                                NODO_USR *usr_nodo = usuario_login(lst_usr,usr,pwd);
                                if(usr_nodo != NULL)
                                {
                                   // usuario_logeado =  (NODO_USR*)malloc(sizeof(NODO_USR));
                                    usuario_logeado->id  = usr_nodo->id;
                                    usuario_logeado->tipo  = usr_nodo->tipo;
                                    usuario_logeado->inicio_particion = usr_nodo->inicio_particion;
                                    usuario_logeado->size_particion  = usr_nodo->size_particion;
                                    strcpy(usuario_logeado->nombre_grupo,usr_nodo->nombre_grupo);
                                    strcpy(usuario_logeado->nombre_usr,usr_nodo->nombre_usr);
                                    strcpy(usuario_logeado->password_usr,usr_nodo->password_usr);
                                    strcpy(usuario_logeado->path_particion,usr_nodo->path_particion);

                                    printf("Exito se ha logeado con el usuario: %s!!\n",usr);
                                }
                                else
                                {
                                    printf("ERROR: No se encuntran registrado el usuario: %s con password: %s ",usr,pwd);
                                }
                            }
                            else
                                printf("ERROR: En el archivo de Usuarios no hay informacion ");
                         }
                         else
                         {
                            printf("ERROR:hay problemas con encontrar la particion con nombre: %s en login \n\n",mount_particion->name);
                         }

                     }
                     else
                    {
                     printf("ERROR: No se puede abrir el archivo de la particion:  ");
                     printf("%s\n\n",id);
                    }
                    fclose(archivo);
                 }
                 else
                {
                     printf("ERROR: no se encunetra montada la articion:  ");
                     printf("%s\n\n",id);
                }
                free(lst_usr);
                lst_usr =NULL;
            }
            else
                printf("ERROR: Para ejecutar LOGIN es necesario tener un ID , un password y un usuario\n");
        }
        else if(flag_logout == 1) //========================================================= LOGOUT
        {
            if(usuario_logeado->id != 0)
            {
                usuario_logeado->id =0;
                printf("Se ha cerrado la sesion con nombre %s\n\n",usuario_logeado->nombre_usr);
            }
            else
                printf("ERROR: no se puede aplicar logout porque no hay sesion activa\n\n");
        }
        else if(flag_mkusr == 1) //========================================================= MKUSR
        {
            if(strcasecmp(usuario_logeado->nombre_usr,"root") !=0)
            {
                printf("ERROR: No se puede hacer uso de mkusr porque se necesita una sesion activa como root\n\n");
            }
            else if(strcmp(usr,"") != 0 && strcmp(pwd,"")!=0 && strcmp(grp,"")!=0)
            {
                if(strlen(usr) < 11 && strlen(pwd) < 11)
                {
                    //verificando si existe el nuevo usuario a registrar
                    FILE *archivo = fopen(usuario_logeado->path_particion,"r+b");
                    if(archivo!=NULL)
                    {
                        LISTA_USR *lst_usr = (LISTA_USR*)malloc(sizeof(LISTA_USR));
                        inicializar_lst_usr(lst_usr);
                        consultar_usuarios(archivo,usuario_logeado->inicio_particion,usuario_logeado->size_particion,usuario_logeado->path_particion,lst_usr);
                        if(lst_usr->size !=0)
                        {
                            int verif = verif_usr_rep_grp(lst_usr,usr,grp);
                            if(verif == 0)
                            {
                                int verif_grp = verificar_grp_existe(lst_usr,grp);
                                if(verif_grp == 1)
                                {
                                    int result = registrar_en_archivo(archivo,usuario_logeado->inicio_particion,usr,grp,pwd,'U');
                                    if(result !=0)
                                    {
                                        printf("EXITO el usuario %s fue registrado!! en el grupo %s \n",usr,grp);
                                    }
                                    else
                                        printf("Error: no se pudo registrar el usuario por problemas en la actualizacion del archivo\n\n");
                                }
                                else
                                     printf("Error: el grupo: %s no existe en la particion\n",grp);
                            }
                            else
                                printf("Error: el usuario: %s ya pertenece al grupo: %s\n",usr,grp);
                        }
                        else
                            printf("Error: no se pudo obtener lista de usuarios en MKUSR\n");
                        fclose(archivo);
                    }
                    else
                        printf("ERROR: no se puede acceder al path: %s en el estado de MKUSR\n\n",usuario_logeado->path_particion);
                }
                else
                    printf("ERROR: no se puede crear nombres de usuarios o passwords con mas de 10 caracteres\n");

            }
            else
                 printf("ERROR: Para ejecutar MKUSR es necesario tener un grp , un password y un usuario\n");
        }
        else if(flag_mkgrp == 1) //========================================================= MKGRP
        {
            if(strcasecmp(usuario_logeado->nombre_usr,"root") == 0)
            {
                if(strcmp(name,"") != 0)
                {
                    if(strlen(name) < 11)
                    {
                        FILE *archivo = fopen(usuario_logeado->path_particion,"r+b");
                        if(archivo!=NULL)
                        {
                            LISTA_USR *lst_usr = (LISTA_USR*)malloc(sizeof(LISTA_USR));
                            inicializar_lst_usr(lst_usr);
                            consultar_usuarios(archivo,usuario_logeado->inicio_particion,usuario_logeado->size_particion,usuario_logeado->path_particion,lst_usr);
                            if(lst_usr->size !=0)
                            {
                                int verif = verificar_grp_existe(lst_usr,name);
                                if(verif != 1)
                                {
                                    int result = registrar_en_archivo(archivo,usuario_logeado->inicio_particion,"",name,"",'G');
                                    if(result !=0)
                                    {
                                        printf("EXITO el grupo %s fue registrado!!\n",name);
                                    }
                                    else
                                        printf("Error: no se pudo registrar el grupo por problemas en la actualizacion del archivo\n\n");
                                }
                                else
                                    printf("Error: no se puede crear el grupo:  %s  por que ya existe en la particion\n",name);
                            }
                            else
                                printf("Error: no se pudo obtener lista de grupos en MKGRP\n");
                            fclose(archivo);
                        }
                        else
                            printf("ERROR: no se puede acceder al path: %s en el estado de MKGRP\n\n",usuario_logeado->path_particion);
                    }
                    else
                        printf("ERROR: el name: %s posee mas de 10 caracteres\n",name);
                }
                else
                    printf("ERROR: Para ejecutar MKGRP es necesario tener name\n");
            }
            else
                printf("ERROR: No se puede hacer uso de mkgrp porque se necesita una sesion activa como root\n");

        }
        else if(flag_rmgrp == 1) //========================================================= RMGRP
        {
            if(strcasecmp(usuario_logeado->nombre_usr,"root") == 0)
            {
                 if(strcmp(name,"") != 0)
                {
                    printf("ADVERTENCIA: EL COMANDO RMGRP NO HA SIDO CONFIGURADO\n");
                }
                 else
                    printf("ERROR: Para ejecutar RMGRP es necesario tener name\n");
            }
            else
                printf("ERROR: No se puede hacer uso de RMGRP porque se necesita una sesion activa como root\n");
        }
        else if(flag_rmusr == 1) //========================================================= RMUSR
        {
            if(strcasecmp(usuario_logeado->nombre_usr,"root") == 0)
            {
                 if(strcmp(usr,"") != 0)
                {
                    printf("ADVERTENCIA: EL COMANDO RMGRP NO HA SIDO CONFIGURADO\n");
                }
                 else
                    printf("ERROR: Para ejecutar RMUSR es necesario tener usr\n");
            }
            else
                printf("ERROR: No se puede hacer uso de RMUSR porque se necesita una sesion activa como root\n");
        }
        else if(flag_p_mkdir == 1) //========================================================= MKDIR
        {
            if(strcmp(path,"") !=0)
            {
                FILE *archivo = fopen(usuario_logeado->path_particion,"r+b");
                if(archivo!=NULL)
                {
                    if(flag_p_mkdir == 1)
                    {
                        
                        ejecutar_mkdir(archivo,usuario_logeado->inicio_particion,path,1);
                    }
                    fclose(archivo);
                }
                else
                    printf("ERROR: no se puede acceder al path: %s en el estado de MKDIR\n\n",usuario_logeado->path_particion);
            }
            else
                printf("ERROR: No se ejecutar mkdir porque hace falta path\n");
        }
    }

    /*SE LIBERA LA MEMORIA DE LOS REGISTROS*/
    free(path);
    path = NULL;
    free(fit);
    fit =NULL;
    free(acum_comando);
    acum_comando =NULL;
    free(del);
    del = NULL;
    free(name);
    name=NULL;
    free(id);
    id = NULL;
    free(caracter);
    caracter=NULL;
    free(usr);
    usr = NULL;
    free(pwd);
    pwd = NULL;
    //reseteando flag_error
    flag_error = 0;


}


