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
#include "updateTextFile.h"


int copyRecursiveDir(char *dirSource, char *dirDest, char *argv, int iter){
    int checkFlag = 0;
    struct stat inputDir;
    struct stat outputDir;
    stat(dirSource, &inputDir);
    if(stat(dirDest, &outputDir)==-1){
        mkdir(dirDest, inputDir.st_mode);
    }
    DIR *toread = opendir(dirSource);
    if(toread==NULL){
        updateTextFileRecursive("errors.txt","Nie znaleziono katalogu",iter);
        return 1; //nie jestem pewien
    }
    struct dirent *in;
    while((in = readdir(toread)) != NULL){
        int strLenSource = strlen(dirSource)+strlen(in->d_name);
        int strLenDest = strlen(dirDest)+strlen(in->d_name);
        char *fileSourcePath = (char*)malloc((strLenSource+2)*sizeof(char));
        strcpy(fileSourcePath, dirSource);strcat(fileSourcePath, "/");strcat(fileSourcePath, in->d_name);
        char *fileDestPath = (char*)malloc((strLenDest+2)*sizeof(char));
        strcpy(fileDestPath, dirDest);strcat(fileDestPath, "/");strcat(fileDestPath, in->d_name);
        if(in->d_type == DT_DIR){
            if(strcmp(in->d_name,"..")!=0 && strcmp(in->d_name,".")!=0){
                updateTextFileRecursiveParam("logs.txt","Wchodzenie w kopiowanie rekurencyjne katalogu: ", iter, in->d_name);
                if(copyRecursiveDir(fileSourcePath,fileDestPath,argv, iter+4)!=0){
                    checkFlag = 1;
                    updateTextFileRecursiveParam("errors.txt","Kopiowanie rekurencyjne katalogu sie nie powiodlo: ", iter, in->d_name);
                } else {
                    updateTextFileRecursiveParam("logs.txt","Kopiowanie rekurencyjne katalogu sie powiodlo: ", iter, in->d_name);
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
                        updateTextFileRecursive("errors.txt","Nie udalo sie skopiowac plikow", iter);
                    } else {
                        updateTextFileRecursiveParam("logs.txt","Skopiowanie pliku sie powiodlo", iter, in->d_name);
                        chmod(fileDestPath, inputFileAttrib.st_mode);
                    }
                }
            } else {   //jesli nie istnieje to tworzymy
                if(copyFile(fileSourcePath,fileDestPath,argv,inputFileAttrib)==-1)
                {
                    updateTextFileRecursive("errors.txt","Nie udalo sie skopiowac plikow", iter);
                    checkFlag = 1;
                } else {
                    updateTextFileRecursiveParam("logs.txt","Skopiowanie pliku sie powiodlo: ", iter, in->d_name);
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
    updateTextFileRecursive("logs.txt","Wychodzenie z rekurencji", iter);
    return checkFlag;
}
