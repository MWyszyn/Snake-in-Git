#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

enum Kierunek
    {
        UP,
        RIGHT,
        DOWN,
        LEFT
    }
    kierunek;


char **stworz(int wys,int szer)//funkcja tworzy tablice dwuwymiarową o dowolnych rozmiarach i wypelnia ja na pusto
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

void ramka(char **tab,int szer, int wys)//rysuje ramke do okola tablicy o zadanych wymiarach
    {
        for(int i = 0 ; i < szer ; i++)
        {
            tab[0][i]=(char)219;
            tab[wys-1][i]=(char)219;
        }
        for(int i = 1 ; i < (wys-1)  ; i++)
        {
            tab[i][0]=(char)219;
            tab[i][szer-1]=(char)219;

        }
    }

int main()
{
    cout<<"Podaj szerokosc planszy"<<endl;
    int szer;
    cin>>szer;
    cout<<"Podaj wysokoœæ planszy"<<endl;
    int wys;
    cin>>wys;
    char **tab;
    tab=stworz(wys,szer);//piszemy w konwencji tab[wysokosc][szerekosc] czyli tab[2][5] to bedzie element w 3 wierwszy i 6 kolumnie
}
