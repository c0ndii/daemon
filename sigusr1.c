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
#include "currentTime.h"
void funkcja_obslugujaca_sigusr1(int numer) {
    FILE *logs = fopen("logs.txt","a");
    fprintf(logs,"%sWybudzenie deamona z wykorzystaniem SIGUSR1\n",asctime(currentTime()));
    fclose(logs);
}