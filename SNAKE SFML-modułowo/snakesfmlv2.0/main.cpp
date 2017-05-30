#include "headers.hpp"
#include <cmath>
#include <windows.h>
#include <conio.h>
#include <fstream>

using namespace std;

int main()
{
    int szer, wys, punkty=0,fw=0,fh=0;
    int& rfw=fw;
    int& rfh=fh;
    Kierunek dir=RIGHT;
    Snake snake(5,8,dir,3);//kolejno wys glowy, szer glowy, poczatkowy dir ruchu, dlugosc weza
    char nazwa[50];//przechowuje nazwê gracza
    char** tab;//tablica 2d w której porusza sie snake
    int poziom;//regluje trudnoœæ gry
    FILE*plikoutpkt=fopen("danewyjpkt.txt","a");//zapis do pliku z mo¿liwoœci¹ dopisywania tutaj przechowywane bd kolejno punkty  i imiona
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
    system("cls");
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
        window.clear();
        window.draw(plansza);
        window.draw(gwiazda);


        for(int i=0;i<snake.howLong()-1;i++)
            window.draw(snake.tail[i]);
        window.draw(snake.glowa);
        window.display();
        Sleep(poziom);
    }
    return 0;
}
