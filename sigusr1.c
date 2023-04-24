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
/**@brief
*   Funkcja informująca iż sygnał sigusr1 został przechwycony i zostanie obsłużony
@param[in] numer
*   zmnienna potrzebna do prawidłowego działania kodu
*/
void funkcja_obslugujaca_sigusr1(int numer) {
    updateTextFile("logs.txt","Wybudzenie deamona z wykorzystaniem SIGUSR1");
}
