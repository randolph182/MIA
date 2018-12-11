
#include "reporte_grafico.h"

int cvl_path_carpeta(char *path_archivo,char **archivo_dot,char **extension)
{
    int index_path = 0;
    char tmp_path[100];
    int error = 0;

    char *carpeta_actual = (char*)malloc(sizeof(char) * 100);
    memset(carpeta_actual,0,sizeof(carpeta_actual));

    char *caracter = (char*)malloc(sizeof(char) * 1); //sirve de concatenador
    memset(caracter,0,sizeof(caracter));

    int flag_extension = 0;

    if(path_archivo[0] == 34) //si posee  comillas dobles //=================================================== Con comillas
    {
        char *carpeta_funcional = (char*)malloc(sizeof(char) * 1024);
        memset(carpeta_funcional,0,sizeof(carpeta_funcional));

        index_path = 1;  //quitamos las comillas dobles
        int salir = 0;

        while(salir != 1) //quitamos todos los espacios antes de la diagonal
        {
            if(path_archivo[index_path] == '/') { salir = 1; break; } //salimos del bucle while

            else if(path_archivo[index_path] == NULL)
            {
                printf("ERROR: analisis critico en path con comillas dobles para sacar reporte \n\n");
                return 0;
            }
            index_path++;
        }

        caracter[0] = path_archivo[index_path];
        strcat(carpeta_actual,caracter);
        strcat(carpeta_funcional,caracter);
        index_path++;

        while(path_archivo[index_path] != 34) //hasta que encontremos otras comillas dobles
        {
            if(path_archivo[index_path] == '/')
            {
                if(verif_carpeta(carpeta_actual) == 0) // si no existe la carpeta
                {
                    if(crear_carpeta(carpeta_funcional) == 1 ) //creamos la carpeta y retorna un booleano
                        printf("Se ha creado la carpeta para reporte con  path:  %s\n",carpeta_actual);
                    else
                        return 0;
                }
            }
            else if(path_archivo[index_path] == 32) //espacio en blanco
            {
                caracter[0] = 92; //caracter ascii -> "\"
                strcat(carpeta_funcional,caracter); //se le agrega a carpeta funcional porque tiene caracteres especiales para crear la carpeta
            }
            else if(path_archivo[index_path] == '.') //si nos topamos con un punto lo cual seria ya el archivo con extension
            {
                strcpy(*archivo_dot,carpeta_actual); //hacemos copia de la direccion hasta antes del punto
                strcat(*archivo_dot,".dot");
                flag_extension = 1;
            }

            caracter[0] = path_archivo[index_path];
            if(flag_extension == 1)
            {
                if(path_archivo[index_path] != '.')
                    strcat(*extension,caracter);
            }
            strcat(carpeta_actual,caracter);
            strcat(carpeta_funcional,caracter);
            index_path++;
      }
        if(path_archivo[index_path]== 34) //comillas dobles
        {
            //char *path_disco = get_path_mount()
            //reporte_mbr_(archivo_dot,path_disco,path_archivo,id,extension);
                    //liberando la memoria dinamica
            free(carpeta_funcional);
            carpeta_funcional = NULL;
            free(carpeta_actual);
            carpeta_actual =NULL;
            free(caracter);
            caracter =NULL;
            return 1;
        }
        else{
            free(carpeta_funcional);
            carpeta_funcional = NULL;
            free(carpeta_actual);
            carpeta_actual =NULL;
            free(caracter);
            caracter =NULL;
            return 1;
        }
    }
    else if(path_archivo[0] == '/') //=================================================== sin comillas
    {
        caracter[0] = path_archivo[0];
        carpeta_actual[0] = path_archivo[0];
        index_path =1;

        while(path_archivo[index_path] != NULL)
        {
            if(path_archivo[index_path] == '/')
            {

                if(verif_carpeta(carpeta_actual) == 0) // si no existe la carpeta
                {
                    if(crear_carpeta(carpeta_actual) == 1 ) //creamos la carpeta y retorna un booleano
                        printf("Se ha creado la carpeta para reporte con  path:  %s\n",carpeta_actual);
                    else
                        return 0;
                }
            }
            else if(path_archivo[index_path] == 32) //no deben haber espacios en blanco dentro de este tipo de path
            {
                printf("ERROR: no pueden haber Espacios en blanco en el path, si los desea declarelos dentro de comillas dobles\n\n");
                return 0;
            }
            else if(path_archivo[index_path] == '.')
            {
                strcpy(*archivo_dot,carpeta_actual); //hacemos copia de la direccion hasta antes del punto
                strcat(*archivo_dot,".dot");
                flag_extension = 1;
            }

            caracter[0] = path_archivo[index_path];
            if(flag_extension == 1)
            {
                if(path_archivo[index_path] != '.')
                    strcat(*extension,caracter);
            }
            strcat(carpeta_actual,caracter);
            index_path++;
        }

        if(path_archivo[index_path]== NULL)
        {
            free(carpeta_actual);
            carpeta_actual =NULL;
            free(caracter);
            caracter =NULL;
            return 1;
        }
        else
        {
            free(carpeta_actual);
            carpeta_actual =NULL;
            free(caracter);
            caracter =NULL;
            return 0;
        }


    }

}


void reporte_mbr(char *path_archivo,char *path_disco,char *id)
{
    char *archivo_dot  = (char*)malloc(sizeof(char) * 100);
    memset(archivo_dot,0,sizeof(archivo_dot));
    char *extension = (char*)malloc(sizeof(char)*3);
    memset(extension,0,sizeof(extension));

    if(cvl_path_carpeta(path_archivo,&archivo_dot,&extension) == 1)
    {
        reporte_mbr_(archivo_dot,path_disco,path_archivo,id,extension);
    }
    else
        printf("ERROR: Algo salio mal en la creacion de la carpeta con path: %s\n\n",path_archivo);
    free(archivo_dot);
    archivo_dot = NULL;
    free(extension);
    extension = NULL;
}

void reporte_mbr_(char *path_archivo,char *path_disco,char *path_archivo_destino,char *id, char *extension)
{
    FILE *tmp = fopen(path_disco,"rb");
    fseek(tmp,0,SEEK_SET);
    MBR mbr_tmp[1];
    fread(mbr_tmp,sizeof(MBR),1,tmp);

    FILE *archivo;
    archivo = fopen(path_archivo,"w+");

    fprintf(archivo,"digraph mbr{\n");
    fprintf(archivo,"graph [ratio=fill];\n");
    fprintf(archivo,"node [label=\"\\N\", fontsize=15, shape=plaintext];\n");
    fprintf(archivo,"graph [bb=\"0,0,352,154\"];\n");
    fprintf(archivo,"arset [label=<\n");
    fprintf(archivo,"<TABLE ALIGN=\"LEFT\">\n");
    fprintf(archivo,"<TR>");
    fprintf(archivo,"<TD> <B> NOMBRE </B> </TD> ");
    fprintf(archivo,"<TD> <B> VALOR </B> </TD>");
    fprintf(archivo,"</TR>\n");

    //calculo de tamanio de disk
    char tamanio [50];
    sprintf(tamanio,"%d",mbr_tmp[0].mbr_tamanio_disk);
    strcat(&tamanio," bytes");
    concat_2_val(archivo,"tamanio_disk",tamanio,3);

    //concatenando fecha de creacion
    concat_2_val(archivo,"fecha de creacion",mbr_tmp[0].mbr_fecha_creacion,2);

    //concatenando el Id de disco
    char id_disco[50];
    sprintf(id_disco,"%d", mbr_tmp[0].mbr_disk_segnature);
    concat_2_val(archivo,"id asignado a disco = ",id_disco,3);

    //concatenando el fit del disco
    concat_2_val(archivo,"mbr_ajuste = ",mbr_tmp[0].disk_fit,2);

    PTR particion[4];
    particion[0] = (mbr_tmp[0]).mbr_partition_1;
    particion[1] = (mbr_tmp[0]).mbr_partition_2;
    particion[2] = (mbr_tmp[0]).mbr_partition_3;
    particion[3] = (mbr_tmp[0]).mbr_partition_4;

    ordenar_particiones(particion);
    int itera = 0;

    //concatenando las particiones del mbr
    acum_ptr_mbr(archivo,particion,itera);

    fprintf(archivo,"</TABLE>\n");
    fprintf(archivo,">, ];\n");
    fprintf(archivo,"\n}");

   //printf("%s\n",acum);
   //FILE *archivo;
   //archivo = fopen(path_archivo,"w+");
   //fputs(acum,archivo);
   fclose(archivo);

   char *comando = (char*)malloc(sizeof(char)*100);
   memset(comando,0,sizeof(comando));
   strcat(comando,"dot ");
   strcat(comando,path_archivo);
   strcat(comando," -o ");
   strcat(comando,path_archivo_destino);

   if(strcasecmp(extension,"png") ==0)
   {
    strcat(comando," -Tpng");
   }
   else if(strcasecmp(extension,"jpg") ==0)
   {
    strcat(comando," -Tjpg");
   }
   else if(strcasecmp(extension,"pdf") ==0)
   {
    strcat(comando," -Tpdf");
   }

  // printf("%s\n",comando);
   int flag = system(comando);
     printf("Reporte Generado!\n\n");
}


void acum_ptr_mbr(FILE *archivo,PTR particion[],int itera)
{
      if(itera < 4)
      {
          if(particion[itera].part_status == '1')
          {
              concat_2_val(archivo,"part_status",particion[itera].part_status,1);
              concat_2_val(archivo,"part_type",particion[itera].part_type,1);
              concat_2_val(archivo,"part_fit",particion[itera].part_fit,2);

              char part_start[50];
              sprintf(part_start,"%d",particion[itera].part_start);// + '0';
              concat_2_val(archivo,"part_start",part_start,3);

              char part_size[50];
              sprintf(part_size,"%d", particion[itera].part_size);// + '0';
              strcat(&part_size," bytes ");
              concat_2_val(archivo,"part_size",part_size,3);

              concat_2_val(archivo,"part_name",particion[itera].part_name,2);
          }
          itera++;
          acum_ptr_mbr(archivo,particion,itera);
      }
}

void concat_2_val(FILE *archivo,char info1[],char *info2,int tipo)
{
    fprintf(archivo,"<TR>");
    fprintf(archivo,"<TD> ");
    fprintf(archivo,info1);
    fprintf(archivo," </TD>");
    fprintf(archivo,"<TD> <TABLE BORDER=\"0\"> <TR> <TD>");

    if(tipo == 1)// caracteres
        fprintf(archivo,"%c",info2);
    else if(tipo == 2) //cadenas
        fprintf(archivo,"%s",info2);
    else if(tipo == 3) //digitos
        fprintf(archivo,info2);

    fprintf(archivo,"</TD> </TR> </TABLE> </TD>");
    fprintf(archivo,"</TR>\n");
}

void reporte_disk(char *path_archivo,char *path_disco,char *id)
{

    char *archivo_dot  = (char*)malloc(sizeof(char) * 100);
    memset(archivo_dot,0,sizeof(archivo_dot));
    char *extension = (char*)malloc(sizeof(char)*3);
    memset(extension,0,sizeof(extension));

    if(cvl_path_carpeta(path_archivo,&archivo_dot,&extension) == 1)
    {
        reporte_disk_(archivo_dot,path_disco,path_archivo,id,extension);
    }
    else
        printf("ERROR: Algo salio mal en la creacion de la carpeta con path: %s\n\n",path_archivo);
    free(archivo_dot);
    archivo_dot = NULL;
    free(extension);
    extension = NULL;
}

void reporte_disk_(char *path_archivo,char *path_disco, char *path_archivo_destino,char *id,char *extension)
{
    FILE *tmp = fopen(path_disco,"rb");
    fseek(tmp,0,SEEK_SET);
    MBR mbr[1];
    fread(mbr,sizeof(MBR),1,tmp);
    PTR particion[4];
    particion[0] = (mbr[0]).mbr_partition_1;
    particion[1] = (mbr[0]).mbr_partition_2;
    particion[2] = (mbr[0]).mbr_partition_3;
    particion[3] = (mbr[0]).mbr_partition_4;
    fclose(tmp);

    FILE *archivo;
    archivo = fopen(path_archivo,"w+");

    fprintf(archivo,"digraph structs{\n");
    fprintf(archivo,"node [shape=plaintext]\n");
    fprintf(archivo,"struct1 [label=<\n");
    fprintf(archivo,"<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\"> \n");
    fprintf(archivo,"<TR>\n");

    fprintf(archivo,"<TD ROWSPAN=\"3\"> MBR <BR/>");
    char part_size[50];
    sprintf(part_size,"%d",sizeof(mbr));
    fprintf(archivo,part_size);
    fprintf(archivo,"<BR/>");
    int porc = sacar_porcentaje(mbr->mbr_tamanio_disk,sizeof(mbr));
    char porcent[50];
    sprintf(porcent,"%d",porc);
    fprintf(archivo,"%%");
    fprintf(archivo,porcent);
    fprintf(archivo," </TD>\n");

    ordenar_particiones(particion);
    int itera = 0;
    int acum = sizeof(mbr);
    //concatenando las particiones del disk
    acum_ptr_disk(archivo,particion,itera,mbr->mbr_tamanio_disk,acum);
    fprintf(archivo,"</TR>\n");

    fprintf(archivo,"</TABLE>>];\n");
    fprintf(archivo,"}\n");

    fclose(archivo);

    char *comando = (char*)malloc(sizeof(char)*100);
    memset(comando,0,sizeof(comando));
    strcat(comando,"dot ");
    strcat(comando,path_archivo);
    strcat(comando," -o ");
    strcat(comando,path_archivo_destino);

    if(strcasecmp(extension,"png") ==0)
    {
        strcat(comando," -Tpng");
    }
        else if(strcasecmp(extension,"jpg") ==0)
    {
        strcat(comando," -Tjpg");
    }
    else if(strcasecmp(extension,"pdf") ==0)
    {
        strcat(comando," -Tpdf");
    }
   int flag = system(comando);
   printf("Reporte Generado!\n\n");

}

void acum_ptr_disk(FILE *archivo,PTR particion[],int itera,int t_dsk,int acumulador)
{
    if(itera < 4)
    {
        if(particion[itera].part_status == '1')
        {
            if(particion[itera].part_type == 'p')
            {
                fprintf(archivo,"<TD ROWSPAN=\"3\"> Primaria <BR/>");
                fprintf(archivo,"%s",particion[itera].part_name);
                fprintf(archivo," <BR/>");
                char part_start[50];
                sprintf(part_start,"%d",particion[itera].part_start);
                fprintf(archivo,part_start);
                fprintf(archivo," <BR/>");
                char part_size[50];
                sprintf(part_size,"%d",particion[itera].part_size);
                fprintf(archivo,part_size);
                fprintf(archivo,"<BR/>");
                int porc = sacar_porcentaje(t_dsk,particion[itera].part_size);
                acumulador += particion[itera].part_size;
                char porcent[50];
                sprintf(porcent,"%d",porc);
                fprintf(archivo,"%%");
                fprintf(archivo,porcent);
                fprintf(archivo," </TD>\n");
            }
            else if(particion[itera].part_type == 'e')
            {
                fprintf(archivo,"<TD ROWSPAN=\"3\"> Extendida <BR/>");
                fprintf(archivo,"%s",particion[itera].part_name);
                fprintf(archivo," <BR/>");
                char part_start[50];
                sprintf(part_start,"%d",particion[itera].part_start);
                fprintf(archivo,part_start);
                fprintf(archivo," <BR/>");
                char part_size[50];
                sprintf(part_size,"%d",particion[itera].part_size);
                fprintf(archivo,part_size);
                fprintf(archivo,"<BR/>");
                int porc = sacar_porcentaje(t_dsk,particion[itera].part_size);
                acumulador += particion[itera].part_size;
                char porcent[50];
                sprintf(porcent,"%d",porc);
                fprintf(archivo,"%%");
                fprintf(archivo,porcent);
                fprintf(archivo," </TD>\n");
            }
        }
        else
        {
            fprintf(archivo,"<TD ROWSPAN=\"3\"> LIBRE <BR/>\n");
            char porcent[50];
            int porc = sacar_porcentaje(t_dsk,t_dsk-acumulador);
            sprintf(porcent,"%d",porc);
            fprintf(archivo,"%%");
            fprintf(archivo,porcent);
            fprintf(archivo," </TD>\n");
        }
        itera++;
        acum_ptr_disk(archivo,particion,itera,t_dsk,acumulador);
    }
}

void concatenar_particion(int sizePtr,char *acum,PTR particiones[],int itera,int prtConcat,char *enlace,char *rs)
{
    if(itera < 4)
    {
        if(particiones[itera].part_status != '0')
        {
            if( sizePtr == particiones[itera].part_start )
            {
                strcat(acum,"struct");
                char pc[5];
                sprintf(pc,"%d",prtConcat);
                strcat(acum,&pc);
                strcat(acum,"[shape=record,label = \" ");

                if(particiones[itera].part_type == 'p')
                {
                    strcat(acum," Ptr Primaria \\n");
                    strcat(acum,&particiones[itera].part_name);
                    strcat(acum,"\\n part_start: ");
                    int ps[50];
                    sprintf(ps,"%d",particiones[itera].part_start);
                    strcat(acum,&ps);
                    strcat(acum ,"\\n part_size: ");
                    int psz[50];
                    sprintf(psz,"%d",particiones[itera].part_size);
                    strcat(acum,&psz);
                    strcat(acum,"\"];\n");

                }
                else if(particiones[itera].part_type == 'e')
                {
                    strcat(acum," Ptr Extendida \\n");
                    strcat(acum,&particiones[itera].part_name);
                    strcat(acum,"\\n part_start: ");
                    int ps[50];
                    sprintf(ps,"%d",particiones[itera].part_start);
                    strcat(acum,&ps);
                    strcat(acum,"\\n part_size: ");
                    int psz[50];
                    sprintf(psz,"%d",particiones[itera].part_size);
                    strcat(acum,&psz);
                    strcat(acum,"\"];\n");

                }

                strcat(enlace,"struct");
               //int pc[50];
                sprintf(pc,"%d",prtConcat);
                strcat(enlace,&pc);
                strcat(rs,"struct");
                strcat(rs,&pc);
                strcat(rs," ");
                prtConcat++;

                    sizePtr = particiones[itera].part_start + particiones[itera].part_size;

                    //if(itera !=3)
                       // enlace += " -> ";
                        strcat(enlace," -> ");

                    itera++;
                    concatenar_particion(sizePtr,*&acum, particiones,itera,prtConcat,*&enlace,*&rs);
            }
            else
            {
                int sizeLibre = particiones[itera].part_start - sizePtr;
                strcat(acum,"struct");
                int pc[50];
                sprintf(pc,"%d",prtConcat);
                strcat(acum,&pc);
                strcat(acum,"[shape=record,label = \" Libre \\n Espacio:");
                int sl[50];
                sprintf(sl,"%d",sizeLibre);
                strcat(acum,&sl);
                strcat(acum," \"];\n");

                strcat(rs,"struct");
                strcat(rs,&pc);
                strcat(rs," ");

                strcat(enlace,"struct");
                strcat(enlace,&pc);


                if(itera !=3)
                {
                    strcat(enlace," -> ");
                }
                  //  enlace += " -> ";

                sizePtr =  particiones[itera].part_start;
                prtConcat++;
                concatenar_particion(sizePtr,*&acum, particiones,itera,prtConcat,*&enlace,*&rs);
            }
        }
        else
        {
            itera++;
            concatenar_particion(sizePtr,*&acum, particiones,itera,prtConcat,*&enlace,*&rs);
        }
    }
}

int sacar_porcentaje(int val_disco, int val_actual)
{
    int resultado = val_actual * 100;
    resultado = resultado / val_disco;
    return resultado;
}

