#pragma once
#include <SDL.h>
#include <SDL_image.h>

class Screens;
class MainMenu;


struct Screens_Node
{
    Screens* prev_screen;
    Screens* cur_screen;
    Screens* next_screen;
    bool prev_backable;
    bool prev_updatable;

    Screens_Node()
    {
        prev_screen = cur_screen = next_screen = 0;
        prev_backable = prev_updatable = false;
    }

};

class Screens
{
protected:
    static Screens* curScreen;
    Screens* prevScreen;
    bool prevShowable;
    bool prevUpdateable;
    int prevUpdateFactor;

public:
    Screens(Screens*, bool, bool = false, bool = false, int = 1);
    static Screens*& GetCurrent(){return curScreen;}
    static void SetCurrent(Screens* scr){curScreen = scr;}
    static void Initiate();
    //virtual void Click(SDL_Renderer*) = 0;
    virtual void Show(SDL_Renderer*) = 0;
    virtual void Update(int) = 0;
    virtual void HandleEvents(SDL_Event*, Screens_Node&) = 0;
    virtual ~Screens();
};


