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
#include "updateTextFile.h"


void daemon_at_work(char *argv[],int strLenSource,int strLenDest,char *dirSourcePath,char *dirDestPath)
{
    doesItExists_t * start = NULL;
    updateTextFile("logs.txt","Wybudzenie Deamona");
    DIR *toread = opendir(argv[1]);
    DIR *tosync = opendir(argv[2]);
    struct dirent *in;
    struct dirent *out;
    if(toread == NULL){
        updateTextFile("logs.txt","Nie udalo sie otworzyc pierwszego folderu");
        return;
    }
    if(tosync == NULL){
        updateTextFile("errors.txt","Nie udalo sie otworzyc drugiego folderu");
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
                updateTextFileParam("logs.txt","Wchodzenie w kopiowanie rekurencyjne katalogu: ",in->d_name);
                if(copyRecursiveDir(fileSourcePath,fileDestPath,argv[3], 4)==0){
                    updateTextFileParam("logs.txt","Kopiowanie rekurencyjne katalogu sie powiodlo: ",in->d_name);
                } else {
                    updateTextFileParam("errors.txt","Kopiowanie rekurencyjne katalogu sie nie powiodlo: ",in->d_name);
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
                        updateTextFile("errors.txt","Nie udało się skopiować plików");
                    } else {
                        updateTextFileParam("logs.txt","Skopiowanie pliku się powiodło: ", in->d_name);
                        chmod(fileDestPath, inputFileAttrib.st_mode);
                    }
                }
            } else {   //jesli nie istnieje to tworzymy
                if(copyFile(fileSourcePath,fileDestPath,argv[3],inputFileAttrib)==-1)
                {
                    updateTextFile("errors.txt","Nie udało się skopiować plików");
                } else {
                    updateTextFileParam("logs.txt","Skopiowanie pliku się powiodło", in->d_name);
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
        if(out->d_type == DT_DIR){
            if(strcmp(argv[4],"-R")==0 && strcmp(out->d_name,"..")!=0 && strcmp(out->d_name,".")!=0){
                if(isThereThatFile(start, out->d_name, 1)==1){
                    updateTextFileParam("logs.txt","Rozpoczynanie usuwania rekurencyjnego katalogu: ",out->d_name);
                    if(deleteRecursive(fileToDel,4)==0){
                        updateTextFileParam("logs.txt","Usuwanie rekurencyjne katalogu sie powiodlo: ",out->d_name);
                    } else {
                        updateTextFileParam("errors.txt","Usuwanie rekurencyjne katalogu sie nie powiodlo: ",out->d_name);
                    }
                }
            }
        } else {
            if(isThereThatFile(start, out->d_name, 0)==1){
                updateTextFile("logs.txt","Przystapienie do usuwania plikow");
                if(unlink(fileToDel)==0){
                    updateTextFileParam("logs.txt","Usuwanie plików z folderu do synchronizacji sie powiodło: ",out->d_name);
                } else {
                    updateTextFileParam("errors.txt","Usuwanie plików z folderu do synchronizacji sie nie powiodło: ",out->d_name);
                }
            }
        }
        free(fileToDel);
    }
    if(closedir(toread)==-1){
        updateTextFile("errors.txt","Blad podczas zamykania katalogu SOURCE");
        return;
    }
    if(closedir(tosync)==-1){
        updateTextFile("errors.txt","Blad podczas zamykania katalogu DESTINATION");
        return;
    }
    updateTextFile("logs.txt","Uśpienie Deamona");
    deleteList(&start);
}
