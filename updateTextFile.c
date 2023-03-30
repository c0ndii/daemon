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
void updateTextFile(char *fileName,char *tekst)
{
    FILE *file = fopen(fileName, "a");
    fprintf(file,"%s%s\n",asctime(currentTime()),tekst);
    fclose(file);
}