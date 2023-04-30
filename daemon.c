#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
/**
 * @file
 * @brief Tworzenie demona
 *
 * Plik posiadający funkcję która służy do tworzenia demona.
 */

/**@brief
*   Funkcja tworząca demona- tworzony jest proces potomny, następnie zabijany proces "rodzic". "Osierocony" proces staje się demonem.
*/
void create_deamon()
{
    pid_t pid, sid;
    pid = fork();
    if(pid<0)
    {
        exit(EXIT_FAILURE);
    }
    if(pid>0)
    {
        exit(EXIT_SUCCESS);
    }
    umask(0);
    sid =setsid();
    if(sid<0)
    {
        exit(EXIT_FAILURE);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}
