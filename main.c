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
#include <signal.h>
#include "daemon.h"
#include "copy.h"
#include "currentTime.h"
#include "daemon_at_work.h"
#include "sigusr1.h"
#include "updateTextFile.h"

int main(int argc, char *argv[]) {
    if(argc<3){
        updateTextFile("errors.txt","Podano mniej niz 2 argumenty");
        printf("Błąd, zajrzyj do errors.txt\n");
        return 1;
    }
    struct stat buffer1;
    struct stat buffer2;
    stat(argv[1], &buffer1);
    stat(argv[2], &buffer2);
    if(!S_ISDIR(buffer1.st_mode) || !S_ISDIR(buffer2.st_mode)){
        updateTextFile("errors.txt","Jeden lub oba parametry nie wskazuja na folder lub nie istnieja");
        printf("Błąd, zajrzyj do errors.txt\n");
        return 1;
    }
    //tu juz powinno kopiowac w petli

    int strLenSource = strlen(argv[1]); //dlugosc path folderu source
    int strLenDest = strlen(argv[2]); //dlugosc path folderu destination
    char *dirSourcePath = (char *)malloc((strLenSource+1)*sizeof(char)); //alkowanie pamieci
    char *dirDestPath = (char *)malloc((strLenDest+1)*sizeof(char));
    strcpy(dirSourcePath, argv[1]); strcpy(dirDestPath, argv[2]); //kopiowanie zawartosci argv do stringow
    strcat(dirSourcePath, "/");strcat(dirDestPath, "/"); //dodawanie / zeby podac od razu nazwe pliku jako caly path
    strLenSource = strlen(dirSourcePath); //aktualizacja dlugosci do obslugi plikow
    strLenDest = strlen(dirDestPath);
    size_t bytes;
    printf("Uruchamianie Deamona\n");
    create_deamon();
    updateTextFile("logs.txt","---------------------------------------------------------");
    updateTextFile("logs.txt","Powstanie nowego Deamona");
    signal(SIGUSR1,funkcja_obslugujaca_sigusr1);
    while (1)
    {
        daemon_at_work(argv,strLenSource,strLenDest,dirSourcePath,dirDestPath);
        sleep(30);
    }
    return 0;
}
