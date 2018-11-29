#include "Human.h"
#include <random>
#include <cmath>
#include "House.h"


Human::Human(): Clickable(0,0,197, 575)
{
    ownHouse = 0;
}


Human::Human(int x, int y, House* house): Clickable(x,y,197, 570)
{
    spriteNum = 74;
    ownHouse  =  house;
    currentScenario = house;
    sizeFactor = 0.3;
    ReduceSize(sizeFactor);

    isIndoor = true;
    faceDirection = RIGHT;
    isGoingToBed = true;
    isGoingOut = false;
    isVertical = false;
    isHorizontal = true;
    isWalking = false;
    activity = WALKING;
    timeSince = 0;
    step = 1;
    slowDownFactor = 3;
    isInfected = false;
    door = ownHouse->GetDoor();
    faceSprite = 86;
    bodySprite = 83;
    legSprite = 103;
    walker = 0;
    BuildHuman();


}


void Human::ChangeScenario(Scenario* scenario)
{
    currentScenario = scenario;
}



void Human::BuildHuman()
{
    face.x = pos.x;
    face.y = pos.y;
    face.w = 193*sizeFactor;
    face.h = 124*sizeFactor;

    body.x = pos.x;
    body.y = face.y + face.h - (10*(3*sizeFactor));
    body.w = 193 * sizeFactor;
    body.h = 268 * sizeFactor;

    legs.x = pos.x+10;
    legs.y = body.y + body.h - (10*(3*sizeFactor));
    legs.w = 177*sizeFactor;
    legs.h = 250*sizeFactor;
    collideRect.x = pos.x + (10*(3*sizeFactor));
    collideRect.y = legs.y + legs.h - 10;
    collideRect.w = legs.w;
    collideRect.h = 10;

}

void Human::HandleEvents(SDL_Event* e, Screens_Node&)
{
    int x = e->button.x;
    int y = e->button.y;
    if (WithinRegion(x,y) && e->type == SDL_MOUSEBUTTONDOWN)
    {
        ownHouse->GetOutdoor()->hospital->AddHuman(this);
        ChangeState(IN_HOSPITAL);
    }

    return;
}

Human::Human(House* house): Human(0,0, house)
{

}


bool Human::Collide(SDL_Rect& tempRect)
{
    int n;
    if (isIndoor)
    {

        Bed* bed = ownHouse->GetBeds(n);
        for (int i = 0; i <n; i++)
        {
            if (bed[i].Collides(tempRect))
            {
                return true;
            }
        }
    }
    BreedingGround** breedingGrounds = currentScenario->GetBreedingGrounds(n);
    for (int i = 0; i < n; i++)
    {
        if (breedingGrounds[i]->Collides(tempRect))
        {
            return true;
        }
    }
    DLL <Human*> humans = currentScenario->GetHumans(n);
    for (int i = 0; i < n; i++)
    {
        if (humans.GiveItem(i)->Collides(tempRect, humans.GiveItem(i)->collideRect) && humans.GiveItem(i) != this)
        {
            return true;
        }
    }
    return false;
}

void Human::Update(int frame)
{
    if (frame % slowDownFactor == 0)
    {

        walker += 0.02;
        if (walker > 10)
        {
            walker = 0;
        }
        switch (activity)
        {

            case (LYING):
            {
                timeSince++;
                if (timeSince > 2000)
                {
                    ChangeState();
                }
                break;
            }

            case SITTING:
            {
                timeSince++;
                if (timeSince > 2000)
                {
                    bedToGoTo->SetOccupied(false);
                    ChangeState();
                }
                break;
            }

            case GOING_TO_BED:
            {
                if (!bedToGoTo->GetOccupied())
                {
                    if (toFollowX > collideRect.x && isHorizontal)
                    {
                        faceDirection = RIGHT;
                    }
                    else if(toFollowX < collideRect.x && isHorizontal)
                    {
                        faceDirection = LEFT;
                    }
                    else
                    {
                        isHorizontal = !isHorizontal;
                        faceDirection = UP;
                    }
                    if (toFollowY < collideRect.y && !isHorizontal)
                    {
                        faceDirection = UP;

                    }
                    if (bedToGoTo->Collides(collideRect) || bedToGoTo->Collides(legs))
                    {
                        isHorizontal = true;
                        spriteNum = 75;
                        bedToGoTo->SetOccupied(true);
                        if (!isInfected)
                        {
                            ChangeState(SITTING);
                        }
                    }
                    Move();
                }
                else
                {
                    ChangeBedToFollow();
                }
                break;
            }
            case GOING_TO_DOOR:
            {
                    if (toFollowX > collideRect.x && isHorizontal)
                    {
                        faceDirection = RIGHT;
                    }
                    else if(toFollowX < collideRect.x && isHorizontal)
                    {
                        faceDirection = LEFT;
                    }
                    else
                    {
                        isHorizontal = false;
                        faceDirection = UP;
                    }
                    if (toFollowY < collideRect.y && !isHorizontal)
                    {
                        faceDirection = UP;

                    }
                    if (door->Collides(collideRect) || door->Collides(legs))
                    {
                        spriteNum = 76;
                        ChangeState(LYING);
                        isHorizontal = true;
                        if (!isIndoor)
                        {
                            GoIndoor();
                        }
                        else
                        {
                            GoOutdoor();
                        }

                    }
                Move();
                break;
            }


            case WALKING:
            {
                if (timeSince*step > (currentScenario->GetWidth())*2)
                {
                    ChangeState();
                }
                if (collideRect.x >= currentScenario->GetWidth())
                {
                    faceDirection = LEFT;
                }
                else if (collideRect.x <= 0)
                {
                    faceDirection = RIGHT;
                }
                Move();
                timeSince++;
                break;
            }
            case AVOIDING_COLLISION:
            {
                Move();
                timeSince++;
                if (timeSince>10)
                {
                    ChangeState();
                }
                break;
            }
        }
    }

}


void Human::ChangeDirection()
{
    switch (faceDirection)
    {
        case (UP):
        {

            if( rand()%2 == 0)
            {
                faceDirection = RIGHT;
            }
            else
            {
                faceDirection = LEFT;
            }
            break;
        }
        case (RIGHT):
        {
            if (rand()%2 == 0)
            {
                faceDirection = DOWN;
            }
            else
            {
                faceDirection = UP;
            }
            break;
        }
        case (DOWN):
        {
            if (rand()%2 == 0)
            {
                faceDirection = DOWN;
            }
            else
            {
                faceDirection = RIGHT;
            }
            break;
        }
        case (LEFT):
        {
            if (rand()%2 == 0)
            {
                faceDirection = UP;
            }
            else
            {
                faceDirection = DOWN;
            }
            break;
        }
    }
}

void Human::Move()
{
    if (!MoveAllowed())
    {
        if (activity != AVOIDING_COLLISION)
        {
            myStack.Append(activity);
        }
        ChangeDirection();
        timeSince = 0;
        activity = AVOIDING_COLLISION;
    }
    else
    {

        if (faceDirection == RIGHT)
        {

            pos.x += step;
            collideRect.x += step;
            face.x += step;
            body.x += step;
            legs.x += step;

        }
        else if (faceDirection == LEFT)
        {
            pos.x -= step;
            collideRect.x -= step;
            face.x -= step;
            body.x -= step;
            legs.x -= step;

        }
        else if (faceDirection == DOWN)
        {
            pos.y += step;
            collideRect.y += step;
            face.y += step;
            body.y += step;
            legs.y += step;

        }
        else if (faceDirection == UP)
        {
            pos.y -= step;
            collideRect.y -= step;
            face.y -= step;
            body.y -= step;
            legs.y -= step;

        }

    }

}

void Human::ChangeState(int n)
{
    timeSince = 0;
    if (!myStack.IsEmpty())
    {
        activity = myStack.Pop();
    }
    else if (n != -1)
    {
        activity = n;
    }
    else
    {
        activity = rand()%3;
        switch (activity)
        {
            case (WALKING):
            {
                break;
            }
            case (GOING_TO_BED):
            {
                if (!isIndoor)
                {
                    myStack.Append(GOING_TO_BED);
                    activity = GOING_TO_DOOR;
                }
            }
            case (GOING_TO_DOOR):
            {
                break;
            }
        }
    }

    if (activity == GOING_TO_BED)
    {
        ChooseBed();
    }
    else if (activity == GOING_TO_DOOR)
    {

        ChooseDoor();
    }

}

void Human::ChangeBedToFollow()
{
    ChooseBed();
}

void Human::ChooseBed()
{
    int n;
    Bed* myBeds = ownHouse->GetBeds(n);
    for (int i = 0; i < n; i++)
    {
        if (!myBeds[i].GetOccupied())
        {
            myBeds[i].GetCenter(toFollowX, toFollowY);
            bedToGoTo = &myBeds[i];
            return;
        }
    }

}

bool Human::MoveAllowed()
{
    SDL_Rect tempRect = collideRect;
    switch (faceDirection)
    {
        case (RIGHT):
        {
            tempRect.x++;
            if (tempRect.x > currentScenario->GetWidth())
            {
                return false;
            }
            break;
        }
        case (LEFT):
        {
            tempRect.x--;
            if (tempRect.x < 0)
            {
                return false;
            }
            break;
        }
        case (UP):
        {
            tempRect.y--;
            if (tempRect.y <= currentScenario->GetHeight())
            {
                return false;
            }
            break;
        }
        case (DOWN):
        {
            tempRect.y++;
            if (tempRect.y >= 786)
            {
                return false;
            }
            break;
        }

    }
    if (Collide(tempRect))
    {
        return false;
    }
    return true;
}

void Human::ChooseDoor()
{
    if (isIndoor)
    {
        door->GetCenter(toFollowX, toFollowY);
    }
    else
    {
        door->OutdoorPosCenter(toFollowX, toFollowY);
    }

}

void Human::Show(SDL_Renderer* renderer)
{
    if (activity == WALKING || activity == GOING_TO_BED || activity == GOING_TO_DOOR)
    {
        int face = 0;
        int body = 0;
        int leg = 0;
        bool flipped = false;

        if (faceDirection == UP)
        {
            face = 85;
            leg = 93;
            body = 82;
        }
        else if (faceDirection == RIGHT)
        {
            face = 86;
            leg = 103;
            body = 83;
            flipped = false;
        }
        else if (faceDirection == LEFT)
        {
            face = 86;
            leg = 103;
            body = 83;
            flipped = true;
        }
        else if (faceDirection == DOWN)
        {
            face = 84;
            leg = 93;
            body = 81;
        }
        if (!flipped)
        {
            Texture::GetInstance()->Render(face,renderer, &this->face);
            Texture::GetInstance()->Render((int)(leg+ walker), renderer, &legs);
            Texture::GetInstance()->Render(body,renderer, &this->body);
        }
        else
        {
            Texture::GetInstance()->RenderFlipped(face,renderer, &this->face);
            Texture::GetInstance()->RenderFlipped((int)(leg+ walker), renderer, &legs);
            Texture::GetInstance()->RenderFlipped(body,renderer, &this->body);
        }

    }
    else if (activity == SITTING)
    {
        Texture::GetInstance()->Render(spriteNum, renderer, &pos);
    }

    else if (activity == IN_HOSPITAL)
    {
        Texture::GetInstance()->Render(76, renderer, &pos);
    }
    SDL_SetRenderDrawColor( renderer, 170, 170, 170, 0);
    SDL_RenderDrawRect(renderer, &collideRect);
}


void Human::GoIndoor()
{
    isIndoor = true;

}

void Human::GoOutdoor()
{
    isIndoor = false;
    ownHouse->GetOutdoor()->AddHuman(this);
    ChangeScenario(ownHouse->GetOutdoor());
    door->OutdoorPosCenter(pos.x, pos.y);
    sizeFactor = 0.2;
    ReduceSize(sizeFactor);
    BuildHuman();

    ChangeState(WALKING);

}

bool Human::GetIndoor()
{
    return isIndoor;
}


void Human::SetX(int delta, int direction)
{
    if ( direction == 0)
    {
        collideRect.x+=delta;
        face.x += delta;
        body.x += delta;
        legs.x += delta;
    }
    if ( direction == 1)
    {
        collideRect.x -=delta;
        face.x -= delta;
        body.x -= delta;
        legs.x -= delta;
    }
    Clickable::SetX(delta, direction);
}

void Human::SetPos(int x, int y)
{
    pos.x = x;
    pos.y = y;
    BuildHuman();
}
