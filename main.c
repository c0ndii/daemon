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
int main(int argc, char *argv[]) {
    FILE *logs = fopen("logs.txt","a");
    if(argc<3){
        printf("Podano mniej niz 2 argumenty\n");
        return 1;
    }
    struct stat buffer1;
    struct stat buffer2;
    stat(argv[1], &buffer1);
    stat(argv[2], &buffer2);
    if(!S_ISDIR(buffer1.st_mode) || !S_ISDIR(buffer2.st_mode)){
        fprintf(logs,"%sJeden lub oba parametry nie wskazuja na folder lub nie istnieja\n",asctime(currentTime()));
        return 1;
    }
    //tu juz powinno kopiowac w petli
    
    int strLenSource = strlen(argv[1]); //dlugosc path folderu source
    int strLenDest = strlen(argv[2]); //dlugosc path folderu destination
    char *dirSourcePath = (char *)malloc((strLenSource+1)*sizeof(char)); //alkowanie pamieci
    char *dirDestPath = (char *)malloc((strLenDest+1)*sizeof(char));
    strcpy(dirSourcePath, argv[1]); strcpy(dirDestPath, argv[2]); //kopiowanie zawartosci argv do stringow
    strcat(dirSourcePath, "/");strcat(dirDestPath, "/"); //dodawanie / zeby podac od razu nazwe pliku jako caly path
    strLenSource = strlen(dirSourcePath); //aktualizacja dlugosci do obslugi plikow
    strLenDest = strlen(dirDestPath);
    size_t bytes;
    fclose(logs);
    create_deamon();
    while (1)
    {
        logs = fopen("logs.txt","a");
        DIR *toread = opendir(argv[1]);
        DIR *tosync = opendir(argv[2]);
        struct dirent *in;
        if(toread == NULL){
            fprintf(logs,"%sNie udalo sie otworzyc pierwszego folderu\n",asctime(currentTime()));
            return 1;
        }
        if(tosync == NULL){
            fprintf(logs,"%sNie udalo sie otworzyc drugiego folderu\n",asctime(currentTime()));
            return 1;
        }
        while((in = readdir(toread)) != NULL){
        if(in->d_type == DT_DIR){
            continue;
        }
        char *fileSourcePath = (char*)malloc((strLenSource+1)*sizeof(char));
        strcpy(fileSourcePath, dirSourcePath);strcat(fileSourcePath, in->d_name);
        char *fileDestPath = (char*)malloc((strLenDest+1)*sizeof(char));
        strcpy(fileDestPath, dirDestPath);strcat(fileDestPath, in->d_name);

        if(access(fileDestPath, F_OK)==0){  //jesli istnieje to trzeba porownac daty
            struct stat inputFileAttrib;
            struct stat outputFileAttrib;
            stat(fileSourcePath, &inputFileAttrib);
            stat(fileDestPath, &outputFileAttrib);
            if(copyOrNot(inputFileAttrib, outputFileAttrib)==1)
            {
                if(copyFile(fileSourcePath,fileDestPath)==-1)
                {
                    fprintf(logs,"%sNie udało się skopiować plików\n",asctime(currentTime()));
                }
            }
            chmod(fileDestPath, inputFileAttrib.st_mode);


        } else {   //jesli nie istnieje to tworzymy
            if(copyFile(fileSourcePath,fileDestPath)==-1)
            {
                fprintf(logs,"%sNie udało się skopiować plików\n",asctime(currentTime()));
            }
            struct stat inputFileAttrib;
            struct stat outputFileAttrib;
            stat(fileSourcePath, &inputFileAttrib);
            stat(fileDestPath, &outputFileAttrib);
            chmod(fileDestPath, inputFileAttrib.st_mode);
        }
        free(fileSourcePath);  //zwalniamy pamiec zeby wczytac nastepna nazwe pliku
        free(fileDestPath); //zwalniamy pamiec zeby wczytac nastepna nazwe pliku
        }
        if(closedir(toread)==-1){
            fprintf(logs,"%sBlad podczas zamykania pliku nr1\n",asctime(currentTime()));
            return 1;
        }
        if(closedir(tosync)==-1){
            fprintf(logs,"%sBlad podczas zamykania pliku nr2\n",asctime(currentTime()));
            return 1;
        }
        fclose(logs);
        sleep(30);
    }
    return 0;
}