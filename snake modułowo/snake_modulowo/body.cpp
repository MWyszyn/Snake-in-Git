#include "headers.hpp"
using namespace std;

///////////////////
/*  KLASA POINT  */
///////////////////

Point::Point (int w=0, int h=0)
{
    width=w;
    height=h;//zgodnie z konwencja narzucona przez michala pierwsza wsp odpowiada wysokosci a druga szerokosci
}
void Point::MoveTo (int w, int h)
{
    width=w;
    height=h;
}

void Point::MoveToPoint (Point p)
{
    width=p.widthOut();
    height=p.heightOut();
}

bool Point::IsEqual (int w, int h)
{
    if(w==width&&h==height) return true;
    else return false;
}

int Point::widthOut()
{
    return width;
}

int Point::heightOut()
{
    return height;
}


/////////////////////
/*   KLASA SNAKE   */
/////////////////////

Snake::Snake(int w, int h, Kierunek dir, int ll)
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
Snake::~Snake()
{
    delete places;
}

int Snake::howLong ()
{
    return l;
}

void Snake::draw (char **tab)
{
    *(tab[places[0].heightOut()]+places[0].widthOut())='X';
    for(int i=1;i<l;i++) *(tab[places[i].heightOut()]+places[i].widthOut())='o';
}

void Snake::vanish (char **tab)
{
    for(int i=0;i<l;i++) *(tab[places[i].heightOut()]+places[i].widthOut())=' ';
}

void Snake::step (Kierunek dir)
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


void Snake::expandToDir (Kierunek dir)
{

    if(dir!=UP&&dir!=DOWN&&dir!=LEFT&&dir!=RIGHT) return; //sprawdzenie poprawnosci wczytanego kierunku

    Point* temp=new Point[l+1]; //utworzenie nowej wiekszej tablicy na segmenty
    temp[0].MoveToPoint(places[0]); //przekopiowanie pozycji glowy

    for(int i=0;i<l;i++) temp[i+1].MoveToPoint(places[i]); //przekopiowanie wraz z przenumerowaniem elementow (w temp-ie "glowa" wystepuje dwa razy na zerowym i pierwszym miejscu, spokojnie tak ma byæ)

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

bool Snake::crash(char** tab,Kierunek dir)
{
    switch(dir)
    {
        case UP://sprawdza czy waz nie wchodzi w ramkê VVVVVVV                VVV samego siebie z pominiêciem osatatniego segmentu (po ruchu ogona ju¿ tam nie bêdzie)
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
bool Snake::point(char** tab,Kierunek dir)
{
    switch(dir)
    {
        case UP://sprawdza czy waz nie wchodzi w ramkê VVVVVVV                VVV samego siebie z pominiêciem osatatniego segmentu (po ruchu ogona ju¿ tam nie bêdzie)
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


///////////////////////
/* POZOSTALE FUNKCJE */
///////////////////////

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

char **stworz(int wys,int szer)
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

char** ramka(char **tab,int wys, int szer)
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
