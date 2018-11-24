#include "Clickable.h"
#include <cstdlib>
#include <cmath>

Clickable::Clickable(int x, int y, int w, int h)
{
    pos.x = x;
    pos.y = y;
    pos.w = w;
    pos.h = h;
}

Clickable::~Clickable()
{

}

bool Clickable::WithinRegion(int x, int y)
{
    return (x >= pos.x && y >= pos.y && x <= pos.x + pos.w && y <= pos.y + pos.h);
}

bool Clickable::Collides(const SDL_Rect& rect)
{
    if (pos.y + pos.h <= rect.y)
    {
        return false;
    }
    if (pos.y >= rect.y + rect.h)
    {
        return false;
    }
    if (pos.x + pos.w <= rect.x)
    {
        return false;
    }
    if (pos.x >= rect.x + rect.w)
    {
        return false;
    }
    return true;
}

bool Clickable::Collides(const int x, const int y, const int w, const int h)
{
    SDL_Rect temp;
    temp.x = x;
    temp.y = y;
    temp.w = w;
    temp.h = h;
    return Collides(temp);
}

void Clickable::ReduceSize(double n)
{
    pos.w = pos.w * n;
    pos.h = pos.h * n;
}


bool Clickable::Collides(const Clickable& obj)
{
    return Collides(obj.pos);
}
void Clickable::UpdatePos(int x,int y)
{
    pos.x = x;
    pos.y = y;
}

int Clickable::GetDistance(int x, int y)
{
    return sqrt((abs(pos.x - x)*abs(pos.x - x))+(abs(pos.y - y)*abs(pos.y - y)));
}
