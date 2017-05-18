#include <iostream>

using namespace std;

class Point //podstawowa klasa opisuj¹ca punkt x,y w ukl wsp
{
    int width, height;

    public:

    Point (int w, int h)
    {
        width=w;
        height=h;//zgodnie z konwencja narzucona przez michala pierwsza wsp odpowiada wysokosci a druga szerokosci
    }

    void MoveTo (int w, int h)
    {
        width=w;
        height=h;
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
    Point** places; //tablica (dynamicznie alokowana bo ilosc segmentow bedzie sie zwiekszac) zawieraj¹ca punkty w ktorych znajduja sie kolejne segmenty weza
    int l;

    public:

    Snake(int w, int h, Kierunek dir=RIGHT, int ll=3) //konstruktor tworzy w linii prostej weza z glowa w punkcie w,h ustawionego poczatkowo do rucchu w kirunku dir o dlugosci l
    {
        l=ll;
        places=new Point*[l];
        switch(dir)
        {
        case UP:
            for(int i=0;i<l;i++) places[i]=new Point(w+i,h);
            break;

        case DOWN:
            for(int i=1;i<l;i++) places[i]=new Point(w-i,h);
            break;

        case LEFT:
            for(int i=1;i<l;i++) places[i]=new Point(w,h+i);
            break;

        case RIGHT:
            for(int i=1;i<l;i++) places[i]=new Point(w,h-i);
            break;
        }

    }


    ~Snake() //destruktor-zabija snake'a :(
    {
        for(int i=0;i<l;i++) delete places[i];
        delete places;
    }

    int howLong ()
    {
        return l;
    }

    void rysuj (char **tab)
    {
        for(int i=0;i<l;i++)
            tab[][]// do dokonczenia
    }
};

int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
