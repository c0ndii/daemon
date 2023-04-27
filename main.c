/*! \mainpage Strona główna dokumentacji Deamona
 *
 * \section Opis
 *
 * Demon którego głównym zadaniem jest utrzymywanie tej samej zawartości pomiędzy dwoma katalogami.\n
 * Po podaniu katalogu źródłowego i docelowego demon rozpoczyna swoją pracę.\n
 * Dodatkowo jest opcja aby program działał rekurencyjnie.
 *
 * \section Inicjalizacja 
 * \subsection Parametry
 * Parametr 1: Folder źródłowy\n
 * Parametr 2: Folder docelowy\n
 * Parametr 3: Wielkość która będzie dzeliła pliki na "małe" i "duże"\n
 * Parametr 4(opcjonalny): -R, służy do przeszukiwania rekurencyjnego
 * \subsection Działanie
 * Program działa w tle jako demon. Co 5 minut sprawdza stan folderów.\n
 * Jeżeli został znaleziony plik który znajduje się w katalogu źródłowym, a nie ma go w docelowym, zostaje do niego skopiowany.
 * Jeżeli plik istnieje zarówno w folderze źródłowym i docelowym zostaje sprawdzona data modyfikacji, od której zależy dalsza praca, jeżeli plik ma nowszą datę w folderze źródłowym zostaje skopiowany
 * do folderu docelowego, w innym wypadku nic się nie dzieje
 * Jeżeli w folderze docelowym zostaje znaleziony plik któego nie ma w folderze źródłowym zostaje on usunięty.\n
 * Jeżeli została wybrana opcja -R zostają sprawdzane również katalogi, i ich zawartości, z folderów źródłowego i docelowego.\n
 * \section Autorzy
 * Mateusz Kondraciuk\n
 * Jakub Franciszek Modzelewski
 * \section Prowadzący
 * dr inż. Wojciech Kwedlo
 */
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
/**@brief
*   Funkcja main rozpoczynająca działanie całego programu
@param[in] argc
*   Ilość parametrów wejściowych
@param[in] argv
*   parametry wejściowe
@retval 1 Zwraca błąd, jego szczegóły można podejrzeć w errors.txt
@retval 0 Kończy działanie programu
*/
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

    size_t bytes;
    printf("Uruchamianie Deamona\n");
    create_deamon();
    updateTextFile("logs.txt","---------------------------------------------------------");
    updateTextFile("logs.txt","Powstanie nowego Deamona");
    signal(SIGUSR1,funkcja_obslugujaca_sigusr1);
    while (1)
    {
        int strLenSource = strlen(argv[1]); //dlugosc path folderu source
        int strLenDest = strlen(argv[2]); //dlugosc path folderu destination
        char *dirSourcePath = (char *)malloc((strLenSource+2)*sizeof(char)); //alkowanie pamieci
        char *dirDestPath = (char *)malloc((strLenDest+2)*sizeof(char));
        strcpy(dirSourcePath, argv[1]); strcpy(dirDestPath, argv[2]); //kopiowanie zawartosci argv do stringow
        strcat(dirSourcePath, "/");strcat(dirDestPath, "/"); //dodawanie / zeby podac od razu nazwe pliku jako caly path
        strLenSource = strlen(dirSourcePath); //aktualizacja dlugosci do obslugi plikow
        strLenDest = strlen(dirDestPath);

        daemon_at_work(argv,strLenSource,strLenDest,dirSourcePath,dirDestPath);
        free(dirSourcePath);
        free(dirDestPath);
        sleep(30);

        
    }
    return 0;
}
