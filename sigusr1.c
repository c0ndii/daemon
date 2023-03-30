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
#include "updateTextFile.h"
void funkcja_obslugujaca_sigusr1(int numer) {
    updateTextFile("logs.txt","Wybudzenie deamona z wykorzystaniem SIGUSR1");
}
