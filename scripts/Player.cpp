#include<iostream>

using namespace std;

#include "Player.hpp"
#include "Functions.hpp"

const int walkingFrames = 12;
const int attackingFrames = 10;

Player::Player(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, vector<SDL_Texture*> textures)
:Entity(srcX, srcY, srcW, srcH, desX, desY, desW, desH, textures[0])
{
    girlIdle = textures[0];
    girlWalk = textures[1];
    girlAttack = textures[2];

    movementSpeed = 3;

    movingLeft = movingRight = movingUp = movingDown = false;

    attacking = false;

    frame = 0;

    facingLeft = false;
    movingDirections = 0;

    lastFrameTime = 0;

    float tmp_srcX = 42, tmp_srcY = 55, tmp_srcW = 41, tmp_srcH = 75;
    const float walkingFrameDistance = 128;
    for (int i = 0; i < walkingFrames; i++)
    {
         srcXWalkingFrames[i] = tmp_srcX;
         srcYWalkingFrames[i] = tmp_srcY;
         srcWWalkingFrames[i] = tmp_srcW;
         srcHWalkingFrames[i] = tmp_srcH;

         tmp_srcX += walkingFrameDistance;
    }

    tmp_srcX = 32, tmp_srcY = 55, tmp_srcW = 80, tmp_srcH = 74;
    const float attackingFrameDistance = 128;
    for (int i = 0; i < attackingFrames; i++)
    {
        srcXAttackingFrames[i] = tmp_srcX;
        srcYAttackingFrames[i] = tmp_srcY;
        srcWAttackingFrames[i] = tmp_srcW;
        srcHAttackingFrames[i] = tmp_srcH;

        if (i < attackingFrames - 2)
            tmp_srcX += attackingFrameDistance;
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

void Player::updateFrame(SDL_Texture* tex, float x, float y, float w, float h, bool flip)
{
    setSrcX(x); setSrcY(y);
    setSrcW(w); setSrcH(h);
    setDesW(w); setDesH(h);

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

    if (keyState[SDL_SCANCODE_J] && !attacking)
    {
        attacking = true;
        frame = -1;
    }

    if (attacking)
    {
        if (currentFrameTime - lastFrameTime > 110)
        {
            frame++;
            if (frame == attackingFrames)
                attacking = false;
            lastFrameTime = currentFrameTime;
        }
        frame = max(frame, 0);
        updateFrame(girlAttack, srcXAttackingFrames[frame], srcYAttackingFrames[frame], srcWAttackingFrames[frame], srcHAttackingFrames[frame], facingLeft);
        return;
    }

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
            if (frame == walkingFrames)
                frame = 0;
            lastFrameTime = currentFrameTime;
        }
        updateFrame(girlWalk, srcXWalkingFrames[frame], srcYWalkingFrames[frame], srcWWalkingFrames[frame], srcHWalkingFrames[frame],  facingLeft);
    }
    else
        updateFrame(girlIdle, srcXWalkingFrames[0], srcYWalkingFrames[0], srcWWalkingFrames[frame], srcHWalkingFrames[frame], facingLeft);

    if (movingDirections > 1)
        movementSpeed = 1.8;
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
