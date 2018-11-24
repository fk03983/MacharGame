#pragma once

#include <SDL.h>
#include <SDL_image.h>


class Clickable
{
    public:
        Clickable(int,int,int,int);
        bool WithinRegion(int, int);
        void UpdatePos(int,int);
        bool Collides(const SDL_Rect&);
        bool Collides(const int, const int, const int, const int);
        bool Collides(const Clickable&);
        static bool Collides(const SDL_Rect&, const SDL_Rect&);
        void ReduceSize(double);
        virtual ~Clickable();
        int GetDistance(int,int);
        void GetCenter(int&, int&);

    protected:
        SDL_Rect pos;



    private:
};
