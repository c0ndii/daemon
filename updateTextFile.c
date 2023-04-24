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
#include "currentTime.h"
/**@brief
*   Funkcja dodająca tekst do wybranego pliku
@param[in] fileName
*   nazwa pliku do którego dodajemy tekst
@param[in] text
*   Tekst który dodajemy do pliku
*/
void updateTextFile(const char *fileName,const char *text)
{
    FILE *file = fopen(fileName, "a");
    fprintf(file,"%s%s\n",asctime(currentTime()),text);
    fclose(file);
}
/**@brief
*   Funkcja dodająca tekst do wybranego pliku z użyciem zmiennej
@param[in] fileName
*   nazwa pliku do którego dodajemy tekst
@param[in] text
*   Tekst który dodajemy do pliku
@param[in] param
*   Zmienna przechowywująca nazwy plików lub katalogów
*/
void updateTextFileParam(const char *fileName,const char *text,const char *param)
{
    FILE *file = fopen(fileName, "a");
    fprintf(file,"%s%s%s\n",asctime(currentTime()),text, param);
    fclose(file);
}
/**@brief
*   Funkcja dodająca tekst rekurencyjnie do wybranego pliku
@param[in] fileName
*   nazwa pliku do którego dodajemy tekst
@param[in] text
*   Tekst który dodajemy do pliku
@param[in] iter
*   służy do robienia wcięć w tekście
@
*/
void updateTextFileRecursive(const char * fileName,const char *text, int iter){
    FILE *file = fopen(fileName, "a");
    for(int i=0; i<iter;i++){
        fprintf(file, " ");
    }
    fprintf(file,"%s",asctime(currentTime()));
    for(int i=0; i<iter;i++){
        fprintf(file, " ");
    }
    fprintf(file,"%s\n",text);
    fclose(file);
}
/**@brief
*   Funkcja dodająca tekst rekurencyjnie do wybranego pliku z użyciem zmiennej
@param[in] fileName
*   nazwa pliku do którego dodajemy tekst
@param[in] text
*   Tekst który dodajemy do pliku
@param[in] param
*   Zmienna przechowywująca nazwy plików lub katalogów
@param[in] iter
*   służy do robienia wcięć w tekście
*/
void updateTextFileRecursiveParam(const char * fileName,const char *text, int iter,const char* param){
    FILE *file = fopen(fileName, "a");
    for(int i=0; i<iter;i++){
        fprintf(file, " ");
    }
    fprintf(file,"%s",asctime(currentTime()));
    for(int i=0; i<iter;i++){
        fprintf(file, " ");
    }
    fprintf(file,"%s",text);
    fprintf(file,"%s\n",param);
    fclose(file);
}
