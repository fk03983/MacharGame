#include "Plant.h"

Plant::Plant(int x, int y): Container(x,y, PLANT_WIDTH, PLANT_HEIGHT)
{
    spriteNum = rand()%4 + 27; //to choose if plant is watered or not
    SetCovered((bool)(rand()%2));
        //plant image proportion with which image will be rendered
    pos.w = 80;
    pos.h = 150.588;
    water = 0;
}

void Plant::SetCovered(bool status)
{
    spriteNum = spriteNum + (4*(int)status);
    if (status && water != 0)
    {
        delete water;
    }
    else
    {
        water = new CleanWater(0,0);
    }
    Container::SetCovered(status);
}

void Plant::Show(SDL_Renderer* renderer)
{
    Texture::GetInstance()->Render(spriteNum,renderer, &pos);
}

Mosquito* Plant::Breed()
{
    return water->Breed();

}

Plant::~Plant()
{
    //dtor
}
