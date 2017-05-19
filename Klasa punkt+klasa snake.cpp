#include <iostream>

using namespace std;

enum Kierunek
    {
        UP,
        RIGHT,
        DOWN,
        LEFT
    }
    kierunek;

class Point //podstawowa klasa opisuj¹ca punkt x,y w ukl wsp
{
    int width, height;

    public:

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

    void MoveToPoint (Point p)
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
    Point* places; //tablica (dynamicznie alokowana bo ilosc segmentow bedzie sie zwiekszac) zawieraj¹ca punkty w ktorych znajduja sie kolejne segmenty weza
    int l;

    public:

    Snake(int w=5, int h=3, Kierunek dir=RIGHT, int ll=3) //konstruktor tworzy w linii prostej weza z glowa w punkcie w,h ustawionego poczatkowo do rucchu w kirunku dir o dlugosci l
    {
        if(dir!=UP&&dir!=DOWN&&dir!=LEFT&&dir!=RIGHT) return;
        l=ll;
        places=new Point[l];
        switch(dir)
        {
        case UP:

            for(int i=0;i<l;i++) places[i].MoveTo(w+i,h);
            break;

        case DOWN:
            for(int i=1;i<l;i++) places[i].MoveTo(w-i,h);
            break;

        case LEFT:
            for(int i=1;i<l;i++) places[i].MoveTo(w,h+i);
            break;

        case RIGHT:
            for(int i=1;i<l;i++) places[i].MoveTo(w,h-i);
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

    void draw (char **tab)
    {
        for(int i=0;i<l;i++) *(tab[places[i].heightOut()]+places[i].widthOut())='x';// rysuje snake'a w tabeli za pomoca 'x'-ów
    }

    void vanish (char **tab)
    {
        for(int i=0;i<l;i++) *(tab[places[i].heightOut()]+places[i].widthOut())=' ';// wymazuje snake'a (jesli spacja nie odpowiada  za puste pole- do poprawy)
    }

    void step (Kierunek dir)
    {
        if(dir!=UP&&dir!=DOWN&&dir!=LEFT&&dir!=RIGHT) return;
        places[l-1].MoveToPoint(places[0]);
        switch(dir)
        {
        case UP:
            places[0].MoveTo(places[0].widthOut(),places[0].heightOut()-1);
            break;
        case DOWN:
            places[0].MoveTo(places[0].widthOut(),places[0].heightOut()+1);
            break;
        case LEFT:
            places[0].MoveTo(places[0].widthOut()-1,places[0].heightOut());
            break;
        case RIGHT:
            places[0].MoveTo(places[0].widthOut()+1,places[0].heightOut());
            break;
        }
    }

    void expandToDir (Kierunek dir)
    {
        Point* temp=new Point[l+1];
        if(dir!=UP&&dir!=DOWN&&dir!=LEFT&&dir!=RIGHT) return;

        temp[0].MoveToPoint(places[0]);

        for(int i=0;i<l;i++) temp[i+1].MoveToPoint(places[i]);

        switch(dir)
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
        l++;
        places=temp;

    }
};

int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
