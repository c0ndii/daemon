#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include "daemon.h"
#include "copy.h"
#include "currentTime.h"
#include "deleteNotExisting.c"
#include "copyRecursive.h"



void daemon_at_work(char *argv[],FILE *logs,FILE *errors,int strLenSource,int strLenDest,char *dirSourcePath,char *dirDestPath)
{
    doesItExists_t * start = NULL;
    logs = fopen("logs.txt","a");
    errors = fopen("errors.txt","a");
    fprintf(logs,"%sWybudzenie Deamona\n",asctime(currentTime()));
    DIR *toread = opendir(argv[1]);
    DIR *tosync = opendir(argv[2]);
    struct dirent *in;
    struct dirent *out;
    if(toread == NULL){
        fprintf(errors,"%sNie udalo sie otworzyc pierwszego folderu\n",asctime(currentTime()));
        return;
    }
    if(tosync == NULL){
        fprintf(errors,"%sNie udalo sie otworzyc drugiego folderu\n",asctime(currentTime()));
        return;
    }
    while((in = readdir(toread)) != NULL){
        char *fileSourcePath = (char*)malloc((strLenSource+1)*sizeof(char));
        strcpy(fileSourcePath, dirSourcePath);strcat(fileSourcePath, in->d_name);
        char *fileDestPath = (char*)malloc((strLenDest+1)*sizeof(char));
        strcpy(fileDestPath, dirDestPath);strcat(fileDestPath, in->d_name);
        if(in->d_type == DT_DIR){
            if(strcmp(argv[4],"-R")==0 && strcmp(in->d_name,"..")!=0 && strcmp(in->d_name,".")!=0){
                addToList(&start, in->d_name,1);
                fprintf(logs,"%sWchodzenie w kopiowanie rekurencyjne katalogu %s\n",asctime(currentTime()),in->d_name);
                if(copyRecursiveDir(fileSourcePath,fileDestPath,argv[3], logs, errors, 4)==0){
                    fprintf(logs,"%sKopiowanie rekurencyjne katalogu %s sie powiodlo\n",asctime(currentTime()),in->d_name);
                } else {
                    fprintf(errors,"%sKopiowanie rekurencyjne katalogu %s sie nie powiodlo\n",asctime(currentTime()),in->d_name);
                }
            }
        } else {
            addToList(&start, in->d_name, 0);
            struct stat inputFileAttrib;
            struct stat outputFileAttrib;
            stat(fileSourcePath, &inputFileAttrib);
            stat(fileDestPath, &outputFileAttrib);
            if(access(fileDestPath, F_OK)==0){  //jesli istnieje to trzeba porownac daty
                if(copyOrNot(inputFileAttrib, outputFileAttrib)==1)
                {
                    if(copyFile(fileSourcePath,fileDestPath,argv[3],inputFileAttrib)==-1)
                    {
                        fprintf(errors,"%sNie udało się skopiować plików\n",asctime(currentTime()));
                    } else {
                        fprintf(logs,"%sSkopiowanie pliku %s się powiodło\n",asctime(currentTime()),in->d_name);
                        chmod(fileDestPath, inputFileAttrib.st_mode);
                    }
                }
            } else {   //jesli nie istnieje to tworzymy
                if(copyFile(fileSourcePath,fileDestPath,argv[3],inputFileAttrib)==-1)
                {
                    fprintf(errors,"%sNie udało się skopiować plików\n",asctime(currentTime()));
                } else {
                    fprintf(logs,"%sSkopiowanie pliku %s się powiodło\n",asctime(currentTime()),in->d_name);
                    chmod(fileDestPath, inputFileAttrib.st_mode);
                }
            }
        }
        free(fileSourcePath);  //zwalniamy pamiec zeby wczytac nastepna nazwe pliku
        free(fileDestPath); //zwalniamy pamiec zeby wczytac nastepna nazwe pliku

    }
    while((out = readdir(tosync)) != NULL){
        char *fileToDel = (char*)malloc((strLenDest+1)*sizeof(char));
        strcpy(fileToDel, dirDestPath);strcat(fileToDel, out->d_name);
        FILE * es = fopen("a","a");
        fprintf(es,"\nkurwa %s",fileToDel);
        fclose(es);
        if(out->d_type == DT_DIR){
            if(strcmp(argv[4],"-R")==0 && strcmp(out->d_name,"..")!=0 && strcmp(out->d_name,".")!=0){
                if(isThereThatFile(start, out->d_name, 1)==1){
                    if(deleteRecursive(fileToDel, fileToDel,fileToDel)==0){
                        //git
                    } else {
                        //blad
                    }
                }
            }
        } else {
            if(isThereThatFile(start, out->d_name, 0)==1){
                fprintf(logs,"%sUsuwanie pliku %s z folderu do synchronizacji się powiodło\n",asctime(currentTime()),out->d_name); //jeszcze nie zrobilismy wiec nie wiemy ze sie powiodlo/ przeredagowac komentarz do logow
                //tutaj dodac usuwanko
                unlink(fileToDel);
            }
        }
    }
    if(closedir(toread)==-1){
        fprintf(errors,"%sBlad podczas zamykania pliku nr1\n",asctime(currentTime()));
        return;
    }
    if(closedir(tosync)==-1){
        fprintf(errors,"%sBlad podczas zamykania pliku nr2\n",asctime(currentTime()));
        return;
    }
    fprintf(logs,"%sUśpienie Deamona\n",asctime(currentTime()));
    deleteList(&start);
    fclose(logs);
    fclose(errors);
}
