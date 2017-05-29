#include "headers.hpp"
#include <windows.h>
#include <conio.h>
#include <fstream>

using namespace std;

int main()
{
    int szer, wys, punkty=0;
    Kierunek dir=RIGHT;
    Snake snake(5,8,dir,3);//kolejno wys glowy, szer glowy, poczatkowy dir ruchu, dlugosc weza
    char nazwa[50];
    char** tab;


    FILE*plikout=fopen("danewyj.txt","w");

    cout<<"Witaj w terminalowej grze snake"<<endl;
    cout<<"podaj nazwe gracza"<<endl;
    fgets(nazwa,50,stdin);
    fprintf(plikout,"%s",nazwa);

    cout<<"Podaj szerokosc planszy"<<endl;
    cin>>szer;

    cout<<"Podaj wysokosc planszy"<<endl;
    cin>>wys;


    tab=stworz(wys,szer);//piszemy w konwencji tab[wysokosc][szerekosc] czyli tab[2][5] to bedzie element w 3 wierwszy i 6 kolumnie*/
    tab=ramka(tab,wys,szer);

    generuj_jedzenie(tab,wys,szer);

    system("cls");
    for(;;) //obs³ugi klawiatury nie ruszaæ
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
        if(snake.crash(tab,dir)==true)
        {
            fprintf(plikout,"%i\n\n\n",punkty);// MOZESZ PISAC plikout<<punkty (jak do couta)
            fclose(plikout);
            return punkty;//sprawdza czy waz nie umrze
        }
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
        Sleep(300);

    }

}
