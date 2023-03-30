#include <stdio.h>
#include <string.h>

typedef struct doesItExists {
    char name[32];
    int isDir;
    struct doesItExists * next;
} doesItExists_t;


int deleteRecursive(const char* dirPath, const char * logs, const char * errors){
    int checkFlag = 0;
    int fileLen = strlen(dirPath);
    DIR *toDelete = opendir(dirPath);
    if(toDelete==NULL){
        //tutaj wypisz blad
        return 1;
    }
    struct dirent *fileToDelete;
    while((fileToDelete = readdir(toDelete)) != NULL){
        if(strcmp(fileToDelete->d_name,"..")==0 || strcmp(fileToDelete->d_name,".")==0){
            continue;
        }
        FILE * es = fopen("a","a");
        fprintf(es,"\n%s",fileToDelete->d_name);
        fclose(es);
        char *filePath = (char*)malloc((fileLen+1)*sizeof(char));
        strcpy(filePath, dirPath);strcat(filePath, "/");strcat(filePath, fileToDelete->d_name);
        if(fileToDelete->d_type == DT_DIR){
            if(deleteRecursive(filePath, logs, errors)!=0){
                checkFlag = 1;
                //error
            }
        } else {
            if(unlink(filePath)!=0){
                checkFlag = 1;
            }
        }
        free(filePath);
    }
    if(closedir(toDelete)==-1){
        checkFlag = 1;
    }
    if(rmdir(dirPath)!=0){
        checkFlag = 1;
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


