#include <SFML/Graphics.hpp>
#include <cmath>
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <fstream>
#define border_up 20
#define border_left 20
#define border 20

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
    sf::ConvexShape glowa;
    sf::CircleShape* tail;


    Snake(int w, int h, Kierunek dir, int ll) //konstruktor tworzy w linii prostej weza z glowa w punkcie w,h ustawionego poczatkowo do rucchu w kirunku dir o dlugosci l
    {
        if(dir!=UP&&dir!=DOWN&&dir!=LEFT&&dir!=RIGHT) return;
        l=ll;
        places=new Point[l];
        tail=new sf::CircleShape[l-1];
        glowa.setPointCount(6);
        glowa.setPoint(0, sf::Vector2f(5, 0));
        glowa.setPoint(1, sf::Vector2f(15, 0));
        glowa.setPoint(2, sf::Vector2f(20, 15));
        glowa.setPoint(3, sf::Vector2f(15, 20));
        glowa.setPoint(4, sf::Vector2f(5, 20));
        glowa.setPoint(5, sf::Vector2f(0, 15));

        glowa.setFillColor(sf::Color::Green);

        switch(dir)
        {
        case LEFT:
            for(int i=0;i<l;i++) places[i].MoveTo(w+i,h);
            glowa.setRotation(270);
            glowa.setPosition(places[0].widthOut()*20-border,places[0].heightOut()*20+20-border);

            break;

        case RIGHT:
            for(int i=0;i<l;i++) places[i].MoveTo(w-i,h);
            glowa.setRotation(90);
            glowa.setPosition(places[0].widthOut()*20-border,places[0].heightOut()*20-border);

            break;

        case UP:
            for(int i=0;i<l;i++) places[i].MoveTo(w,h+i);
            glowa.setRotation(0);
            glowa.setPosition(places[0].widthOut()*20-border,places[0].heightOut()*20-border);
            break;

        case DOWN:
            for(int i=0;i<l;i++) places[i].MoveTo(w,h-i);
            glowa.setRotation(180);
            glowa.setPosition(places[0].widthOut()*20+20-border,places[0].heightOut()*20+20-border);

            break;

        default:
            break;
        }

        for(int i=0;i<(l-1);i++)
        {
            tail[i].setRadius(10);
            tail[i].setPointCount(30);
            tail[i].setPosition(places[i+1].widthOut()*20/*-border_left*/,places[i+1].heightOut()*20/*-border_up*/);
            tail[i].setFillColor(sf::Color::Green);
        }


    }


    ~Snake() //destruktor-zabija snake'a :(
    {
        delete places;
        delete tail;
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

    void vanish (char **tab) // wymazuje snake'a
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
            glowa.setRotation(0);
            glowa.setPosition(places[0].widthOut()*20,places[0].heightOut()*20);
            break;                                                          //
        case DOWN:                                                          //
            places[0].MoveTo(places[0].widthOut(),places[0].heightOut()+1); //
            glowa.setRotation(180);
            glowa.setPosition(places[0].widthOut()*20+20,places[0].heightOut()*20+20);
            break;                                                          // przesuwanie glowy we wskazanym kierunku
        case LEFT:                                                          // (powyzszy for jej nie przesuwal)
            places[0].MoveTo(places[0].widthOut()-1,places[0].heightOut()); //
            glowa.setRotation(270);
            glowa.setPosition(places[0].widthOut()*20,places[0].heightOut()*20+20);
            break;                                                          //
        case RIGHT:                                                         //
            places[0].MoveTo(places[0].widthOut()+1,places[0].heightOut()); /////////////////////////////////////////////////////////
            glowa.setRotation(90);
            glowa.setPosition(places[0].widthOut()*20+20,places[0].heightOut()*20);

            break;
        }

        for(int i=0;i<(l-1);i++)
        {
            tail[i].setPosition(places[i+1].widthOut()*20,places[i+1].heightOut()*20);
        }
    }

    void expandToDir (Kierunek dir)
    {

        if(dir!=UP&&dir!=DOWN&&dir!=LEFT&&dir!=RIGHT) return; //sprawdzenie poprawnosci wczytanego kierunku

        Point* temp=new Point[l+1]; //utworzenie nowej wiekszej tablicy na segmenty
        temp[0].MoveToPoint(places[0]); //przekopiowanie pozycji glowy

        for(int i=0;i<l;i++) temp[i+1].MoveToPoint(places[i]); //przekopiowanie wraz z przenumerowaniem elementow (w temp-ie "glowa" wystepuje dwa razy na zerowym i pierwszym miejscu, spokojnie tak ma być)


        delete tail;
        tail=new sf::CircleShape[l];

        for(int i=0;i<(l);i++)
        {
            tail[i].setRadius(10);
            tail[i].setPointCount(30);
            tail[i].setPosition(temp[i+1].widthOut()*20,temp[i+1].heightOut()*20);
            tail[i].setFillColor(sf::Color::Green);
        }

        switch(dir) //przesuniecie faktycznej glowy we wskazanym kierunku
        {
        case UP:
            temp[0].MoveTo(temp[0].widthOut(),temp[0].heightOut()-1);
            glowa.setRotation(0);
            glowa.setPosition(temp[0].widthOut()*20,temp[0].heightOut()*20);
            break;
        case DOWN:
            temp[0].MoveTo(temp[0].widthOut(),temp[0].heightOut()+1);
            glowa.setRotation(180);
            glowa.setPosition(temp[0].widthOut()*20+20,temp[0].heightOut()*20+20);
            break;
        case LEFT:
            temp[0].MoveTo(temp[0].widthOut()-1,temp[0].heightOut());
            glowa.setRotation(270);
            glowa.setPosition(temp[0].widthOut()*20,temp[0].heightOut()*20+20);
            break;
        case RIGHT:
            temp[0].MoveTo(temp[0].widthOut()+1,temp[0].heightOut());
            glowa.setRotation(90);
            glowa.setPosition(temp[0].widthOut()*20+20,temp[0].heightOut()*20);
            break;
        }
        l++; //zwiekszenie dlugosci

        delete places;//usuniecie starych pozycji
        places=temp; //przekopiowanie nowych pozycji  na miejsce starych
        //glowa.setPosition(places[0].widthOut()*20+border_left,places[0].heightOut()*20+border_left);
        //for(int i=0;i<l-1;i++) tail[i].setPosition(temp[i+1].widthOut()*20,temp[i+1].heightOut()*20);

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


char** generuj_jedzenie (char** plansza, int wysokosc, int szerokosc, int& fh, int& fw)
{

    while(1)
    {
        fh=rand()% (wysokosc-2)+1; //pomijamy ramke
        fw=rand()% (szerokosc-2)+1;

        if(plansza[fh][fw]!='o'&&plansza[fh][fw]!='X')
        {
            plansza[fh][fw]='+';
            return plansza;
        }

    }
    return plansza;
}

char **stworz(int wys,int szer)//funkcja tworzy tablice dwuwymiarow¹ o dowolnych rozmiarach i wypelnia ja na pusto
    {
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
{
    int szer, wys, punkty=0,fw=0,fh=0;
    int& rfw=fw;
    int& rfh=fh;
    dir=RIGHT;
    Snake snake(5,8,dir,3);//kolejno wys glowy, szer glowy, poczatkowy dir ruchu, dlugosc weza
    char nazwa[50];
    char** tab;
    int poziom;


    FILE*plikoutpkt=fopen("danewyjpkt.txt","a");
    FILE*plikoutnames=fopen("danewyjnames.txt","a");

    cout<<"Witaj w terminalowej grze snake"<<endl;
    cout<<"podaj nazwe gracza"<<endl;
    fgets(nazwa,50,stdin);
    fprintf(plikoutnames,"%s",nazwa);
    fclose(plikoutnames);
    cout<<"Podaj szerokosc planszy"<<endl;
    cin>>szer;

    cout<<"Podaj wysokosc planszy"<<endl;
    cin>>wys;
    cout<<"Podaj poziom trudnosci w skali 1-10"<<endl;
    cin>>poziom;
    poziom=200/poziom;


    sf::ConvexShape gwiazda(5);
    gwiazda.setPoint(0,sf::Vector2f(1,6));
    gwiazda.setPoint(1,sf::Vector2f(19,6));
    gwiazda.setPoint(2,sf::Vector2f(4,18));
    gwiazda.setPoint(3,sf::Vector2f(10,0));
    gwiazda.setPoint(4,sf::Vector2f(16,18));
    gwiazda.setFillColor(sf::Color::Yellow);

    tab=stworz(wys,szer);//piszemy w konwencji tab[wysokosc][szerekosc] czyli tab[2][5] to bedzie element w 3 wierwszy i 6 kolumnie*/
    tab=ramka(tab,wys,szer);

    generuj_jedzenie(tab,wys,szer,rfh,rfw);
    gwiazda.setPosition(fw*20,fh*20);


    sf::RenderWindow window(sf::VideoMode(szer*20, wys*20), "WOLOLOLO");
    sf::RenderWindow& pwindow=window;

    sf::RectangleShape plansza(sf::Vector2f((szer-2)*20,(wys-2)*20));
    plansza.setPosition(border_left,border_up);
    plansza.setFillColor(sf::Color(150,150,150));
    plansza.setOutlineColor(sf::Color::Red);
    plansza.setOutlineThickness(20);

    sf::ConvexShape convex(6);
    convex.setPoint(0, sf::Vector2f(5, 0));
    convex.setPoint(1, sf::Vector2f(15, 0));
    convex.setPoint(2, sf::Vector2f(20, 15));
    convex.setPoint(3, sf::Vector2f(15, 20));
    convex.setPoint(4, sf::Vector2f(5, 20));
    convex.setPoint(5, sf::Vector2f(0, 15));
    convex.setPosition(100,100);
    convex.setFillColor(sf::Color::Green);
    convex.rotate(90);

    sf::CircleShape* kola;
    kola=new sf::CircleShape[3];
    kola[0].setPosition(100,100);
    kola[0].setRadius(30);

    kola[1].setPosition(100,200);
    kola[1].setRadius(30);

    kola[2].setPosition(200,200);
    kola[2].setRadius(30);


    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    cout<<endl;
    system("pause");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();




        }


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

        if(snake.crash(tab,dir)==true)
            {fprintf(plikoutpkt,"%i\n",punkty);// MOZESZ PISAC plikout<<punkty (jak do couta)
            fclose(plikoutpkt);
            return punkty;//sprawdza czy waz nie umrze
            }
        snake.vanish(tab);  //wymazuje starego weza z tablicy
        if(snake.point(tab,dir)==true)
        {
            snake.expandToDir(dir);
            generuj_jedzenie(tab,wys,szer,rfh,rfw);
            gwiazda.setPosition(fw*20,fh*20);
            punkty++;
        }
        else snake.step(dir); // wykonuje krok
        snake.draw(tab); // wpisuje weza po wykonaniu kroku do tablicy
        system("cls");
        wyswietl(tab,wys,szer); //wypisuje tablice na ekran


        window.clear();
        window.draw(plansza);
        window.draw(gwiazda);


        for(int i=0;i<snake.howLong()-1;i++)window.draw(snake.tail[i]);
        window.draw(snake.glowa);
        window.display();
        Sleep(poziom);


    }

    return 0;
}
