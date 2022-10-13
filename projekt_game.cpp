#include <iostream>     // biblioteka standardowa
#include <vector>       // dla wykozystywanych wektorow
#include <cstdlib>      // dla uzywania klasy
#include <stdio.h>      // biblioteka umozliwiajaca wykozystanie get() i t p
#include <iomanip>      // dla zmiany true/false na 1/0 odpowiednio
#include <string>       // dla wpisywania z klawiatury
#include <fstream>      // biblioteka do zapisania pliku (save ze game)
#include <conio.h>      // pomoc konsolowego wej/wyj
#include <windows.h>    // do polozenia i rozmiaru konsoli

using namespace std;    // po tym nie piszemy std
using std::vector;      // zwracamy szczegolna uwage na vektora
using std::cout;        // zwracamy uwage ze mamy cout tez

vector<int> in_game_map(4);
vector<vector<int>> game_map(4, in_game_map);   // pole dla gry

vector<int> in_right_map(4);
vector<vector<int>> right_map(4, in_right_map); // napewno dobre pole dla gry

////////////////// Dzialanie pomocnicze //////////////////

class Menju {

private:
    string E;
    string R;
    string Q;
public:
    void UstawMenju(string E, string R, string Q) {
        this->E = E;
        this->R = R;
        this->Q = Q;//definicja metody wewnątrz klasy
    }
    void WypiszMenju();      //tylko deklaracja metody
};

void Menju::WypiszMenju() { // cialo metody

    cout << E << "\n" << R << "\n" << Q << endl;

}

struct K00rdynaty { //koordynata 0 elementa

    int x = 0;
    int y = 0;

} zero; // zerowe koordynaty OX OY

void konsola() {    // ustalamy pozycje konsoli

    // Get the window console handle
    HWND ConsoleWindow;
    ConsoleWindow = GetForegroundWindow();

    // Getting the desktop hadle and rectangule
    HWND   hwndScreen;
    RECT   rectScreen;
    hwndScreen = GetDesktopWindow();
    GetWindowRect(hwndScreen, &rectScreen);

    // Set windows size
    SetWindowPos(ConsoleWindow, NULL, 0, 0, 500, 500, SWP_SHOWWINDOW);

    // Get the current width and height of the console
    RECT rConsole;
    GetWindowRect(ConsoleWindow, &rConsole);
    int Width = rConsole.left = rConsole.right;
    int Height = rConsole.bottom - rConsole.top;

    // Caculate the window console to center of the screen	
    int ConsolePosX;
    int ConsolePosY;
    ConsolePosX = ((rectScreen.right - rectScreen.left) / 2 - Width / 2);
    ConsolePosY = ((rectScreen.bottom - rectScreen.top) / 2 - Height / 2);
    SetWindowPos(ConsoleWindow, NULL, ConsolePosX, ConsolePosY, Width, Height, SWP_SHOWWINDOW || SWP_NOSIZE);

}

////////////////// Dzialanie na polu gry //////////////////

void create_right_map() { // tworzymy poprawne pole gry i wypelniamy po kolei

    int right = 1;

    for (int i = 0; i < 4; i++) {

        for (int j = 0; j < 4; j++) {

            right_map[i][j] = right++;  // numeracja

        }

    }

    right_map[3][3] = 0; // ustalamy element zerowy w prawym dolnym rogu

}

void create_game_map() { // wypelniamy pole gry randomowymi elementami 

    int limit = 16; // zakres cyft na ktorych gramy
    int value = 0;
    vector<int> temporary; // elemnt czsowy ktory bedzie wskazywal na pole gry

    for (int i = 0; i < limit; i++) {

        temporary.push_back(i);

    }

    for (int i = 0; i < 4; i++) {

        for (int j = 0; j < 4; j++) {

            value = rand() % limit--;
            game_map[i][j] = temporary[value];

            if (temporary[value] == 0) { // zapisz zerowy element

                zero.x = j;
                zero.y = i;

            }

            temporary.erase(temporary.begin() + value); // usuwame element z mozliwej randomizacji

        }

    }

}

bool check_map() {   // porownanie biezacego pola do pola zakonczenia gry

    if (game_map == right_map) {

        return true;

    }
    else {

        return false;

    }

}

////////////////// Zapisywanie i ladowanie gry //////////////////

void save_game() {  //zapisanie gry

    if (game_map[0][0] == 0) {  //czy gracz spelnia warunki zapisania gry

        ofstream fin;

        fin.open("Saving.txt", ios::in);    //otwieramy plik dla zapisu

        for (int i = 0; i < 4; i++) {

            for (int j = 0; j < 4; j++) {


                fin << " " << setw(2) << setfill('0') << game_map[i][j];   //zapis progresu

            }

            fin << "\n";    //zapis progresu

        }

        fin.close(); //zamykamy plik zapisanej gry

        cout << "\nTwoja gra zostala zapisana";
        cout << "\nNastepnego razu mozesz rozpoczac gre z tego samego miejsca\n";
        cout << "\nWcisnij dowolna klawisze aby kontnuowac\n";
        system("pause");    //czekamy poki gracz przeczyta wskazowke

    }
    else {
        system("cls");
        cout << "\nNie spelniasz warunku zapisania\n";
        cout << "\nMusisz ustawic pusta klatke w lewym gornym rogu i powtozyc operacje\n";
        cout << "\nWcisnij dowolna klawisze aby kontnuowac\n";
        system("pause");    //czekamy poki gracz przeczyta wskazowke
    }

}

void load_game() { // wypelniamy pole gry randomowymi elementami 

    ifstream fout;
    string line;

    fout.open("Saving.txt", ios::out | ios::app);   //otwieramy plik zapisanej gry

    cout << line << endl;

    for (int i = 0; i < 4; i++) {

        for (int j = 0; j < 4; j++) {

            char str[4];    // w pliku mialem zapisane dane tekstowe

            fout.get(str, 4);   // odczytuke "plytke" krok po kroku

            game_map[i][j] = atoi(str); // zamieniam cyfre tekstowa na cyfre zeczywista

        }

        getline(fout, line);

    }

    fout.close();     //zamykamy plik

}

////////////////// Krokowanie po polu gry (zmiana polozenia pustego miejsca) //////////////////

void up_move() { // krokujemy w gore (dolny element w gore)

    if (zero.y > 0) {

        game_map[zero.y][zero.x] = game_map[zero.y - 1][zero.x];
        zero.y--;
        game_map[zero.y][zero.x] = 0;

    }

}

void down_move() {   // krokujemy w dol (gorny element w dol)

    if (zero.y < 3) {

        game_map[zero.y][zero.x] = game_map[zero.y + 1][zero.x];
        zero.y++;
        game_map[zero.y][zero.x] = 0;

    }

}

void right_move() { // krokujemy w prawo (lewy element w prawo)

    if (zero.x < 3) {

        game_map[zero.y][zero.x] = game_map[zero.y][zero.x + 1];
        zero.x++;
        game_map[zero.y][zero.x] = 0;

    }

}

void left_move() { // krokujemy w lewo (prawy element w lewo)

    if (zero.x > 0) {

        game_map[zero.y][zero.x] = game_map[zero.y][zero.x - 1];
        zero.x--;
        game_map[zero.y][zero.x] = 0;

    }

}

void get_direction() { // sprawdza ktora strzalka jest nacisnieta

    int move = static_cast<int> (_getch()); // UP = w, DOWN = s, RIGHT = d, LEFT = a

    switch (move) {

    case 'w': { up_move(); break; } //UP = w
    case 's': { down_move(); break; }   //DOWN = s
    case 'd': { right_move(); break; }   //RIGHT = d
    case 'a': { left_move(); break; }    //LEFT = a

    case 'e': { save_game(); break; }   //zapisywanie gry
    case 'q': { system("cls"); cout << "\nThanks for game!" << endl; exit(0); }     //wyjscie z gry. innej opcji nie ma

    default: { get_direction(); }

    }

}

////////////////// Proces dzialania samej gry //////////////////

void screen() { // odtwazanie gry na ekranie

    system("cls");

    for (int i = 0; i < 4; i++) {

        for (int j = 0; j < 4; j++) {

            if (game_map[i][j] != 0) {

                cout << setw(2) << setfill('0') << game_map[i][j] << ' '; // setw - szerokosc "kostki" (pole mamy 4*4, a wiec max liczbe - 15 (sklada sie z 2 cyfr); 
                                                                          //setfill - dadajemy 0 przed 1-9 (tak wygodniej patrzec) 

            }
            else {

                cout << "   "; // element zerowy

            }

        }

        cout << '\n';   // mamy 4 wierszy i 4 kolumny - rozdzielamy ich enterem

    }

    cout << "\nGame kontrol:\n";
    cout << "(WASD) Control\n";
    cout << "   (E) Zapisz gre\n";
    cout << "   (Q) Exit\n";


}

////////////////// Petla glowna //////////////////

int main() {    //petla glowna

    system("cls");  // make console clear

    string menu;

    Menju* mainmeniu = new Menju;

    mainmeniu->UstawMenju("(E) Game start", "(R) Continue the game", "(Q) Exit");    // zapisze dane do klasy

    konsola();  // ustalam konsole tak jak chce
    srand(static_cast<int>(time(NULL)));    // tu jestesmy pewni ze rand bedzie dzialalo dobrze po kazdym rozpoczeciu gry
    create_right_map(); // Twozymy dobrego pola dla gry

    do {

        mainmeniu->WypiszMenju();    // twozymy menu gry

        getline(cin, menu);

        if (menu == "q" || menu == "Q") { // exit

            system("cls");

            cout << "\nSee You Later" << endl;

            return(0);
        }
        else if (menu == "e" || menu == "E") { // grasz

            do {

                create_game_map();

            } while (check_map()); // Twozymy elementy do gry (pole, randomowe polozenie cyfr i zera)

            do {

                screen();
                get_direction();

            } while (!check_map()); // proces gry

            system("cls");

            cout << "\nYou win!\nGame over!\n";

        }
        else if (menu == "r" || menu == "R") { // kontynuj popszednia gre

            do {

                load_game();

            } while (check_map()); // otwieramy elementy z poprzedniej gry

            do {

                screen();
                get_direction();

            } while (!check_map()); // proces gry

            system("cls");

            cout << "\nYou win!\nGame over!\n";

            return(0);

        }
        else {  // popelniles blad w meni

            system("cls");
            cout << "\nNie mamy takiej opcji \n";
            cout << "Sproboj zmienic jezyk lub wcisnac inna klawisze\n" << endl;

        }

    } while (menu != "q" || menu == "Q");

    return(0);

}