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
void updateTextFile(const char *fileName,const char *text)
{
    FILE *file = fopen(fileName, "a");
    fprintf(file,"%s%s\n",asctime(currentTime()),text);
    fclose(file);
}

void updateTextFileParam(const char *fileName,const char *text,const char *param)
{
    FILE *file = fopen(fileName, "a");
    fprintf(file,"%s%s%s\n",asctime(currentTime()),text, param);
    fclose(file);
}

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
