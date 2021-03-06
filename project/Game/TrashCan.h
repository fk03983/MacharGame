#pragma once
#ifndef TRASHCAN_H
#define TRASHCAN_H


#include "Container.h"
#include "Texture.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Mosquito.h"
#include "TrashCanLid.h"
#include "DoublyLinked.h"

const int CAN_WIDTH = 70;
const int CAN_HEIGHT = 73.234;


class TrashCan : public Container
{
    SDL_Rect ownRect;
    int breedCount;

    public:
        TrashCan(int,int);
        void Show(SDL_Renderer*);
        void SetCovered(bool);
        void HandleEvents(SDL_Event*, Screens_Node&);
        void Update(int);
        Mosquito* Breed();
        void Collision();
        int GetBreedCount();
        ~TrashCan();
        void SetX(int delta, int direction);

};

#endif // TRASHCAN_H
