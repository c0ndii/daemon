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
/**
 * @file
 * @brief Demon
 *
 * Plik posiadający jedną funkcję, która jest wywoływana przez demona, służy do sprawdzanie potrzeby kopiowania czy usuwania plików.
 */
/**@brief
*   Funkcja wywyływana przez demona, służy do sprawdzanie potrzeby kopiowania czy usuwania plików.
@param[in] argv
*   parametry wejściowe
@param[in] strLenSource
*   długość ścieżki pliku źródłowego
@param[in] strLenDest
*   długość ścieżki pliku docelowego
@param[in] dirSourcePath
*   scieżka pliku źródłowego
@param[in] dirDestPath
*   scieżka pliku docelowego
@retval '' Zwraca błąd, jego szczegóły można sprawdzić w pliku errors.txt
*/
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
        int fileSourceLen = strLenSource+strlen(in->d_name);
        int fileDestLen = strLenDest+strlen(in->d_name);
        char *fileSourcePath = (char*)malloc((fileSourceLen+1)*sizeof(char));
        strcpy(fileSourcePath, dirSourcePath);strcat(fileSourcePath, in->d_name);
        char *fileDestPath = (char*)malloc((fileDestLen+1)*sizeof(char));
        strcpy(fileDestPath, dirDestPath);strcat(fileDestPath, in->d_name);
        if(in->d_type == DT_DIR){
            if(argv[4]!=NULL){
                if(strcmp(argv[4],"-R")==0 && strcmp(in->d_name,"..")!=0 && strcmp(in->d_name,".")!=0){
                    addToList(&start, in->d_name,1);
                    updateTextFileParam("logs.txt","Wchodzenie w kopiowanie rekurencyjne katalogu: ",in->d_name);
                    if(copyRecursiveDir(fileSourcePath,fileDestPath,argv[3], 4)==0){
                        updateTextFileParam("logs.txt","Kopiowanie rekurencyjne katalogu sie powiodlo: ",in->d_name);
                    } else {
                        updateTextFileParam("errors.txt","Kopiowanie rekurencyjne katalogu sie nie powiodlo: ",in->d_name);
                    }
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
                        updateTextFile("errors.txt","Nie udało sie skopiowac plikow");
                    } else {
                        updateTextFileParam("logs.txt","Skopiowanie pliku sie powiodlo: ", in->d_name);
                        chmod(fileDestPath, inputFileAttrib.st_mode);
                    }
                }
            } else {   //jesli nie istnieje to tworzymy
                if(copyFile(fileSourcePath,fileDestPath,argv[3],inputFileAttrib)==-1)
                {
                    updateTextFile("errors.txt","Nie udalo sie skopiowac plikow");
                } else {
                    updateTextFileParam("logs.txt","Skopiowanie pliku sie powiodlo: ", in->d_name);
                    chmod(fileDestPath, inputFileAttrib.st_mode);
                }
            }
        }
        free(fileSourcePath);  //zwalniamy pamiec zeby wczytac nastepna nazwe pliku
        free(fileDestPath); //zwalniamy pamiec zeby wczytac nastepna nazwe pliku

    }
    while((out = readdir(tosync)) != NULL){
        int fileDelLen = strLenDest+strlen(out->d_name);
        char *fileToDel = (char*)malloc((fileDelLen+1)*sizeof(char));
        strcpy(fileToDel, dirDestPath);strcat(fileToDel, out->d_name);
        if(out->d_type == DT_DIR){
            if(argv[4]!=NULL){
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
            }
        } else {
            if(isThereThatFile(start, out->d_name, 0)==1){
                updateTextFile("logs.txt","Przystapienie do usuwania plikow");
                if(unlink(fileToDel)==0){
                    updateTextFileParam("logs.txt","Usuwanie plikow z folderu do synchronizacji sie powiodlo: ",out->d_name);
                } else {
                    updateTextFileParam("errors.txt","Usuwanie plikow z folderu do synchronizacji sie nie powiodlo: ",out->d_name);
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
    updateTextFile("logs.txt","Uspienie Deamona");
    deleteList(&start);
}
