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


int copyOrNot(struct stat sourceFile, struct stat destFile){
    time_t sourceTime = sourceFile.st_mtime;
    time_t destTime = destFile.st_mtime;
    ctime(&sourceTime);ctime(&destTime);
    if(sourceTime > destTime){
        printf("Trzeba kopiowac\n");
        return 1;
    }
    printf("Nie trzeba kopiowac\n");
    return 0;
}
int copyFile(char *fileSourcePath, char *fileDestPath)
{
    int size=16384;
    char fileBuffer[16384];
    int infile = open(fileSourcePath,O_RDONLY);
    int outfile = open(fileDestPath,O_WRONLY | O_CREAT | O_TRUNC); 
    if(infile==-1 || outfile==-1)
    {
        return -1;
    }
    while(1)
    {
        int readBytes = read(infile, fileBuffer, size);
        write(outfile, fileBuffer, readBytes);
        if(readBytes < size)
            break;
    }
    return 0;
}