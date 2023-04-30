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
/**
 * @file
 * @brief Aktualna godzina i data
 *
 * Plik posiadający jedną funkcję, której zadaniem jest zwracanie aktualnego czasu.
 */
/**@brief
*   Funkcja służąca do pobierania aktualnego czasu.
@retval timeinfo aktualny czas
*/
struct tm* currentTime()
{
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return timeinfo;
}