#include<iostream>

using namespace std;

#include "lib/Player.hpp"
#include "lib/Functions.hpp"

const int idleFrames = 12;
const int walkingFrames = 8;
const int attackingFrames = 6;

const float frameDistance = 64;

Player::Player(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex)
:Entity(srcX, srcY, srcW, srcH, desX, desY, desW, desH, tex)
{

    movementSpeed = 2.5;

    healthPoints = 100;

    movingLeft = movingRight = movingUp = movingDown = false;

    attacking = false;

    frame = 0;

    movingDirection = 0;
    movingDirections = 0;

    lastFrameTime = 0;

    srcXFrames[0] = 26;
    for (int i = 1; i < 20; i++)
        srcXFrames[i] = srcXFrames[i - 1] + frameDistance;

    srcYIdleFrames[0] = 109;
    srcYIdleFrames[1] = 493;
    srcYIdleFrames[2] = 877;
    srcYIdleFrames[3] = 1261;

    srcYWalkingFrames[0] = 237;
    srcYWalkingFrames[1] = 622;
    srcYWalkingFrames[2] = 1005;
    srcYWalkingFrames[3] = 1389;

    srcYAttackingFrames[0] = 365;
    srcYAttackingFrames[1] = 749;
    srcYAttackingFrames[2] = 1133;
    srcYAttackingFrames[3] = 1517;

}

SDL_Rect Player::getHitBox()
{
    SDL_Rect hitBox = getDestFrame();

    hitBox.y -= (29 * 2);
    hitBox.h = (32 * 2);

    return hitBox;
}

SDL_Rect Player::getRenderBoxValues()
{
    SDL_Rect renderBox;

    renderBox.x = -26;
    renderBox.y = -45;
    renderBox.h = 64;
    renderBox.w = 64;

    return renderBox;
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

void Player::updateFrame(float x, float y)
{
    setSrcX(x);
    setSrcY(y);
}

bool Player::outOfMap()
{
     SDL_Rect dest = getDestFrame();
     if (dest.x < 0 || dest.y < 0 || dest.x + dest.w > LEVEL_WIDTH || dest.y + dest.h > LEVEL_HEIGHT)
         return true;
    return false;
}

void Player::updatePlayerMovement(vector<Entity> &Obstacles, float currentFrameTime)
{
    const Uint8* keyState = SDL_GetKeyboardState(NULL);

    movingLeft = keyState[SDL_SCANCODE_A];
    movingRight = keyState[SDL_SCANCODE_D];
    movingUp = keyState[SDL_SCANCODE_W];
    movingDown = keyState[SDL_SCANCODE_S];

    if (healthPoints < 100)
        healthPoints += 0.01;

    if (keyState[SDL_SCANCODE_J] && !attacking)
    {
        attacking = true;
        frame = 0;
    }

    if (attacking)
    {
        updateFrame(srcXFrames[frame], srcYAttackingFrames[movingDirection]);
        if (currentFrameTime - lastFrameTime > 100)
        {
            frame++;
            if (frame == attackingFrames)
                attacking = false;

            lastFrameTime = currentFrameTime;
        }
        return;
    }

    movingDirections = 0;

    if (movingDown)
    {
        movingDirection = 1;
        movingDirections++;
    }

    if (movingUp)
    {
        movingDirection = 3;
        movingDirections++;
    }

    if (movingLeft)
    {
        movingDirection = 2;
        movingDirections++;
    }

    if (movingRight)
    {
        movingDirection = 0;
        movingDirections++;
    }

    if (movingDirections > 0)
    {
        if (currentFrameTime - lastFrameTime > 100)
        {
            frame++;
            if (frame == walkingFrames)
                frame = 0;

            lastFrameTime = currentFrameTime;
        }
        updateFrame(srcXFrames[frame], srcYWalkingFrames[movingDirection]);
    }
    else
    {
        if (currentFrameTime - lastFrameTime > 100)
        {
            frame++;
            if (frame == walkingFrames)
                frame = 0;
            lastFrameTime = currentFrameTime;
        }
        updateFrame(srcXFrames[frame], srcYIdleFrames[movingDirection]);
    }

    if (movingDirections > 1)
        movementSpeed = 1.5;
    else
        movementSpeed = 2.5;

    float prev_desX = desX;
    float prev_desY = desY;

    moveCharacter();

    if (checkCollision(getDestFrame(), Obstacles) || outOfMap())
    {
            setDesX(prev_desX);
            setDesY(prev_desY);
    }
}
