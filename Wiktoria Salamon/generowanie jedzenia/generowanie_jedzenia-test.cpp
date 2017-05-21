#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

char** generuj_jedzenie (char** plansza, int wysokosc,int szerokosc)
{

    int x,y;
    x=rand()% (wysokosc-2)+1; //pomijamy ramke
    y=rand()% (szerokosc-2)+1;

    plansza[x][y]='X';

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
    srand (time (NULL));
    cout<<"Podaj szerokosc planszy"<<endl;
    int szer;
    cin>>szer;
    cout<<"Podaj wysokosc planszy"<<endl;
    int wys;
    cin>>wys;
    char **tab;
    tab=stworz(wys,szer);
    tab=ramka(tab,wys,szer);
    wyswietl(tab,wys,szer);
    cout <<endl;

    for (int i=0;i<5;i++)
    {
        tab=generuj_jedzenie(tab,wys,szer);
        wyswietl(tab,wys,szer);
        cout << endl <<endl;
    }
    return 0;

}
