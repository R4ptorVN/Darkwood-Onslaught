#include<iostream>

using namespace std;

#include "Player.hpp"



Player::Player(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, vector<SDL_Texture*> textures)
:Entity(srcX, srcY, srcW, srcH, desX, desY, desW, desH, textures[0])
{
        girlIdle = textures[0];
        girlWalk = textures[1];

        movementSpeed = 5;

        movingLeft = movingRight = movingUp = movingDown = false;

        frame = 0;

        facingLeft = false;
        movingDirections = 0;

        float tmp_srcX = srcX;
        const float walkingFrameDistance = 128;
        for (int i = 0; i < 12; i++)
        {
             srcXFrames[i] = tmp_srcX;
             tmp_srcX += walkingFrameDistance;
        }
}

SDL_Rect Player::getLegFrame()
{
        SDL_Rect legFrame = getDestFrame();
        legFrame.y += 50;
        legFrame.h = 22;
        return legFrame;
}

bool checkCollision(SDL_Rect a, vector<Entity> &Obstacles)
{
        for (Entity object : Obstacles)
        {
            SDL_Rect b = object.getDestFrame();
            if (SDL_HasIntersection(&a , &b))
                return true;
        }
        return false;
}

void Player::updateMovement(SDL_Event &event, vector<Entity> &Obstacles)
{
        if(event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_a:
                {
                    if (!movingLeft)
                        movingDirections++;
                    facingLeft = true;
                    movingLeft = true;
                    break;
                }

                case SDLK_w:
                {
                    if (!movingUp)
                        movingDirections++;
                    movingUp = true;
                    break;
                }

                case SDLK_d:
                {
                    if (!movingRight)
                        movingDirections++;
                    facingLeft = false;
                    movingRight = true;
                    break;
                }

                case SDLK_s:
                {
                    if (!movingDown)
                        movingDirections++;
                    movingDown = true;
                    break;
                }
            }
            if (movingUp || movingDown || movingLeft || movingRight)
            {
                frame++;
                if (frame > 11)
                    frame = 0;
                setSrcX(srcXFrames[frame]);
                if (getTex() != girlWalk)
                    setTex(girlWalk);
                if (facingLeft)
                    setFlip(SDL_FLIP_HORIZONTAL);
                else
                    setFlip(SDL_FLIP_NONE);
            }
            if (movingDirections > 1)
                movementSpeed = 2.5;
            else
                movementSpeed = 5;

            float prev_desX = desX;
            float prev_desY = desY;

            if (movingLeft)
                setDesX(getDesX() - movementSpeed);
            if (movingUp)
                setDesY(getDesY() - movementSpeed);
            if (movingRight)
                setDesX(getDesX() + movementSpeed);
            if (movingDown)
                setDesY(getDesY() + movementSpeed);

            if (checkCollision(getLegFrame(), Obstacles))
            {
                setDesX(prev_desX);
                setDesY(prev_desY);
            }
        }

        if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_a:
                {
                    movingLeft = false;
                    movingDirections--;
                    break;
                }

                case SDLK_w:
                {
                    movingUp = false;
                    movingDirections--;
                    break;
                }

                case SDLK_d:
                {
                    movingRight = false;
                    movingDirections--;
                    break;
                }

                case SDLK_s:
                {
                    movingDown = false;
                    movingDirections--;
                    break;
                }
            }

            float prev_desX = desX;
            float prev_desY = desY;

            if (movingLeft)
                setDesX(getDesX() - movementSpeed);
            if (movingUp)
                setDesY(getDesY() - movementSpeed);
            if (movingRight)
                setDesX(getDesX() + movementSpeed);
            if (movingDown)
                setDesY(getDesY() + movementSpeed);

            if (checkCollision(getLegFrame(), Obstacles))
            {
                setDesX(prev_desX);
                setDesY(prev_desY);
            }
        }
        if (!movingLeft && !movingUp && !movingRight && !movingDown)
        {
            if (getTex() != girlIdle)
            {
                setTex(girlIdle);
                setSrcX(srcXFrames[0]);
            }

            if (facingLeft)
                setFlip(SDL_FLIP_HORIZONTAL);
            else
                setFlip(SDL_FLIP_NONE);
        }
}
