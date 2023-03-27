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
#include <sys/mman.h>
#include "copy.h"
#include "currentTime.h"

void writeSpaces(FILE * stream, int iter){
        for(int i=0; i<iter;i++){
            fprintf(stream, " ");
        }
}



int copyRecursiveDir(char *dirSource, char *dirDest, char *argv, FILE * logs, FILE * errors, int iter){
    int strLenSource = strlen(dirSource);
    int strLenDest = strlen(dirDest);
    int checkFlag = 0;
    struct stat inputDir;
    struct stat outputDir;
    stat(dirSource, &inputDir);
    if(stat(dirDest, &outputDir)==-1){
        mkdir(dirDest, inputDir.st_mode);
    }
    DIR *toread = opendir(dirSource);
    if(toread==NULL){
        writeSpaces(errors, iter);
        fprintf(errors,"%s",asctime(currentTime()));
        writeSpaces(errors, iter);
        fprintf(errors,"Nie znaleziono katalogu \n");
        return 1; //nie jestem pewien
    }
    struct dirent *in;
    while((in = readdir(toread)) != NULL){
        char *fileSourcePath = (char*)malloc((strLenSource+1)*sizeof(char));
        strcpy(fileSourcePath, dirSource);strcat(fileSourcePath, "/");strcat(fileSourcePath, in->d_name);
        char *fileDestPath = (char*)malloc((strLenDest+1)*sizeof(char));
        strcpy(fileDestPath, dirDest);strcat(fileDestPath, "/");strcat(fileDestPath, in->d_name);
        if(in->d_type == DT_DIR){
            if(strcmp(in->d_name,"..")!=0 && strcmp(in->d_name,".")!=0){
                writeSpaces(logs, iter);
                fprintf(logs,"%s",asctime(currentTime()));
                writeSpaces(logs, iter);
                fprintf(logs,"Wchodzenie w kopiowanie rekurencyjne katalogu %s\n",in->d_name);
                if(copyRecursiveDir(fileSourcePath,fileDestPath,argv, logs, errors, iter+4)!=0){
                    checkFlag = 1;
                    writeSpaces(errors, iter);
                    fprintf(errors,"%s",asctime(currentTime()));
                    writeSpaces(errors, iter);
                    fprintf(errors,"Kopiowanie rekurencyjne katalogu %s sie nie powiodlo\n",in->d_name);
                } else {
                    writeSpaces(logs, iter);
                    fprintf(logs,"%s",asctime(currentTime()));
                    writeSpaces(logs, iter);
                    fprintf(logs,"Kopiowanie rekurencyjne katalogu %s sie powiodlo\n",in->d_name);
                }
            }
        } else {
            struct stat inputFileAttrib;
            struct stat outputFileAttrib;
            stat(fileSourcePath, &inputFileAttrib);
            stat(fileDestPath, &outputFileAttrib);
            if(access(fileDestPath, F_OK)==0){  //jesli istnieje to trzeba porownac daty
                if(copyOrNot(inputFileAttrib, outputFileAttrib)==1)
                {
                    if(copyFile(fileSourcePath,fileDestPath,argv,inputFileAttrib)==-1)
                    {
                        checkFlag = 1;
                        writeSpaces(errors, iter);
                        fprintf(errors,"%s",asctime(currentTime()));
                        writeSpaces(errors, iter);
                        fprintf(errors,"Nie udalo siê skopiowac plikow\n");
                    } else {
                        writeSpaces(logs,iter);
                        fprintf(logs,"%s",asctime(currentTime()));
                        writeSpaces(logs,iter);
                        fprintf(logs,"Skopiowanie pliku %s sie powiodlo\n",in->d_name);
                        chmod(fileDestPath, inputFileAttrib.st_mode);
                    }
                }
            } else {   //jesli nie istnieje to tworzymy
                if(copyFile(fileSourcePath,fileDestPath,argv,inputFileAttrib)==-1)
                {
                    writeSpaces(errors, iter);
                    fprintf(errors,"%s",asctime(currentTime()));
                    writeSpaces(errors, iter);
                    fprintf(errors,"Nie udalo sie skopiowac plikow\n");
                    checkFlag = 1;
                } else {
                    writeSpaces(logs,iter);
                    fprintf(logs,"%s",asctime(currentTime()));
                    writeSpaces(logs,iter);
                    fprintf(logs,"Skopiowanie pliku %s sie powiodlo\n",in->d_name);
                    chmod(fileDestPath, inputFileAttrib.st_mode);
                }
            }
        }
        free(fileSourcePath);  //zwalniamy pamiec zeby wczytac nastepna nazwe pliku
        free(fileDestPath); //zwalniamy pamiec zeby wczytac nastepna nazwe pliku
    }
    if(closedir(toread)==-1){
        checkFlag = 1;
    }
    writeSpaces(logs,iter);
    fprintf(logs,"%s",asctime(currentTime()));
    writeSpaces(logs,iter);
    fprintf(logs,"Wychodzenie z rekurencji\n");
    return checkFlag;
}
