#include <iostream>

using namespace std;

class Point
{
    int x,y;
    Point (int xx, int yy)
    {
        x=xx;
        y=yy;
    }

    public

    void MoveTo (int xx, int yy)
    {
        x=xx;
        y=yy;
    }
    bool IsEqual (int xx, int yy)
    {
        if(xx==x&&yy==y) return true;
        else return false;
    }
};

class Snake
{
    Point** places;
    int l;

    public

    Snake(int xx, int yy, char dir='w'/* zmienic na enum!*/, int ll=3)
    {
        l=ll;
        places=new Point*[l];
        places[0]=new Point(xx,yy)
            switch(dir)
            {
            case n:
                for(int i=1;i<l;i++)
                {
                    places[i]=new Point(xx,yy+i)
                }
            case s:
                for(int i=1;i<l;i++)
                {
                    places[i]=new Point(xx,yy-i)
                }
            case w:
                for(int i=1;i<l;i++)
                {
                    places[i]=new Point(xx+,yy)
                }
            case e:
                for(int i=1;i<l;i++)
                {
                    places[i]=new Point(xx-i,yy-i)
                }
            }

        }
    }

    ~Snake
    {
        for(int i=0;i<l;i++)
        {
            delete places(i);
        }
        delete places;
    }
};

int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
