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
        float tmp_srcY = srcY;
        const float walkingFrameDistance = 128;
        for (int i = 0; i < 12; i++)
        {
             srcXFrames[i] = tmp_srcX;
             srcYFrames[i] = tmp_srcY;
             tmp_srcX += walkingFrameDistance;
        }
}

SDL_Rect Player::getLegFrame()
{
        SDL_Rect legFrame = getDestFrame();
        legFrame.y += 60;
        legFrame.h = 12;
        return legFrame;
}

void Player::moveCharacter()
{
        if (movingLeft)
            setDesX(getDesX() - movementSpeed);
        if (movingUp)
            setDesY(getDesY() - movementSpeed);
        if (movingRight)
            setDesX(getDesX() + movementSpeed);
        if (movingDown)
            setDesY(getDesY() + movementSpeed);
}

void Player::updateFrame(SDL_Texture* tex, float x, float y, bool flip)
{
        setSrcX(x);
        setSrcY(y);
        if (getTex() != tex)
            setTex(tex);
        if (flip)
            setFlip(SDL_FLIP_HORIZONTAL);
        else
            setFlip(SDL_FLIP_NONE);
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
                updateFrame(girlWalk, srcXFrames[frame], srcYFrames[frame], facingLeft);
            }

            if (movingDirections > 1)
                movementSpeed = 2.5;
            else
                movementSpeed = 5;

            float prev_desX = desX;
            float prev_desY = desY;

            moveCharacter();

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

            moveCharacter();

            if (checkCollision(getLegFrame(), Obstacles))
            {
                setDesX(prev_desX);
                setDesY(prev_desY);
            }
        }

        if (!movingLeft && !movingUp && !movingRight && !movingDown)
            updateFrame(girlIdle, srcXFrames[0], srcYFrames[0], facingLeft);
}
