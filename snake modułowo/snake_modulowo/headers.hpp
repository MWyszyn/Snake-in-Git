#ifndef headers.hpp
#define headers.hpp

#include <iostream>
#include <cstdlib>
#include <ctime>

enum Kierunek
    {
        UP,
        RIGHT,
        DOWN,
        LEFT
    };

class Point //podstawowa klasa opisuj¹ca punkt x,y w ukl wsp
{
    public:
    int width, height;

    Point (int w, int h);

    void MoveTo (int w, int h);
    void MoveToPoint (Point p); //nadaje punktowi takie wspolrzedne jakie ma wczytany punkt
    bool IsEqual (int w, int h); //metoda sprawdzaj¹ca czy punkt ma podane wspolrzedne
    int widthOut();
    int heightOut();

};

class Snake //klasa opisuj¹ca polozenie/ulozenie weza w tablicy
{

    int l;

    public:
    Point* places; //tablica (dynamicznie alokowana bo ilosc segmentow bedzie sie zwiekszac) zawieraj¹ca punkty w ktorych znajduja sie kolejne segmenty weza

    Snake(int, int, Kierunek, int); //konstruktor tworzy w linii prostej weza z glowa w punkcie w,h ustawionego poczatkowo do rucchu w kirunku dir o dlugosci l
    ~Snake(); //destruktor-zabija snake'a :(

    int howLong ();
    void draw (char **tab); // rysuje snake'a w tabeli za pomoca 'o'-ów
    void vanish (char **tab); // wymazuje snake'a (jesli spacja nie odpowiada  za puste pole- do poprawy)
    void step (Kierunek dir); //krok weza (bez uwzglednienia zebrania pnktu, wejscia w sciane
    void expandToDir (Kierunek dir);
    bool crash(char** tab,Kierunek dir); // metoda sprawdzajaca czy w nastepnym ruchu snake nie wejdzie w ramke badz w samego siebie
    bool point(char** tab,Kierunek dir); // metoda sprawdzajaca czy w nastepnym ruchu snake nie wejdzie w ramke badz w samego siebie
};

char** generuj_jedzenie (char** plansza, int wysokosc,int szerokosc);
char **stworz(int wys,int szer);//funkcja tworzy tablice dwuwymiarow¹ o dowolnych rozmiarach i wypelnia ja na pusto
void wyswietl (char** plansza, int wys, int szer);
char** przepisz_tablice (char** oryginal, char** kopia,int wys,int szer);
char** ramka(char **tab,int wys, int szer);//rysuje ramke do okola tablicy o zadanych wymiarach

#endif
