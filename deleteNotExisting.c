#include <stdio.h>
#include <string.h>

typedef struct doesItExists {
    char name[32];
    int isDir;
    struct doesItExists * next;
} doesItExists_t;


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


