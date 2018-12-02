#include "Hospital.h"
#include <random>
#include <iostream>

using namespace std;

Hospital:: Hospital()
{
    int humanPos[8] = {340,427,523,610,705,792,885,972};

    for (int i = 0; i < 8; i++)
    {
        this->humanPos[i] = humanPos[i];
    }
    pos.x = 0;
    pos.y  = 0;
    pos.w  = 1024;
    pos.h = 786;

    btn = new Button[2];
    btn[0].SetButtonSprite(false);
    btn[0].setPosition(280,80);
    btn[0].SetWidth(315*1.5,70*0.8);
    btn[0].setText("SEE PATIENT");
    btn[0].word->ReduceSize(0.8);

    btn[1].setPosition(800,60);
    btn[1].SetWidth(200,55);
    btn[1].setText("OUTDOOR");

    upperRect = new SDL_Rect;

    upperRect->x = 0;
    upperRect-> y =0;
    upperRect->x = 1240;
    upperRect->h = 55;

    manualShow = false;

}

void Hospital::Show(SDL_Renderer* renderer)
{
    Texture::GetInstance()->Render(113, renderer, &pos);
    for(int i = 0; i<humans.GetLength(); i++)
     {
         humans.GiveItem(i)->Show(renderer);
     }

     if(!manualShow)
     {
         btn[0].Show(renderer);
     }
     btn[1].Show(renderer);
     SDL_SetRenderDrawColor( renderer, 2,85,89,0 );
    SDL_RenderDrawRect(renderer,upperRect);
    SDL_RenderFillRect(renderer,upperRect);
    points->Show(renderer);
}

void Hospital::HandleEvents(SDL_Event* e,Screens_Node& node)
{
    int hoverX = e->button.x;
    int hoverY = e->button.y;
    if (e->type == SDL_QUIT)
    {
        SDL_Quit();
    }
    if( e->type == SDL_KEYDOWN )
    {

        if(e->key.keysym.sym == SDLK_ESCAPE)    //will open pause menu
        {
//            node.cur_screen = new PauseMenu(outdoor);  //EVENT TO BE DEALT
//            node.prev_screen = this;
//            node.prev_updatable = false;
//            node.prev_backable = true;

        }
    }
    if( btn[0].WithinRegion(hoverX,hoverY)==true)
    {
        btn[0].Hover();
        if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
        {
            node.cur_screen = new Manual(235,70);
            node.prev_screen = this;
            node.prev_backable = true;
            node.prev_updatable = true;
            manualShow = true;
        }

    }

    else
    {
        btn[0].SetSprite2(123);
    }

    if (btn[1].WithinRegion(hoverX,hoverY))  //for outdoor button in house
    {
        if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
        {
            node.cur_screen = node.prev_screen;
            node.prev_screen = this;
        }
    }

}

void Hospital::Update(int frame)
{

}

Hospital :: ~Hospital()
{
    delete[] btn;
}

 bool Hospital :: AddHuman(Human* human)
 {
     bool allowAppend = false;
     if(humans.GetLength()<=8) //only 8 humans allowed in hospital
     {
         allowAppend = true;
     }
     if(allowAppend==true)
     {
         humans.Append(human);

         for(int i = 0; i<humans.GetLength(); i++)
         {
             humans.GiveItem(i)->UpdatePos(humanPos[i],570);
         }
     }
     return allowAppend;
 }

void Hospital :: LeaveHuman(Human* human)
{
      humans.Pop();
      for(int i = 0; i<humans.GetLength(); i++)
     {
         humans.GiveItem(i)->UpdatePos(humanPos[i],570);
     }
}

