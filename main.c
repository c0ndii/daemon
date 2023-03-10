#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
    int main(int argc, char **argv) {
    printf("Test! \n");
    if(argc<3)
    {
        printf("Za mało argumentów \n");
        return -1; 
    }
    struct stat buf1;
    struct stat buf2;
    stat(argv[1],&buf1);
    stat(argv[2],&buf2);
    if(!S_ISDIR(buf1.st_mode) || !S_ISDIR(buf2.st_mode))
    {
        printf("Not Directory \n");
        return 0;
    }
    printf("Directory \n");
    return 0;
}