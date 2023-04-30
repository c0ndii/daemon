/*! \mainpage Strona główna dokumentacji Demona
 *
 *
 * \section sec Dokumentacja
 * <table style="width:100%">
 * <tr>
 *   <td>Wydział Informatyki Politechniki Białostockiej<br>
 *      Przedmiot: Systemy Operacyjne</td>
 *   <td>Data oddania: 28.04.2023 r.</td>
 * </tr>
 * <tr>
 *   <td>Pracownia specjalistyczna nr 2<br>
 *      Zespół:
 *      1. Mateusz Kondraciuk
 *      2. Jakub Franciszek Modzelewski</td>
 *   <td>Prowadzący:<br>
 *      dr inż. Wojciech Kwedlo<br>
 *      Ocena:</td>
 * </tr>
 * </table>
 * \section sec4 Realizacja zadania
 * W celu sprawdzenia treści zadania proszę udać się pod \ref page1.<br>
 * W celu sprawdzenia wykonanych podpunktów proszę udać się pod \ref page2.<br>
 * W celu sprawdzenia instrukcji obsługi proszę udać się pod \ref page3. 
 * \page page1 Treść Zadania
 * <b>Temat 2 - Demon synchronizujący dwa podkatalogi</b><br>
[12p.] Program który otrzymuje co najmniej dwa argumenty: ścieżkę źródłową oraz ścieżkę docelową . Jeżeli któraś ze ścieżek nie jest katalogiem program powraca natychmiast z komunikatem błędu. 
W przeciwnym wypadku staje się demonem. Demon wykonuje następujące czynności: śpi przez pięć minut (czas spania można zmieniać przy pomocy dodatkowego opcjonalnego argumentu), po czym po obudzeniu się 
porównuje katalog źródłowy z katalogiem docelowym. Pozycje które nie są zwykłymi plikami są ignorowane (np. katalogi i dowiązania symboliczne). Jeżeli demon (a) napotka na  nowy plik w katalogu 
źródłowym, i tego pliku brak w katalogu docelowym lub (b) plik w katalogu źrodłowym ma późniejszą datę ostatniej modyfikacji demon wykonuje kopię pliku z  katalogu źródłowego do katalogu docelowego - 
ustawiając w katalogu docelowym datę modyfikacji tak aby przy kolejnym obudzeniu nie trzeba było wykonać kopii (chyba  ze plik w katalogu źródłowym zostanie ponownie zmieniony). Jeżeli zaś odnajdzie 
plik w katalogu docelowym, którego nie ma w katalogu źródłowym to usuwa ten plik z katalogu docelowego. Możliwe jest również natychmiastowe obudzenie się demona poprzez wysłanie mu sygnału SIGUSR1. 
Wyczerpująca informacja o każdej akcji typu uśpienie/obudzenie się demona (naturalne lub w wyniku sygnału), wykonanie kopii lub usunięcie pliku jest przesłana do logu systemowego. Informacja ta powinna 
zawierać aktualną datę.<br>
a) [10p.]  Dodatkowa opcja -R pozwalająca na rekurencyjną synchronizację katalogów (teraz pozycje będące katalogami nie są ignorowane). W szczególności jeżeli demon  stwierdzi w katalogu docelowym  
podkatalog którego brak w katalogu źródłowym powinien usunąć go wraz z zawartością.<br>
b) [12p.] W zależności  od rozmiaru plików dla małych plików wykonywane jest kopiowanie przy pomocy read/write a w przypadku dużych przy pomocy mmap/write (plik źródłowy) zostaje zamapowany w całości w 
pamięci. Próg dzielący pliki małe od dużych  może być przekazywany jako opcjonalny argument.<br>
Uwagi: (a) Wszelkie operacje na plikach i tworzenie demona należy wykonywać przy pomocy API Linuksa a nie standardowej biblioteki języka C (b)  kopiowanie za każdym obudzeniem całego drzewa katalogów 
zostanie potraktowane jako poważny błąd (c) podobnie jak przerzucenie części zadań na shell systemowy (funkcja system).
 * Demon którego głównym zadaniem jest utrzymywanie tej samej zawartości pomiędzy dwoma katalogami.\n
 * Po podaniu katalogu źródłowego i docelowego demon rozpoczyna swoją pracę.\n
 * Dodatkowo jest opcja aby program działał rekurencyjnie.
 *<br>
 * W celu sprawdzenia wykonanych podpunktów proszę udać się pod \ref page2.
 * 
 */
/*! \page page2 Wykonane podpunkty
 * \section sec2 Podpunkty
 * \subsection subsection1 Główne zadanie
 *  Program otrzymuje dwa katalogi. Co 5 minut sprawdza ich stan, dąży do tego aby katalog który został podany jako drugi argument posiadał te same pliki co katalog pierwszy. Kopiuje pliki i je usuwa.
 * <br> Dodatkowo możliwe jest natychmiastowe wybudzenie demona wysyłając sygnał SIGUSR1.
 * \subsection subsection2 Rekurencyjna synchronizacja katalogów
 * Dodatkowa opcja -R pozwala na rekurencyjną synchronizację katalogów.
 * \subsection subsection3 Rozmiar pliku
 * Inna metoda kopiowania pliku w zależności od jego rozmiaru. Próg dzielący pliki na małe i duże jest przekazywany jako argument.
 * 
 * \page page3 Instrukcja obsługi
 * \section sec3 Parametry podawane przy uruchamianiu programu
 * \subsection subsection4 Folder źródłowy
 * Pierwszym parametrem do podania jest folder źródłowy- folder którego pliki i katalogi będziemy kopiować.
 * \subsection subsection5 Folder docelowy
 * Drugim parametrem do podania jest folder docelowy- folder do którego pliki i katalogi będą kopiowane.
 * \subsection subsection6 Rozmiar pliku
 * Kolejnym parametrem jest wielkość która będzie dzieliła pliki na "małe" i "duże". Wielkość ta jest podawana w b.
 * \subsection subsection7 Rekurencyjna synchronizacja katalogów -R (opcjonaly)
 * Ostatnim parametrem jest opcja -R pozwalająca na rekurencyjną synchronizację katalogów.
 * \section Przykłady 
 * \subsection subsection8 ./program source dest 1000000
 * Wywołujemy program, jako folder źródłowy został wybrany katalog source, jako docelowy dest, 1000000b jest wielkością dzielącą pliki na małe i duże.
 * \subsection subsection9 ./program source dest 1000000 -R
 * To samo co w poprzednim z tą różnicą, że dodatkowo została użyta opcja -R, czyli nasz program będzie rekurencyjnie synchronizował katalogi.
*/

/**
 * @file
 * @brief Główny plik
 *
 * Plik rozpoczynający pracę całego programu.<br>
 * To tu sprawdzane jest wywołanie sygnału SIGUSR1.<br>
 * W tym pliku możemy zmienić czas "snu" demona.
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
*   Funkcja main wywoływana na początku działania programu, po odebraniu parametrów wejściowych wywołuje demona.
@param[in] argc
*   Ilość parametrów wejściowych
@param[in] argv
*   parametry wejściowe
@retval 1 Zwraca błąd, jego szczegóły można sprawdzić w pliku errors.txt
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
        sleep(300);

        
    }
    return 0;
}
