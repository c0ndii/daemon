#include <stdio.h>
#include <string.h>
#include "updateTextFile.h"
/**
 * @file
 * @brief Usuwanie plików
 *
 * Plik sprawdzający czy podany plik lub katalog istnieje w folderze źródłowym, jeśli nie, jest usuwany.
 */
/**@brief
*   Struktura przechowywująca wszystkie pliki które znajdują się w folderze źródłowym z uwzględnieniem podziału na foldery oraz pliki.
*/
typedef struct doesItExists {
    /**@brief
*   Przechowuje informacje o nazwie pliku lub katalogu
*/
    char name[32];
        /**@brief
*   Przechowuje informacje o tym czy element jest katalogiem
*/
    int isDir;
    /**@brief
*   Wzkaźnik na następny element 
*/
    struct doesItExists * next;
} doesItExists_t;

/**@brief
*   Funkcja usuwająca rekurencyjnie
@param[in] dirPath
*   scieżka folderu do usunięcia
@param[in] iter
*   służy do robienia "wcięć" w tekście
@retval 1 Zwraca błąd, jego szczegóły można sprawdzić w pliku errors.txt
@retval checkFlag zmienna przechowywująca informacje czy wystąpił błąd
*/
int deleteRecursive(const char* dirPath, int iter){
    int checkFlag = 0;
    DIR *toDelete = opendir(dirPath);
    if(toDelete==NULL){
        updateTextFileRecursive("errors.txt","Nie udalo sie otworzyc katalogu", iter);
        return 1;
    }
    struct dirent *fileToDelete;
    while((fileToDelete = readdir(toDelete)) != NULL){
        if(strcmp(fileToDelete->d_name,"..")==0 || strcmp(fileToDelete->d_name,".")==0){
            continue;
        }
        int fileLen = strlen(dirPath)+strlen(fileToDelete->d_name);
        char *filePath = (char*)malloc((fileLen+2)*sizeof(char));
        strcpy(filePath, dirPath);strcat(filePath, "/");strcat(filePath, fileToDelete->d_name);
        if(fileToDelete->d_type == DT_DIR){
            updateTextFileRecursiveParam("logs.txt","Wchodzenie w usuwanie rekurencyjne katalogu: ", iter, fileToDelete->d_name);
            if(deleteRecursive(filePath, iter+4)!=0){
                checkFlag = 1;
                updateTextFileRecursiveParam("errors.txt","Usuwanie rekurencyjne katalogu sie nie powiodlo: ", iter, fileToDelete->d_name);
            } else {
                updateTextFileRecursiveParam("logs.txt","Usuwanie rekurencyjne katalogu sie powiodlo: ", iter, fileToDelete->d_name);
            }
        } else {
            if(unlink(filePath)!=0){
                checkFlag = 1;
                updateTextFileRecursiveParam("errors.txt","Usuwanie rekurencyjne pliku sie nie powiodlo: ", iter, fileToDelete->d_name);
            } else {
                updateTextFileRecursiveParam("logs.txt","Usuwanie rekurencyjne pliku sie powiodlo: ", iter, fileToDelete->d_name);
            }
        }
        free(filePath);
    }
    if(closedir(toDelete)==-1){
        updateTextFileRecursive("errors.txt","Nie udalo sie zamknac katalogu DESTINATION", iter);
        checkFlag = 1;
    }
    if(rmdir(dirPath)!=0){
        updateTextFileRecursiveParam("errors.txt","Nie udalo sie usunac katalogu: ", iter, dirPath);
        checkFlag = 1;
    } else {
        updateTextFileRecursiveParam("logs.txt","Usuwanie katalogu: ", iter, dirPath);
    }
    return checkFlag;
}
/**@brief
*   Funkcja dodająca nazwy plików i folderów do struktury "doesItExists"
@param[in] start
*   początek listy
@param[in] name
*   nazwa pliku lub katalogu
@param[in] dirFlag
*   ustala rodzaj tzn. czy jest folderem
@retval '' Pusty return nie informuje o błędzie
*/
void addToList(doesItExists_t **start, const char* name, int dirFlag){
    if(*start ==NULL){
        *start = (doesItExists_t*)malloc(sizeof(doesItExists_t));
        strcpy((*start)->name,name);
        (*start)->isDir = dirFlag;
        (*start)->next = NULL;
    } else {
        doesItExists_t *current = *start;
        while(current->next!=NULL){
            current=current->next;
        }
        current->next = (doesItExists_t *)malloc(sizeof(doesItExists_t));
        strcpy(current->next->name,name);
        current->next->isDir=dirFlag;
        current->next->next=NULL;
    }
    return;
}
/**@brief
*   Wyczyszczenie listy
@param[in] start
*   początek listy
*/
void deleteList(doesItExists_t** start)
{
    doesItExists_t* current = (*start);
    doesItExists_t* nextNode;
    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
    *start = NULL;
}
/**@brief
*   Sprawdza czy podany plik lub katalog jest na liście
@param[in] start
*   początek listy
@param[in] name
*   nazwa pliku lub katalogu
@param[in] dirFlag
*   ustala rodzaj tzn. czy jest folderem
@retval 1 Plik nie został znaleziony
@retval 0 Plik został znaleziony
*/
int isThereThatFile(doesItExists_t *start, char* name, int dirFlag){
    doesItExists_t* current = start;
    while(current!=NULL){
        if(strcmp(current->name,name)==0 && current->isDir==dirFlag){
            return 0;
        }
        current = current->next;
    }
    return 1;
}


