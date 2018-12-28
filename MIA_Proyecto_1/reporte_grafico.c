
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

//::::::::::::::::::::::: METODOS USADOS PARA EL TREE:::::::::::::::::::::::

//path del reporte es a donde lo voy a graficar
void reporteTree(FILE *archivo,int ini_particion, char *path_reporte)
{
    char *archivo_dot  = (char*)malloc(sizeof(char) * 100);
    memset(archivo_dot,0,sizeof(archivo_dot));
    char *extension = (char*)malloc(sizeof(char)*4);
    memset(extension,0,sizeof(extension));

    if(cvl_path_carpeta(path_reporte,&archivo_dot,&extension) == 1)
    {
       reporteTree_(archivo,ini_particion,archivo_dot,path_reporte,extension);
    }
    else
        printf("ERROR: Algo salio mal en la creacion de la carpeta con path: %s\n\n",path_reporte);
    free(archivo_dot);
    archivo_dot = NULL;
    free(extension);
    extension = NULL;
}

void reporteTree_(FILE *archivo,int ini_particion,char *path_dot,char *path_reporte,char *extension)
{
    SB sb;
    fseek(archivo,ini_particion,SEEK_SET);
    fread(&sb,sizeof(SB),1,archivo);

    FILE *archivo_dot;
    archivo_dot = fopen(path_dot,"w+"); //SI EXISTE O SI NO EXISTE LO CREA

    fprintf(archivo_dot,"digraph TREE {\n");
    fprintf(archivo_dot,"rankdir=\"LR\";\n");
    fprintf(archivo_dot,"node [shape=record];\n");
    fprintf(archivo_dot,"style=\"bold, filled, striped\";\n");
    escribir_inodo(archivo,archivo_dot,ini_particion,-1,0); // 0 porque iniciamos desde el padre
    fprintf(archivo_dot,"\n}");
    fclose(archivo_dot);

   char *comando = (char*)malloc(sizeof(char)*100);
   memset(comando,0,sizeof(comando));

   strcat(comando,"dot ");
   strcat(comando,path_dot);
   strcat(comando," -o ");
   strcat(comando,path_reporte);

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

void escribir_inodo(FILE * archivo,FILE *archivo_dot,int ini_particion,int bm_bloque,int bm_inodo)
{
    SB sb;
    fseek(archivo,ini_particion,SEEK_SET);
    fread(&sb,sizeof(SB),1,archivo);

    TI inodo_actual;
    int pos_inodo = sb.s_inode_start + (bm_inodo * sizeof(TI));
    fseek(archivo,pos_inodo,SEEK_SET);
    fread(&inodo_actual,sizeof(TI),1,archivo);
    char entero[50];
    fprintf(archivo_dot,"\tnode[width=2,style=\"filled\",fillcolor=\"orange\"];\n");
    fprintf(archivo_dot,"\tinodo");
    fprintf(archivo_dot,"%d",bm_inodo);
    fprintf(archivo_dot,"[fillcolor=\"orange\",label=\"\n");
    fprintf(archivo_dot,"\t\t{ INODO ");//  );
    fprintf(archivo_dot,"%d",bm_inodo);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ i_uid\t| ");
    fprintf(archivo_dot,"%d",inodo_actual.i_uid);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ i_gid\t| ");
    fprintf(archivo_dot,"%d",inodo_actual.i_gid);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ i_size\t| ");
    fprintf(archivo_dot,"%d",inodo_actual.i_size);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ i_atime\t| ");
    fprintf(archivo_dot,inodo_actual.i_atime);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ i_ctime\t| ");
    fprintf(archivo_dot,inodo_actual.i_ctime);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ i_mtime\t| ");
    fprintf(archivo_dot,inodo_actual.i_mtime);
    fprintf(archivo_dot," }|\n");

    for(int i = 0; i < 12; i++)
    {
        if(inodo_actual.i_block[i] != -1)
        {
            fprintf(archivo_dot,"\t\t{ i_block[");//\t| ");
            fprintf(archivo_dot,"%d",i+1);
            fprintf(archivo_dot,"]\t| ");
            fprintf(archivo_dot,"<ap");
            fprintf(archivo_dot,"%d",inodo_actual.i_block[i]);
            fprintf(archivo_dot,"> ");
            fprintf(archivo_dot,"%d",inodo_actual.i_block[i]);
            fprintf(archivo_dot," }|\n");
        }
    }

    fprintf(archivo_dot,"\t\t{ i_type\t| ");
    fprintf(archivo_dot,"%c",inodo_actual.i_type);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ i_perm\t| ");
    fprintf(archivo_dot,"%d",inodo_actual.i_perm);
    fprintf(archivo_dot," }|\n");

    fprintf(archivo_dot,"\t\"];\n");

    if(bm_bloque != -1)
    {
        //haciendo enlace
        fprintf(archivo_dot,"\tblock");
        fprintf(archivo_dot,"%d",bm_bloque);
        fprintf(archivo_dot,":");
        fprintf(archivo_dot,"ap");
        fprintf(archivo_dot,"%d",bm_inodo);
        fprintf(archivo_dot," -> ");
        fprintf(archivo_dot,"inodo");
        fprintf(archivo_dot,"%d",bm_inodo);
        fprintf(archivo_dot,"\n");

    }
    for(int i = 0; i < 12; i++)
    {
        if(inodo_actual.i_block[i] != -1)
        {
            if(inodo_actual.i_type == '0') //carpeta
                escribir_bloque_carpeta(archivo,archivo_dot,ini_particion,bm_inodo,inodo_actual.i_block[i]);
            else if(inodo_actual.i_type == '1') //carpeta
                escribir_bloque_archivo(archivo,archivo_dot,ini_particion,bm_inodo,inodo_actual.i_block[i]);
        }
    }
}

void escribir_bloque_carpeta(FILE * archivo,FILE *archivo_dot,int ini_particion,int bm_inodo,int bm_bloque)
{
    SB sb;
    fseek(archivo,ini_particion,SEEK_SET);
    fread(&sb,sizeof(SB),1,archivo);

    BC bc_actual;
    int pos_bloque = sb.s_block_start + (bm_bloque * 64);
    fseek(archivo,pos_bloque,SEEK_SET);
    fread(&bc_actual,sizeof(BC),1,archivo);

    fprintf(archivo_dot,"\tnode[width=2,style=\"filled\",fillcolor=\"olivedrab2\"];\n");
    fprintf(archivo_dot,"\tblock");
    fprintf(archivo_dot,"%d",bm_bloque);
    fprintf(archivo_dot,"[fillcolor=\"olivedrab2\",label=\"\n");
    fprintf(archivo_dot,"\t\t{ BLOQUE CARPETA ");//  );
    fprintf(archivo_dot,"%d",bm_bloque);
    fprintf(archivo_dot," }|\n");

    for(int i = 0; i < 4; i++)
    {
        if(bc_actual.b_content[i].b_inodo != -1)
        {
            fprintf(archivo_dot,"\t\t{");
            fprintf(archivo_dot,bc_actual.b_content[i].b_name);
            fprintf(archivo_dot,"\t| ");
            fprintf(archivo_dot,"<ap");
            fprintf(archivo_dot,"%d",bc_actual.b_content[i].b_inodo);
            fprintf(archivo_dot,"> ");
            fprintf(archivo_dot,"%d",bc_actual.b_content[i].b_inodo);
            fprintf(archivo_dot," }|\n");
        }
    }
    fprintf(archivo_dot,"\t\"];\n");

    //haciendo enlace
    fprintf(archivo_dot,"\tinodo");
    fprintf(archivo_dot,"%d",bm_inodo);
    fprintf(archivo_dot,":");
    fprintf(archivo_dot,"ap");
    fprintf(archivo_dot,"%d",bm_bloque);
    fprintf(archivo_dot," -> ");
    fprintf(archivo_dot,"block");
    fprintf(archivo_dot,"%d",bm_bloque);
    fprintf(archivo_dot,"\n");

    for(int i = 0; i < 4; i++)
    {
        if(bc_actual.b_content[i].b_inodo != -1)
        {
            if(strcmp(bc_actual.b_content[i].b_name,".") != 0 && strcmp(bc_actual.b_content[i].b_name,"..") != 0)
            {
                escribir_inodo( archivo,archivo_dot,ini_particion,bm_bloque,bc_actual.b_content[i].b_inodo);
            }

        }
    }
}


void escribir_bloque_archivo(FILE * archivo,FILE *archivo_dot,int ini_particion,int bm_inodo,int bm_bloque)
{
    SB sb;
    fseek(archivo,ini_particion,SEEK_SET);
    fread(&sb,sizeof(SB),1,archivo);

    BA ba_actual;
    int pos_bloque = sb.s_block_start + (bm_bloque * 64);
    fseek(archivo,pos_bloque,SEEK_SET);
    fread(&ba_actual,sizeof(BA),1,archivo);

    fprintf(archivo_dot,"\tnode[width=2,style=\"filled\",fillcolor=\"deepskyblue3\"];\n");
    fprintf(archivo_dot,"\tblock");
    fprintf(archivo_dot,"%d",bm_bloque);
    fprintf(archivo_dot,"[fillcolor=\"deepskyblue3\",label=\"\n");
    fprintf(archivo_dot,"\t\t{ BLOQUE ARCHIVO ");//  );
    fprintf(archivo_dot,"%d",bm_bloque);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ ");

    for(int i = 0; i < strlen(ba_actual.b_content); i++)
    {
        if(ba_actual.b_content[i] == '\n')
            fprintf(archivo_dot,"\\n");
        else
            fprintf(archivo_dot,"%c",ba_actual.b_content[i]);
    }


    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\"];\n");
    //haciendo enlace
    fprintf(archivo_dot,"\tinodo");
    fprintf(archivo_dot,"%d",bm_inodo);
    fprintf(archivo_dot,":");
    fprintf(archivo_dot,"ap");
    fprintf(archivo_dot,"%d",bm_bloque);
    fprintf(archivo_dot," -> ");
    fprintf(archivo_dot,"block");
    fprintf(archivo_dot,"%d",bm_bloque);
    fprintf(archivo_dot,"\n");

}

//::::::::::::::::::::::: METODOS USADOS PARA EL REPO INODE:::::::::::::::::::::::
void reporteInode(FILE *archivo,int ini_particion, char *path_reporte)
{
    char *archivo_dot  = (char*)malloc(sizeof(char) * 100);
    memset(archivo_dot,0,sizeof(archivo_dot));
    char *extension = (char*)malloc(sizeof(char)*4);
    memset(extension,0,sizeof(extension));

    if(cvl_path_carpeta(path_reporte,&archivo_dot,&extension) == 1)
    {
       reporteInode_(archivo,ini_particion,archivo_dot,path_reporte,extension);
    }
    else
        printf("ERROR: Algo salio mal en la creacion de la carpeta con path: %s\n\n",path_reporte);
    free(archivo_dot);
    archivo_dot = NULL;
    free(extension);
    extension = NULL;
}

void reporteInode_(FILE *archivo,int ini_particion,char *path_dot,char *path_reporte,char *extension)
{
    SB sb;
    fseek(archivo,ini_particion,SEEK_SET);
    fread(&sb,sizeof(SB),1,archivo);

    FILE *archivo_dot;
    archivo_dot = fopen(path_dot,"w+"); //SI EXISTE O SI NO EXISTE LO CREA

    fprintf(archivo_dot,"digraph INODE{\n");
    fprintf(archivo_dot,"rankdir=\"LR\";\n");
    fprintf(archivo_dot,"node [shape=record];\n");
    fprintf(archivo_dot,"style=\"bold, filled, striped\";\n");

    //parte donde escribimos todos los inodos
    int ini_bm_inodo = sb.s_bm_inode_start;
    int fin_bm_inodo = sb.s_bm_inode_start + sb.s_inodes_count;

    char bit;
    int bm_anterior = -1;
    int count_bm_inodo = 0;
    for (int i = ini_bm_inodo; i < fin_bm_inodo; i++)
    {
        fseek(archivo, i, SEEK_SET);
        fread(&bit, sizeof(char), 1, archivo);
        if (bit == '1')
        {
            escribir_inodo_tipo2(archivo,archivo_dot,ini_particion,bm_anterior,count_bm_inodo);
        }
        bm_anterior = count_bm_inodo;
        count_bm_inodo++;
    }

    //escribir_inodo_tipo2

    fprintf(archivo_dot,"\n}");
    fclose(archivo_dot);

   char *comando = (char*)malloc(sizeof(char)*100);
   memset(comando,0,sizeof(comando));

   strcat(comando,"dot ");
   strcat(comando,path_dot);
   strcat(comando," -o ");
   strcat(comando,path_reporte);

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


void escribir_inodo_tipo2(FILE * archivo,FILE *archivo_dot,int ini_particion,int bm_inodo_anterior,int bm_inodo_actual)
{
    SB sb;
    fseek(archivo,ini_particion,SEEK_SET);
    fread(&sb,sizeof(SB),1,archivo);

    TI inodo_actual;
    int pos_inodo = sb.s_inode_start + (bm_inodo_actual * sizeof(TI));
    fseek(archivo,pos_inodo,SEEK_SET);
    fread(&inodo_actual,sizeof(TI),1,archivo);
    char entero[50];
    fprintf(archivo_dot,"\tnode[width=2,style=\"filled\",fillcolor=\"orange\"];\n");
    fprintf(archivo_dot,"\tinodo");
    fprintf(archivo_dot,"%d",bm_inodo_actual);
    fprintf(archivo_dot,"[fillcolor=\"orange\",label=\"\n");
    fprintf(archivo_dot,"\t\t{ INODO ");//  );
    fprintf(archivo_dot,"%d",bm_inodo_actual);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ i_uid\t| ");
    fprintf(archivo_dot,"%d",inodo_actual.i_uid);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ i_gid\t| ");
    fprintf(archivo_dot,"%d",inodo_actual.i_gid);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ i_size\t| ");
    fprintf(archivo_dot,"%d",inodo_actual.i_size);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ i_atime\t| ");
    fprintf(archivo_dot,inodo_actual.i_atime);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ i_ctime\t| ");
    fprintf(archivo_dot,inodo_actual.i_ctime);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ i_mtime\t| ");
    fprintf(archivo_dot,inodo_actual.i_mtime);
    fprintf(archivo_dot," }|\n");

    for(int i = 0; i < 12; i++)
    {
        if(inodo_actual.i_block[i] != -1)
        {
            fprintf(archivo_dot,"\t\t{ i_block[");//\t| ");
            fprintf(archivo_dot,"%d",i+1);
            fprintf(archivo_dot,"]\t| ");
            fprintf(archivo_dot,"%d",inodo_actual.i_block[i]);
            fprintf(archivo_dot," }|\n");
        }
    }

    fprintf(archivo_dot,"\t\t{ i_type\t| ");
    fprintf(archivo_dot,"%c",inodo_actual.i_type);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ i_perm\t| ");
    fprintf(archivo_dot,"%d",inodo_actual.i_perm);
    fprintf(archivo_dot," }|\n");

    fprintf(archivo_dot,"\t\"];\n");

    if(bm_inodo_anterior != -1)
    {
        //haciendo enlace
        fprintf(archivo_dot,"\tinodo");
        fprintf(archivo_dot,"%d",bm_inodo_anterior);
        fprintf(archivo_dot," -> ");
        fprintf(archivo_dot,"inodo");
        fprintf(archivo_dot,"%d",bm_inodo_actual);
        fprintf(archivo_dot,"\n");

    }
}

//::::::::::::::::::::::: METODOS USADOS PARA EL REPO DE BM DE INODO:::::::::::::::::::::::
void reporte_bm_inodo(FILE *archivo,int ini_particion, char *path_reporte)
{
    char *archivo_dot  = (char*)malloc(sizeof(char) * 100);
    memset(archivo_dot,0,sizeof(archivo_dot));
    char *extension = (char*)malloc(sizeof(char)*4);
    memset(extension,0,sizeof(extension));

    if(cvl_path_carpeta(path_reporte,&archivo_dot,&extension) == 1)
    {
       reporte_bm_inodo_(archivo,ini_particion,path_reporte);
    }
    else
        printf("ERROR: Algo salio mal en la creacion de la carpeta con path: %s\n\n",path_reporte);
    free(archivo_dot);
    archivo_dot = NULL;
    free(extension);
    extension = NULL;
}

void reporte_bm_inodo_(FILE *archivo,int ini_particion,char *path_reporte)
{
    SB sb;
    fseek(archivo,ini_particion,SEEK_SET);
    fread(&sb,sizeof(SB),1,archivo);
    limpiar_path(path_reporte);
    FILE *archivo_repo;
    archivo_repo = fopen(path_reporte,"w+"); //SI EXISTE O SI NO EXISTE LO CREA

    int ini_bm_inodo = sb.s_bm_inode_start;
    int fin_bm_inodo = sb.s_bm_inode_start + sb.s_inodes_count;

    char bit;
    int count_bm_inodo = 0;
    for (int i = ini_bm_inodo; i < fin_bm_inodo; i++)
    {
        if(count_bm_inodo == 20)
        {
            fprintf(archivo_repo,"\n");
            count_bm_inodo = 0;
        }


        fseek(archivo, i, SEEK_SET);
        fread(&bit, sizeof(char), 1, archivo);
        fprintf(archivo_repo,"%c",bit);
        count_bm_inodo++;
    }
    fclose(archivo_repo);
}

//::::::::::::::::::::::: METODOS USADOS PARA EL REPO DE BM DE BLOQUE:::::::::::::::::::::::
void reporte_bm_bloque(FILE *archivo,int ini_particion, char *path_reporte)
{
    char *archivo_dot  = (char*)malloc(sizeof(char) * 100);
    memset(archivo_dot,0,sizeof(archivo_dot));
    char *extension = (char*)malloc(sizeof(char)*4);
    memset(extension,0,sizeof(extension));

    if(cvl_path_carpeta(path_reporte,&archivo_dot,&extension) == 1)
    {
       reporte_bm_bloque_(archivo,ini_particion,path_reporte);
    }
    else
        printf("ERROR: Algo salio mal en la creacion de la carpeta con path: %s\n\n",path_reporte);
    free(archivo_dot);
    archivo_dot = NULL;
    free(extension);
    extension = NULL;
}

void reporte_bm_bloque_(FILE *archivo,int ini_particion,char *path_reporte)
{
    SB sb;
    fseek(archivo,ini_particion,SEEK_SET);
    fread(&sb,sizeof(SB),1,archivo);
    limpiar_path(path_reporte);
    FILE *archivo_repo;
    archivo_repo = fopen(path_reporte,"w+"); //SI EXISTE O SI NO EXISTE LO CREA

    int ini_bm_inodo = sb.s_bm_block_start;
    int fin_bm_inodo = sb.s_bm_block_start + (3*sb.s_inodes_count);

    char bit;
    int count_bm_inodo = 0;
    for (int i = ini_bm_inodo; i < fin_bm_inodo; i++)
    {
        if(count_bm_inodo == 20)
        {
            fprintf(archivo_repo,"\n");
            count_bm_inodo = 0;
        }

        fseek(archivo, i, SEEK_SET);
        fread(&bit, sizeof(char), 1, archivo);
        fprintf(archivo_repo,"%c",bit);
        count_bm_inodo++;
    }
    fclose(archivo_repo);
}


//::::::::::::::::::::::: METODOS USADOS PARA EL REPO DE JOURNALING:::::::::::::::::::::::
void reporteLOG(FILE *archivo,int ini_particion, char *path_reporte)
{
    char *archivo_dot  = (char*)malloc(sizeof(char) * 100);
    memset(archivo_dot,0,sizeof(archivo_dot));
    char *extension = (char*)malloc(sizeof(char)*4);
    memset(extension,0,sizeof(extension));

    if(cvl_path_carpeta(path_reporte,&archivo_dot,&extension) == 1)
    {
      reporteLOG_(archivo,ini_particion,archivo_dot,path_reporte,extension);
    }
    else
        printf("ERROR: Algo salio mal en la creacion de la carpeta con path: %s\n\n",path_reporte);
    free(archivo_dot);
    archivo_dot = NULL;
    free(extension);
    extension = NULL;
}

void reporteLOG_(FILE *archivo,int ini_particion,char *path_dot,char *path_reporte,char *extension)
{
    SB sb;
    fseek(archivo,ini_particion,SEEK_SET);
    fread(&sb,sizeof(SB),1,archivo);

    FILE *archivo_dot;
    archivo_dot = fopen(path_dot,"w+"); //SI EXISTE O SI NO EXISTE LO CREA

    fprintf(archivo_dot,"digraph INODE{\n");
    fprintf(archivo_dot,"rankdir=\"LR\";\n");
    fprintf(archivo_dot,"node [shape=record];\n");
    fprintf(archivo_dot,"style=\"bold, filled, striped\";\n");

    //parte donde escribimos todos los journals
    int ini_log = ini_particion + sizeof(SB);
    int fin_log = ini_log + (sb.s_inodes_count * sizeof(LOG));

    LOG log_actual;
    int cont_jouornal = 0;
    int cont_jouornal_anterior = -1;
    for (int i = ini_log; i < fin_log; i = i + sizeof(LOG))
    {
        fseek(archivo, i, SEEK_SET);
        fread(&log_actual, sizeof(LOG), 1, archivo);

        if(log_actual.Journal_Tipo_Operacion != '0')
        {
            escribir_journal(archivo,archivo_dot,ini_particion,i,cont_jouornal_anterior,cont_jouornal);
        }
        cont_jouornal_anterior = cont_jouornal;
        cont_jouornal++;
    }

    //escribir_inodo_tipo2

    fprintf(archivo_dot,"\n}");
    fclose(archivo_dot);

   char *comando = (char*)malloc(sizeof(char)*100);
   memset(comando,0,sizeof(comando));

   strcat(comando,"dot ");
   strcat(comando,path_dot);
   strcat(comando," -o ");
   strcat(comando,path_reporte);

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


void escribir_journal(FILE *archivo,FILE *archivo_dot,int ini_particion,int pos_log,int bit_anterior,int bit_actual)
{

    LOG log;
    fseek(archivo,pos_log,archivo);
    fread(&log,sizeof(LOG),1,archivo);

    fprintf(archivo_dot,"\tnode[width=2,style=\"filled\",fillcolor=\"orange\"];\n");
    fprintf(archivo_dot,"\tlog");
    fprintf(archivo_dot,"%d",bit_actual);
    fprintf(archivo_dot,"[fillcolor=\"orange\",label=\"\n");
    fprintf(archivo_dot,"\t\t{ JOURNAL ");//  );
    fprintf(archivo_dot,"%d",bit_actual);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ Journal_Tipo_Operacion\t| ");
    fprintf(archivo_dot,"%c",log.Journal_Tipo_Operacion);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ Journal_tipo\t| ");
    fprintf(archivo_dot,"%c",log.Journal_tipo);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ Journal_nombre\t| ");
    fprintf(archivo_dot,log.Journal_nombre);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ Journal_contenido\t| ");
    fprintf(archivo_dot,log.Journal_contenido);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ Journal_fecha\t| ");
    fprintf(archivo_dot,log.Journal_fecha);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ Journal_propietario\t| ");
    fprintf(archivo_dot,"%c",log.Journal_propietario);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ Journal_permisos\t| ");
    fprintf(archivo_dot,"%d",log.Journal_permisos);
    fprintf(archivo_dot," }|\n");


    fprintf(archivo_dot,"\t\"];\n");

    if(bit_anterior != -1)
    {
        //haciendo enlace
        fprintf(archivo_dot,"\tlog");
        fprintf(archivo_dot,"%d",bit_anterior);
        fprintf(archivo_dot," -> ");
        fprintf(archivo_dot,"log");
        fprintf(archivo_dot,"%d",bit_actual);
        fprintf(archivo_dot,"\n");
    }
}

//::::::::::::::::::::::: METODOS USADOS PARA EL REPO SUPER BLOQUE :::::::::::::::::::::::
void reporte_sb(FILE *archivo,int ini_particion, char *path_reporte)
{
    char *archivo_dot  = (char*)malloc(sizeof(char) * 100);
    memset(archivo_dot,0,sizeof(archivo_dot));
    char *extension = (char*)malloc(sizeof(char)*4);
    memset(extension,0,sizeof(extension));

    if(cvl_path_carpeta(path_reporte,&archivo_dot,&extension) == 1)
    {
      reporte_sb_(archivo,archivo_dot,ini_particion,path_reporte,extension);
    }
    else
        printf("ERROR: Algo salio mal en la creacion de la carpeta con path: %s\n\n",path_reporte);
    free(archivo_dot);
    archivo_dot = NULL;
    free(extension);
    extension = NULL;
}

void reporte_sb_(FILE *archivo,char *path_dot,int ini_particion,char *path_reporte,char * extension)
{
    SB sb;
    fseek(archivo,ini_particion,SEEK_SET);
    fread(&sb,sizeof(SB),1,archivo);

    FILE *archivo_dot;
    archivo_dot = fopen(path_dot,"w+");

    fprintf(archivo_dot,"digraph mbr{\n");
    fprintf(archivo_dot,"graph [ratio=fill];\n");
    fprintf(archivo_dot,"node [label=\"\\N\", fontsize=15, shape=plaintext];\n");
    fprintf(archivo_dot,"graph [bb=\"0,0,352,154\"];\n");
    fprintf(archivo_dot,"arset [label=<\n");
    fprintf(archivo_dot,"<TABLE ALIGN=\"LEFT\">\n");
    fprintf(archivo_dot,"<TR>");
    fprintf(archivo_dot,"<TD> <B> NOMBRE </B> </TD> ");
    fprintf(archivo_dot,"<TD> <B> VALOR </B> </TD>");
    fprintf(archivo_dot,"</TR>\n");

    char tamanio [50];
    sprintf(tamanio,"%d",sb.s_filesystem_type);
    concat_2_val(archivo_dot,"s_filesystem_type",tamanio,3);

    sprintf(tamanio,"%d",sb.s_inodes_count);
    concat_2_val(archivo_dot,"s_inodes_count",tamanio,3);

    sprintf(tamanio,"%d",sb.s_blocks_count);
    concat_2_val(archivo_dot,"s_blocks_count",tamanio,3);


    sprintf(tamanio,"%d",sb.s_free_blocks_count);
    concat_2_val(archivo_dot,"s_free_blocks_count",tamanio,3);

    sprintf(tamanio,"%d",sb.s_free_inodes_count);
    concat_2_val(archivo_dot,"s_free_inodes_count",tamanio,3);

    concat_2_val(archivo_dot,"s_mtime",sb.s_mtime,3);

    concat_2_val(archivo_dot,"s_umtime",sb.s_umtime,3);

    sprintf(tamanio,"%d",sb.s_mnt_count);
    concat_2_val(archivo_dot,"s_mnt_count",tamanio,3);

    sprintf(tamanio,"%d",sb.s_magic);
    concat_2_val(archivo_dot,"s_magic",tamanio,3);

    sprintf(tamanio,"%d",sb.s_inode_size);
    concat_2_val(archivo_dot,"s_inode_size",tamanio,3);

    sprintf(tamanio,"%d",sb.s_block_size);
    concat_2_val(archivo_dot,"s_block_size",tamanio,3);

    sprintf(tamanio,"%d",sb.s_first_ino);
    concat_2_val(archivo_dot,"s_first_ino",tamanio,3);

        sprintf(tamanio,"%d",sb.s_first_blo);
    concat_2_val(archivo_dot,"s_first_blo",tamanio,3);

        sprintf(tamanio,"%d",sb.s_bm_inode_start);
    concat_2_val(archivo_dot,"s_bm_inode_start",tamanio,3);

        sprintf(tamanio,"%d",sb.s_bm_block_start);
    concat_2_val(archivo_dot,"s_bm_block_start",tamanio,3);

        sprintf(tamanio,"%d",sb.s_inode_start);
    concat_2_val(archivo_dot,"s_inode_start",tamanio,3);

            sprintf(tamanio,"%d",sb.s_block_start);
    concat_2_val(archivo_dot,"s_block_start",tamanio,3);

    fprintf(archivo_dot,"</TABLE>\n");
    fprintf(archivo_dot,">, ];\n");
    fprintf(archivo_dot,"\n}");

   fclose(archivo_dot);

   char *comando = (char*)malloc(sizeof(char)*100);
   memset(comando,0,sizeof(comando));
   strcat(comando,"dot ");
   strcat(comando,path_dot);
   strcat(comando," -o ");
   strcat(comando,path_reporte);

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

//::::::::::::::::::::::: METODOS USADOS PARA EL REPO DE BLOQUES :::::::::::::::::::::::
void reporte_blocks(FILE *archivo,int ini_particion,char *path_reporte)
{
    char *archivo_dot  = (char*)malloc(sizeof(char) * 100);
    memset(archivo_dot,0,sizeof(archivo_dot));
    char *extension = (char*)malloc(sizeof(char)*4);
    memset(extension,0,sizeof(extension));

    if(cvl_path_carpeta(path_reporte,&archivo_dot,&extension) == 1)
    {
        reporte_blocks_(archivo,ini_particion,archivo_dot,path_reporte,extension);
    }
    else
        printf("ERROR: Algo salio mal en la creacion de la carpeta con path: %s\n\n",path_reporte);
    free(archivo_dot);
    archivo_dot = NULL;
    free(extension);
    extension = NULL;
}

void reporte_blocks_(FILE *archivo,int ini_particion,char *path_dot,char *path_reporte,char *extension)
{
    SB sb;
    fseek(archivo,ini_particion,SEEK_SET);
    fread(&sb,sizeof(SB),1,archivo);

    FILE *archivo_dot;
    archivo_dot = fopen(path_dot,"w+"); //SI EXISTE O SI NO EXISTE LO CREA
    int bm_tmp = -1;
    fprintf(archivo_dot,"digraph BLOCKS{\n");
    fprintf(archivo_dot,"rankdir=\"LR\";\n");
    fprintf(archivo_dot,"node [shape=record];\n");
    fprintf(archivo_dot,"style=\"bold, filled, striped\";\n");

    inode_admin(archivo,archivo_dot,ini_particion,0,&bm_tmp);

    fprintf(archivo_dot,"\n}\n");
   fclose(archivo_dot);
   char *comando = (char*)malloc(sizeof(char)*100);
   memset(comando,0,sizeof(comando));
   strcat(comando,"dot ");
   strcat(comando,path_dot);
   strcat(comando," -o ");
   strcat(comando,path_reporte);

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

void inode_admin(FILE *archivo,FILE *archivo_dot,int ini_part,int bm_inodo,int *bm_block_ant)
{
    //primero sacamos el inodo
    SB sb;
    fseek(archivo,ini_part,SEEK_SET);
    fread(&sb,sizeof(SB),1,archivo);

    TI inodo_actual;
    int pos_inodo = sb.s_inode_start + (bm_inodo * sizeof(TI));
    fseek(archivo,pos_inodo,SEEK_SET);
    fread(&inodo_actual,sizeof(TI),1,archivo);


    for(int i = 0; i < 12; i++)
    {
        if(inodo_actual.i_block[i] != -1)
        {
            if(inodo_actual.i_type == '0') //de tipo carpeta
            {
                sacar_info_bloque_carpeta(archivo,archivo_dot,ini_part,&*bm_block_ant,inodo_actual.i_block[i]);
            }
            else if(inodo_actual.i_type == '1') //de tipo archivo
            {
                sacar_info_bloque_archivo(archivo,archivo_dot,ini_part,&*bm_block_ant,inodo_actual.i_block[i]);
            }
        }
    }

}

void sacar_info_bloque_carpeta(FILE *archivo,FILE *archivo_dot,int ini_part,int *bm_block_ant,int bm_block_actual)
{
    SB sb;
    fseek(archivo,ini_part,SEEK_SET);
    fread(&sb,sizeof(SB),1,archivo);

    BC bc_actual;
    int pos_bloque = sb.s_block_start + (bm_block_actual * 64);
    fseek(archivo,pos_bloque,SEEK_SET);
    fread(&bc_actual,sizeof(BC),1,archivo);

    fprintf(archivo_dot,"\tnode[width=2,style=\"filled\",fillcolor=\"olivedrab2\"];\n");
    fprintf(archivo_dot,"\tblock");
    fprintf(archivo_dot,"%d",bm_block_actual);
    fprintf(archivo_dot,"[fillcolor=\"olivedrab2\",label=\"\n");
    fprintf(archivo_dot,"\t\t{ BLOQUE CARPETA ");//  );
    fprintf(archivo_dot,"%d",bm_block_actual);
    fprintf(archivo_dot," }|\n");

    for(int i = 0; i < 4; i++)
    {
        if(bc_actual.b_content[i].b_inodo != -1)
        {
            fprintf(archivo_dot,"\t\t{");
            fprintf(archivo_dot,bc_actual.b_content[i].b_name);
            fprintf(archivo_dot,"\t| ");
            fprintf(archivo_dot,"%d",bc_actual.b_content[i].b_inodo);
            fprintf(archivo_dot," }|\n");
        }
    }
    fprintf(archivo_dot,"\t\"];\n");

    if(*bm_block_ant != -1)
    {
            //haciendo enlace
        fprintf(archivo_dot,"\tblock");
        fprintf(archivo_dot,"%d",*bm_block_ant);
        fprintf(archivo_dot," -> ");
        fprintf(archivo_dot,"block");
        fprintf(archivo_dot,"%d",bm_block_actual);
        fprintf(archivo_dot,"\n");


    }
    *bm_block_ant = bm_block_actual;

    for(int i = 0; i < 4; i++) //fijo apunta a un inodo
    {
        if(bc_actual.b_content[i].b_inodo != -1)
        {
            if(strcmp(bc_actual.b_content[i].b_name,".") != 0 && strcmp(bc_actual.b_content[i].b_name,"..") != 0)
            {
                inode_admin(archivo,archivo_dot,ini_part,bc_actual.b_content[i].b_inodo,&*bm_block_ant);
            }

        }
    }
}

void sacar_info_bloque_archivo(FILE *archivo,FILE *archivo_dot,int ini_part,int *bm_block_ant,int bm_block_actual)
{
    SB sb;
    fseek(archivo,ini_part,SEEK_SET);
    fread(&sb,sizeof(SB),1,archivo);

    BA ba_actual;
    int pos_bloque = sb.s_block_start + (bm_block_actual * 64);
    fseek(archivo,pos_bloque,SEEK_SET);
    fread(&ba_actual,sizeof(BA),1,archivo);

    fprintf(archivo_dot,"\tnode[width=2,style=\"filled\",fillcolor=\"deepskyblue3\"];\n");
    fprintf(archivo_dot,"\tblock");
    fprintf(archivo_dot,"%d",bm_block_actual);
    fprintf(archivo_dot,"[fillcolor=\"deepskyblue3\",label=\"\n");
    fprintf(archivo_dot,"\t\t{ BLOQUE ARCHIVO ");//  );
    fprintf(archivo_dot,"%d",bm_block_actual);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\t{ ");
    fprintf(archivo_dot,ba_actual.b_content);
    fprintf(archivo_dot," }|\n");
    fprintf(archivo_dot,"\t\"];\n");

    if(*bm_block_ant != -1)
    {
            //haciendo enlace
        fprintf(archivo_dot,"\tblock");
        fprintf(archivo_dot,"%d",*bm_block_ant);
        fprintf(archivo_dot," -> ");
        fprintf(archivo_dot,"block");
        fprintf(archivo_dot,"%d",bm_block_actual);
        fprintf(archivo_dot,"\n");


    }
    *bm_block_ant = bm_block_actual;
}

//::::::::::::::::::::::: METODOS USADOS PARA EL REPO FILE :::::::::::::::::::::::
void reporteFile(FILE *archivo,int ini_particion, char *path_reporte,int bm_ino_arch)
{
    char *archivo_dot  = (char*)malloc(sizeof(char) * 100);
    memset(archivo_dot,0,sizeof(archivo_dot));
    char *extension = (char*)malloc(sizeof(char)*4);
    memset(extension,0,sizeof(extension));

    if(cvl_path_carpeta(path_reporte,&archivo_dot,&extension) == 1)
    {
       reporteFile_(archivo,ini_particion,path_reporte,bm_ino_arch);
    }
    else
        printf("ERROR: Algo salio mal en la creacion de la carpeta con path: %s\n\n",path_reporte);
    free(archivo_dot);
    archivo_dot = NULL;
    free(extension);
    extension = NULL;
}

void reporteFile_(FILE *archivo,int ini_particion,char *path_reporte,int bm_ino_arch)
{
    SB sb;
    fseek(archivo,ini_particion,SEEK_SET);
    fread(&sb,sizeof(SB),1,archivo);

    FILE *archivo_repo;
    archivo_repo = fopen(path_reporte,"w+"); //SI EXISTE O SI NO EXISTE LO CREA

    int pos_block_archivo = sb.s_block_start + (bm_ino_arch * 64);
    BA bloque_archivo;
    fseek(archivo,pos_block_archivo,SEEK_SET);
    fread(&bloque_archivo,sizeof(BA),1,archivo);

    escribir_cont_archivo(archivo,archivo_repo,ini_particion,bm_ino_arch);

    fclose(archivo_repo);
    printf("REPORTE FILE GENERADO\n");
}



void escribir_cont_archivo(FILE *archivo,FILE *archivo_repo,int ini_particion,int bm_ino_arch)
{
    SB sb;
    fseek(archivo,ini_particion,SEEK_SET);
    fread(&sb,sizeof(SB),1,archivo);
    //get inodo archivo
    int pos_inoarch = sb.s_inode_start + (bm_ino_arch *  sizeof(TI));
    TI ino_arch;
    fseek(archivo,pos_inoarch,SEEK_SET);
    fread(&ino_arch,sizeof(TI),1,archivo);


    for(int i = 0; i < 12; i++)
    {
        if(ino_arch.i_block[i] != -1)
        {
            BA block_arch;
            int pos_ba = sb.s_block_start + (64 * ino_arch.i_block[i]);
            fseek(archivo,pos_ba,SEEK_SET);
            fread(&block_arch,sizeof(BA),1,archivo);
            fprintf(archivo_repo,block_arch.b_content);
        }
    }
}






void reporteLS(FILE *archivo,int ini_particion, char *path_reporte,int bm_inodo,LISTA_USR *lst_usr,char *ruta)
{
    char *archivo_dot  = (char*)malloc(sizeof(char) * 100);
    memset(archivo_dot,0,sizeof(archivo_dot));
    char *extension = (char*)malloc(sizeof(char)*4);
    memset(extension,0,sizeof(extension));

    if(cvl_path_carpeta(path_reporte,&archivo_dot,&extension) == 1)
    {
       reporteLS_(archivo,archivo_dot,ini_particion,path_reporte,bm_inodo,lst_usr,extension,ruta);
    }
    else
        printf("ERROR: Algo salio mal en la creacion de la carpeta con path: %s\n\n",path_reporte);
    free(archivo_dot);
    archivo_dot = NULL;
    free(extension);
    extension = NULL;
}

void reporteLS_(FILE *archivo,char *path_dot,int ini_particion,char *path_reporte,int bm_inodo,LISTA_USR *lst_usr,char *extension,char *ruta)
{
    SB sb;
    fseek(archivo,ini_particion,SEEK_SET);
    fread(&sb,sizeof(SB),1,archivo);

    //sacando al inodo
    int pos_inodo = sb.s_inode_start + (bm_inodo * sizeof(TI));
    TI inodo;
    fseek(archivo,pos_inodo,SEEK_SET);
    fread(&inodo,sizeof(TI),1,archivo);

    //antes de todo vergueo necesito saber si fue posible encontrar el nombre del usurio y el grupo sino que truene
    char nombre_grp[15];
    char nombre_usr[15];
    int result = 0;
    result =  get_nombre_usr_by_id(lst_usr, inodo.i_uid,&nombre_usr);
    if(result != 0)
    {
         result =  get_nombre_grp_by_id(lst_usr, inodo.i_gid,&nombre_grp);
         if(result != 0 )
         {
             //ahora si se puede continuar
            FILE *archivo_dot;
            archivo_dot = fopen(path_dot,"w+"); //SI EXISTE O SI NO EXISTE LO CREA


            fprintf(archivo_dot,"digraph ls{\n");
            fprintf(archivo_dot,"graph [ratio=fill];\n");
            fprintf(archivo_dot,"node [label=\"\\N\", fontsize=15, shape=plaintext];\n");
            fprintf(archivo_dot,"graph [bb=\"0,0,352,154\"];\n");
            fprintf(archivo_dot,"arset [label=<\n");
            fprintf(archivo_dot,"<TABLE ALIGN=\"LEFT\">\n");
            fprintf(archivo_dot,"<TR>");
            fprintf(archivo_dot,"<TD> <B> PERMISOS </B> </TD> ");
            fprintf(archivo_dot,"<TD> <B> OWNER </B> </TD>");
            fprintf(archivo_dot,"<TD> <B> GRUPO </B> </TD> ");
            fprintf(archivo_dot,"<TD> <B> SIZE </B> </TD>");
            fprintf(archivo_dot,"<TD> <B> FECHA </B> </TD> ");
            fprintf(archivo_dot,"<TD> <B> TIPO </B> </TD>");
            fprintf(archivo_dot,"<TD> <B> NAME </B> </TD>");
            fprintf(archivo_dot,"</TR>\n");

            char permisos[4];
            sprintf(permisos,"%d",inodo.i_perm);

            fprintf(archivo_dot,"<TR>");
            fprintf(archivo_dot,"<TD> ");
            decidir_permiso(archivo_dot,permisos[0]);
            decidir_permiso(archivo_dot,permisos[1]);
            decidir_permiso(archivo_dot,permisos[2]);
            fprintf(archivo_dot," </TD>");

            fprintf(archivo_dot,"<TD>");
            fprintf(archivo_dot,nombre_usr);
            fprintf(archivo_dot," </TD>");

            fprintf(archivo_dot,"<TD>");
            fprintf(archivo_dot,nombre_grp);
            fprintf(archivo_dot," </TD>");


            fprintf(archivo_dot,"<TD>");
            fprintf(archivo_dot,"%d",inodo.i_size);
            fprintf(archivo_dot," </TD>");

            fprintf(archivo_dot,"<TD>");
            fprintf(archivo_dot,inodo.i_ctime);
            fprintf(archivo_dot," </TD>");

            fprintf(archivo_dot,"<TD>");
            fprintf(archivo_dot,"%c",inodo.i_type);
            fprintf(archivo_dot," </TD>");

            fprintf(archivo_dot,"<TD>");
            fprintf(archivo_dot,ruta);
            fprintf(archivo_dot," </TD>");

            fprintf(archivo_dot,"</TR>\n");

            fprintf(archivo_dot,"</TABLE>\n");
            fprintf(archivo_dot,">, ];\n");
            fprintf(archivo_dot,"\n}");

            fclose(archivo_dot);

            char *comando = (char*)malloc(sizeof(char)*100);
            memset(comando,0,sizeof(comando));
            strcat(comando,"dot ");
            strcat(comando,path_dot);
            strcat(comando," -o ");
            strcat(comando,path_reporte);

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
         else
            printf("ERROR no se pudo hacer el reporte ls porque el nombre de grupo no se encontro\n\n");
    }
    else
        printf("ERROR no se pudo hacer el reporte ls porque el nombre de usuario no se encontro\n\n");

}

void decidir_permiso(FILE *archivo_dot,char tipo_permiso)
{
    if(tipo_permiso == '0')
    {
        fprintf(archivo_dot,"---");
    }
    else if(tipo_permiso == '1')
    {
        fprintf(archivo_dot,"--x");
    }
    else if(tipo_permiso == '2')
    {
        fprintf(archivo_dot,"-w-");
    }
    else if(tipo_permiso == '3')
    {
        fprintf(archivo_dot,"-wx");
    }
    else if(tipo_permiso == '4')
    {
        fprintf(archivo_dot,"r--");
    }
    else if(tipo_permiso == '5')
    {
        fprintf(archivo_dot,"r-x");
    }
    else if(tipo_permiso == '6')
    {
        fprintf(archivo_dot,"rw-");
    }
    else if(tipo_permiso == '7')
    {
        fprintf(archivo_dot,"rwx");
    }
}

