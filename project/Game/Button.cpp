#include "Button.h"


Button::Button()
{
    pos.x = 354;
    pos.y = 506;
    pos.w = 314;
    pos.h = 64;
    texture = 0;
}
Button::Button( Texture * texture , std::string str , int x, int y)
{
    texture = 0;
    str = " ";
    x = y = 0;
}

void Button::render ( SDL_Renderer * gRenderer )
{
    pos.x = 1;
    pos.y =2;
    pos.w= 200;
    pos.h = 200;
    texture = Texture::GetInstance(gRenderer); //singelton here, static method
    texture->Render(0,gRenderer,&pos);   //should render first image from sprite at the given pos
}

void Button::setPosition ( int x, int y)
{

}
void Button::setText ( std::string str )
{

}
void Button::Click()
{

}



