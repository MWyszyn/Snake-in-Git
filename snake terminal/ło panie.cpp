#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <fstream>

using namespace std;



enum Kierunek
    {
        UP,
        RIGHT,
        DOWN,
        LEFT
    }
    dir;

class Point //podstawowa klasa opisuj¹ca punkt x,y w ukl wsp
{
    public:
    int width, height;
    Point (int w=0, int h=0)
    {
        width=w;
        height=h;//zgodnie z konwencja narzucona przez michala pierwsza wsp odpowiada wysokosci a druga szerokosci
    }

    void MoveTo (int w, int h)
    {
        width=w;
        height=h;
    }

    void MoveToPoint (Point p) //nadaje punktowi takie wspolrzedne jakie ma wczytany punkt
    {
        width=p.widthOut();
        height=p.heightOut();
    }

    bool IsEqual (int w, int h) //metoda sprawdzaj¹ca czy punkt ma podane wspolrzedne
    {
        if(w==width&&h==height) return true;
        else return false;
    }

    int widthOut()
    {
        return width;
    }

    int heightOut()
    {
        return height;
    }


};

class Snake //klasa opisuj¹ca polozenie/ulozenie weza w tablicy
{

    int l;

    public:
    Point* places; //tablica (dynamicznie alokowana bo ilosc segmentow bedzie sie zwiekszac) zawieraj¹ca punkty w ktorych znajduja sie kolejne segmenty weza

    Snake(int w, int h, Kierunek dir, int ll) //konstruktor tworzy w linii prostej weza z glowa w punkcie w,h ustawionego poczatkowo do rucchu w kirunku dir o dlugosci l
    {
        if(dir!=UP&&dir!=DOWN&&dir!=LEFT&&dir!=RIGHT) return;
        l=ll;
        places=new Point[l];
        switch(dir)
        {
        case LEFT:
            for(int i=0;i<l;i++) places[i].MoveTo(w+i,h);
            break;

        case RIGHT:
            for(int i=0;i<l;i++) places[i].MoveTo(w-i,h);
            break;

        case UP:
            for(int i=0;i<l;i++) places[i].MoveTo(w,h+i);
            break;

        case DOWN:
            for(int i=0;i<l;i++) places[i].MoveTo(w,h-i);
            break;

        default:
            break;
        }
    }


    ~Snake() //destruktor-zabija snake'a :(
    {
        delete places;
    }

    int howLong ()
    {
        return l;
    }

    void draw (char **tab) // rysuje snake'a w tabeli za pomoca 'o'-ów
    {
        *(tab[places[0].heightOut()]+places[0].widthOut())='X';
        for(int i=1;i<l;i++) *(tab[places[i].heightOut()]+places[i].widthOut())='o';
    }

    void vanish (char **tab) // wymazuje snake'a (jesli spacja nie odpowiada  za puste pole- do poprawy)
    {
        for(int i=0;i<l;i++) *(tab[places[i].heightOut()]+places[i].widthOut())=' ';
    }

    void step (Kierunek dir) //krok weza (bez uwzglednienia zebrania pnktu, wejscia w sciane
    {
        if(dir!=UP&&dir!=DOWN&&dir!=LEFT&&dir!=RIGHT) return; //sprawdza czy kierunek jest podany poprawnie
        for(int i=l-1;i>0;i--) places[i].MoveToPoint(places[i-1]); //przesuwa i-ty segment w miejsce wczesniejszego (segmenty licze od glowy ktora jest zerowym seg.)
        switch(dir)
        {
        case UP:
            places[0].MoveTo(places[0].widthOut(),places[0].heightOut()-1); //////////////////////////////////////////////////////
            break;                                                          //
        case DOWN:                                                          //
            places[0].MoveTo(places[0].widthOut(),places[0].heightOut()+1); //
            break;                                                          // przesuwanie glowy we wskazanym kierunku
        case LEFT:                                                          // (powyzszy for jej nie przesuwal)
            places[0].MoveTo(places[0].widthOut()-1,places[0].heightOut()); //
            break;                                                          //
        case RIGHT:                                                         //
            places[0].MoveTo(places[0].widthOut()+1,places[0].heightOut()); /////////////////////////////////////////////////////////
            break;
        }
    }

    void expandToDir (Kierunek dir)
    {

        if(dir!=UP&&dir!=DOWN&&dir!=LEFT&&dir!=RIGHT) return; //sprawdzenie poprawnosci wczytanego kierunku

        Point* temp=new Point[l+1]; //utworzenie nowej wiekszej tablicy na segmenty
        temp[0].MoveToPoint(places[0]); //przekopiowanie pozycji glowy

        for(int i=0;i<l;i++) temp[i+1].MoveToPoint(places[i]); //przekopiowanie wraz z przenumerowaniem elementow (w temp-ie "glowa" wystepuje dwa razy na zerowym i pierwszym miejscu, spokojnie tak ma być)

        switch(dir) //przesuniecie faktycznej glowy we wskazanym kierunku
        {
        case UP:
            temp[0].MoveTo(temp[0].widthOut(),temp[0].heightOut()-1);
            break;
        case DOWN:
            temp[0].MoveTo(temp[0].widthOut(),temp[0].heightOut()+1);
            break;
        case LEFT:
            temp[0].MoveTo(temp[0].widthOut()-1,temp[0].heightOut());
            break;
        case RIGHT:
            temp[0].MoveTo(temp[0].widthOut()+1,temp[0].heightOut());
            break;
        }
        l++; //zwiekszenie dlugosci

        delete places;//usuniecie starych pozycji
        places=temp; //przekopiowanie nowych pozycji  na miejsce starych

    }

    bool crash(char** tab,Kierunek dir) // metoda sprawdzajaca czy w nastepnym ruchu snake nie wejdzie w ramke badz w samego siebie
    {
        switch(dir)
        {
        case UP://sprawdza czy waz nie wchodzi w ramkę VVVVVVV                VVV samego siebie z pominięciem osatatniego segmentu (po ruchu ogona już tam nie będzie)
            if(tab[places[0].heightOut()-1][places[0].widthOut()]=='-'||(tab[places[0].heightOut()-1][places[0].widthOut()]=='o'&&!(places[l-1].IsEqual(places[0].heightOut()-1,places[0].widthOut())))) return true;
            else return false;
        case DOWN:
            if(tab[places[0].heightOut()+1][places[0].widthOut()]=='-'||(tab[places[0].heightOut()+1][places[0].widthOut()]=='o'&&!(places[l-1].IsEqual(places[0].heightOut()+1,places[0].widthOut())))) return true;
            else return false;

        case RIGHT:
            if(tab[places[0].heightOut()][places[0].widthOut()+1]=='|'||(tab[places[0].heightOut()][places[0].widthOut()+1]=='o'&&!(places[l-1].IsEqual(places[0].heightOut(),places[0].widthOut()+1)))) return true;
            else return false;

        case LEFT:
            if(tab[places[0].heightOut()][places[0].widthOut()-1]=='|'||(tab[places[0].heightOut()][places[0].widthOut()-1]=='o'&&!(places[l-1].IsEqual(places[0].heightOut(),places[0].widthOut()-1)))) return true;
            else return false;
        }
    }
    bool point(char** tab,Kierunek dir) // metoda sprawdzajaca czy w nastepnym ruchu snake nie wejdzie w ramke badz w samego siebie
    {
        switch(dir)
        {
        case UP://sprawdza czy waz nie wchodzi w ramkę VVVVVVV                VVV samego siebie z pominięciem osatatniego segmentu (po ruchu ogona już tam nie będzie)
            if(tab[places[0].heightOut()-1][places[0].widthOut()]=='+') return true;
            else return false;
        case DOWN:
            if(tab[places[0].heightOut()+1][places[0].widthOut()]=='+') return true;
            else return false;

        case RIGHT:
            if(tab[places[0].heightOut()][places[0].widthOut()+1]=='+') return true;
            else return false;

        case LEFT:
            if(tab[places[0].heightOut()][places[0].widthOut()-1]=='+') return true;
            else return false;
        }
    }
};


char** generuj_jedzenie (char** plansza, int wysokosc,int szerokosc)
{


    int x,y;
    while(1)
    {
        x=rand()% (wysokosc-2)+1; //pomijamy ramke
        y=rand()% (szerokosc-2)+1;

        if(plansza[x][y]!='o'&&plansza[x][y]!='X')
        {
            plansza[x][y]='+';
            return plansza;
        }

    }
    return plansza;
}

char **stworz(int wys,int szer)//funkcja tworzy tablice dwuwymiarow¹ o dowolnych rozmiarach i wypelnia ja na pusto
    {
    char *wektor=new char[szer];
    char **wektory=new char*[wys];
    for( int i = 0 ; i < wys ; i++)
        {
            wektory[i]=new char[szer];
        }
    for(int i = 0 ; i < wys ; i++ )
        {
            for (int  j = 0 ; j < szer ; j++)
            {
              wektory[i][j]=' ';
            }
        }
        return wektory;
    }

void wyswietl (char** plansza, int wys, int szer)
{
    int i,j;
    for(i=0;i<wys;i++)
    {
        for(j=0;j<szer;j++)
        {
            cout << plansza[i][j];
        }
        cout <<endl;
    }
}


//funkcja do przepisywania tablicy

char** przepisz_tablice (char** oryginal, char** kopia,int wys,int szer)
{
	int i,j;
  	for(i=0;i<wys;i++)
   	{
        	for(j=0;j<szer;j++)
        	{
            		kopia[i][j]=oryginal[i][j];
        	}
	}
	return kopia;
}

char** ramka(char **tab,int wys, int szer)//rysuje ramke do okola tablicy o zadanych wymiarach
    {
        for(int i = 0 ; i < szer ; i++)
        {
            tab[0][i]='-';
            tab[wys-1][i]='-';
        }
        for(int i = 1 ; i < (wys-1) ; i++)
        {
            tab[i][0]='|';
            tab[i][szer-1]='|';
        }
        return tab;
    }


int main()

{   int punkty=0;
    FILE*plikout=fopen("danewyj.txt","w");
    cout<<"Witaj w terminalowej grze snake"<<endl;
    dir=LEFT;
    Snake snake(5,7,dir,2); //kolejno wys glowy, szer glowy, poczatkowy dir ruchu, dlugosc weza
    cout<<"podaj nazwe gracza";
    char nazwa[50];
    fgets(nazwa,50,stdin);
    fprintf(plikout,"%s",nazwa);
    cout<<"Podaj szerokosc planszy"<<endl;
    int szer;
    cin>>szer;
    cout<<"Podaj wysokosc planszy"<<endl;
    int wys;
    cin>>wys;
    char **tab;

    tab=stworz(wys,szer);//piszemy w konwencji tab[wysokosc][szerekosc] czyli tab[2][5] to bedzie element w 3 wierwszy i 6 kolumnie*/
    tab=ramka(tab,wys,szer);

    char **kopia; //do przechowywania kopii planszy z poprzedniej iteracji, zeby sprawdzac, czy zostal zebrany punkt
    kopia=stworz(wys,szer);
    generuj_jedzenie(tab,wys,szer);

    przepisz_tablice(tab, kopia, wys,szer);

    system("cls");
    for(;;) //obsługi klawiatury nie ruszać
        {
        if(kbhit())
            {
                switch(getch())
                {
                case 'w':
                case 'W':
                    if(dir!=DOWN) dir=UP;
                    break;

                case 'a':
                case 'A':
                    if(dir!=RIGHT) dir=LEFT;
                    break;

                case 's':
                case 'S':
                    if(dir!=UP) dir=DOWN;
                    break;

                case 'd':
                case 'D':
                    if(dir!=LEFT) dir=RIGHT;

                    break;
                }
            }
        system("cls");
        if(snake.crash(tab,dir)==true) return punkty;//sprawdza czy waz nie umrze
        snake.vanish(tab);  //wymazuje starego weza z tablicy
        if(snake.point(tab,dir)==true)
        {
            snake.expandToDir(dir);
            generuj_jedzenie(tab,wys,szer);
            punkty++;
        }
        else snake.step(dir); // wykonuje krok
        snake.draw(tab); // wpisuje weza po wykonaniu kroku do tablicy
        wyswietl(tab,wys,szer); //wypisuje tablice na ekran

	/* ???? po co to???
	//warunek na generowanie jedzenia

    //wspolrzedne glowy
    int x,y;
    x=snake.places[0].width;
    y=snake.places[0].height;

	if(kopia[y][x]=='+') generuj_jedzenie(tab,wys,szer);

    przepisz_tablice(tab, kopia, wys,szer);*/


        Sleep(300);

        }
        fprintf(plikout,"%i\n",punkty);
        fclose(plikout);
}
