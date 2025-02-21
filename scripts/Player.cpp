#include<iostream>

using namespace std;

#include "Player.hpp"
#include "Functions.hpp"



Player::Player(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, vector<SDL_Texture*> textures)
:Entity(srcX, srcY, srcW, srcH, desX, desY, desW, desH, textures[0])
{
        girlIdle = textures[0];
        girlWalk = textures[1];

        movementSpeed = 3;

        movingLeft = movingRight = movingUp = movingDown = false;

        frame = 0;

        facingLeft = false;
        movingDirections = 0;

        lastFrameTime = 0;

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

SDL_Rect Player::getChestFrame()
{
    SDL_Rect chestFrame = getDestFrame();
    chestFrame.h -= 50;
    return chestFrame;
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

void Player::updateMovement(vector<Entity> &ObstaclesLower, vector<Entity> &ObstaclesUpper, float currentFrameTime)
{
        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        movingLeft = keyState[SDL_SCANCODE_A];
        movingRight = keyState[SDL_SCANCODE_D];
        movingUp = keyState[SDL_SCANCODE_W];
        movingDown = keyState[SDL_SCANCODE_S];

        movingDirections = 0;
        if (movingLeft)
        {
            movingDirections++;
            facingLeft = true;
        }

        if (movingRight)
        {
            movingDirections++;
            facingLeft = false;
        }

        if (movingUp)
            movingDirections++;

        if (movingDown)
            movingDirections++;

        if (movingUp || movingDown || movingLeft || movingRight)
        {
            if (currentFrameTime - lastFrameTime > 100)
            {
                frame++;
                if (frame > 11)
                    frame = 0;
                lastFrameTime = currentFrameTime;
            }
            updateFrame(girlWalk, srcXFrames[frame], srcYFrames[frame], facingLeft);
        }
        else
            updateFrame(girlIdle, srcXFrames[0], srcYFrames[0], facingLeft);

        if (movingDirections > 1)
            movementSpeed = 1.5;
        else
            movementSpeed = 3;

        float prev_desX = desX;
        float prev_desY = desY;

        moveCharacter();

        if (checkCollision(getLegFrame(), ObstaclesLower) || checkCollision(getChestFrame(), ObstaclesUpper))
        {
                setDesX(prev_desX);
                setDesY(prev_desY);
        }
}
