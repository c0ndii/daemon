#include <stdio.h>
#include <string.h>
#include "updateTextFile.h"

typedef struct doesItExists {
    char name[32];
    int isDir;
    struct doesItExists * next;
} doesItExists_t;


int deleteRecursive(const char* dirPath, int iter){
    int checkFlag = 0;
    int fileLen = strlen(dirPath);
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
        char *filePath = (char*)malloc((fileLen+1)*sizeof(char));
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


