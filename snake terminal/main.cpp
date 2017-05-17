#include <iostream>

using namespace std;


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
            tab[0][i]=4;
            tab[wys-1][i]=4;
        }
        for(int i = 1 ; i < (wys-1)  ; i++)
        {
            tab[i][0]=4;
            tab[i][szer-1]=4;

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
    int i; int j;

    ramka(tab,szer,wys);
    for(i = 0 ; i < wys ; i++ )
        {
            for ( j = 0 ; j < szer ; j++)
            {
              cout<<tab[i][j];
            }
            cout<<endl;
        }

}
