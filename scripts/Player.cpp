#include<iostream>

using namespace std;

#include "lib/Player.hpp"
#include "lib/Functions.hpp"


const float frameDistanceX = 48;
const float frameDistanceY = 240;

float srcXFrames[20];

float srcYFrames[5][4];

int maxFrames[5];

SDL_Rect swordFrame[4][2];

SDL_Rect skillFrame[4];

int maxHP = 100;

SDL_Rect healthBar;
SDL_Rect manaBar;

Player::Player(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex)
:Entity(srcX, srcY, srcW, srcH, desX, desY, desW, desH, tex)
{

    movementSpeed = 2.5;

    healthPoints = maxHP;
    manaPoints = 0;

    attackingDamage = 12;

    healthBar.x = 0; healthBar.y = 0;
    healthBar.w = 100; healthBar.h = 14;

    manaBar.x = 0; manaBar.y = 0;
    manaBar.w = 100; manaBar.h = 14;

    movingLeft = movingRight = movingUp = movingDown = false;

    attackingState = 0;

    frame = 0;

    frameDuration = 0;

    state = 0;

    textureState = 0;

    movingDirection = 0;
    movingDirections = 0;

    lastFrameTime = 0;

    frameDistance[0] = 100;
    frameDistance[1] = 100;
    frameDistance[2] = 100;
    frameDistance[3] = 115;
    frameDistance[4] = 100;

    maxFrames[0] = 5;
    maxFrames[1] = 8;
    maxFrames[2] = 6;
    maxFrames[3] = 6;
    maxFrames[4] = 5;

    srcXFrames[0] = 20;
    for (int i = 1; i < 20; i++)
        srcXFrames[i] = srcXFrames[i - 1] + frameDistanceX;

    srcYFrames[0][0] = 85;
    srcYFrames[1][0] = 37;
    srcYFrames[2][0] = 133;
    srcYFrames[3][0] = 181;
    srcYFrames[4][0] = 229;
    for (int i = 0; i < 5; i++)
        for (int j = 1; j < 4; j++)
             srcYFrames[i][j] = srcYFrames[i][j - 1] + frameDistanceY;

    swordFrame[0][0] = makeRec(12, -37, 16, 48);
    swordFrame[0][1] = makeRec(-20, -6, 48, 16);
    swordFrame[1][0] = makeRec(-20, -6, 48, 16);
    swordFrame[1][1] = makeRec(-20, -37, 16, 48);
    swordFrame[2][0] = makeRec(-20, -37, 16, 48);
    swordFrame[2][1] = makeRec(-20, -37, 48, 16);
    swordFrame[3][0] = makeRec(-20, -37, 48, 16);
    swordFrame[3][1] = makeRec(-12, -37, 16, 48);

    skillFrame[0] = makeRec(-68, -85, 71, 77);
    skillFrame[1] = makeRec(3, -85, 73, 77);
    skillFrame[2] = makeRec(-68, -8, 71, 83);
    skillFrame[3] = makeRec(3, -8, 73, 83);
}

SDL_Texture* playerTexture[2];

Player setupPlayerTexture(RenderWindow& window)
{
    playerTexture[0] = window.loadTexture("resources/Player/knight.png");
    playerTexture[1] = window.loadTexture("resources/Player/knightDamaged.png");

    Player player(20, 85, 8, 4, 300, 500, 8 * 1.75, 4 * 1.75, playerTexture[0]);

    return player;
}

float Player::getHealthPoints()
{
    return healthPoints;
}

void Player::setHealthPoints(float x)
{
    if (x < 0)
        healthPoints = 0;
    else
        healthPoints = x;
}

void Player::increaseMana()
{
     if (manaPoints < 20)
         manaPoints++;
}

bool Player::checkDeath()
{
    if (healthPoints <= 0)
        return true;
    return false;
}

void Player::setStateTexture(int x)
{
    textureState = x;
}

int Player::getAttackingState()
{
    return attackingState;
}

float Player::getAttackingDamage()
{
    return attackingDamage;
}

void Player::levelUp()
{
     maxHP += 5;
     healthPoints = maxHP;
     attackingDamage += 3;
}

SDL_Rect Player::getHealthBar()
{
    healthBar.w = (healthPoints / maxHP * 100);
    return healthBar;
}

SDL_Rect Player::getManaBar()
{
    healthBar.w = (manaPoints / 20 * 100);
    return healthBar;
}

SDL_Rect Player::getHitBox()
{
    SDL_Rect hitBox = getDestFrame();

    hitBox.y -= (25 * 1.75);
    hitBox.h = (26 * 1.75);

    return hitBox;
}

SDL_Rect Player::getRenderBoxValues()
{
    SDL_Rect renderBox;

    renderBox.x = -20;
    renderBox.y = -37;
    renderBox.h = 48;
    renderBox.w = 48;

    return renderBox;
}

SDL_Rect Player::getSwordBox(int box)
{
    SDL_Rect swordBox = getDestFrame();

    swordBox.x += (swordFrame[movingDirection][box].x * 2);
    swordBox.y += (swordFrame[movingDirection][box].y * 2);
    swordBox.w = swordFrame[movingDirection][box].w * 2;
    swordBox.h = swordFrame[movingDirection][box].h * 2;

    return swordBox;
}

SDL_Rect Player::getSkillBox(int box)
{
    SDL_Rect skillBox = getDestFrame();

    skillBox.x += (skillFrame[box].x * 2);
    skillBox.y += (skillFrame[box].y * 2);
    skillBox.w = skillFrame[box].w * 2;
    skillBox.h = skillFrame[box].h * 2;

    return skillBox;
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

void Player::updatePlayerMovement(vector<Entity> &Obstacles, float currentFrameTime, bool &gameRunning)
{
    const Uint8* keyState = SDL_GetKeyboardState(NULL);

    setTex(playerTexture[textureState]);

    if (frameDuration == 0)
    {
        if (checkDeath())
        {
            state = 4;
            frameDuration = maxFrames[state] + 5;

            frame = 0;
            updateFrame(srcXFrames[frame], srcYFrames[state][movingDirection]);
            if (movingDirection == 2)
                setFlip(SDL_FLIP_HORIZONTAL);
        }
        else if (keyState[SDL_SCANCODE_K] && manaPoints == 20)
        {
            manaPoints = 0;

            state = 3;
            frameDuration = maxFrames[state];

            frame = 0;
            updateFrame(srcXFrames[frame], srcYFrames[state][movingDirection]);
        }
        else if (keyState[SDL_SCANCODE_J])
        {
            state = 2;
            frameDuration = maxFrames[state];

            frame = 0;
            updateFrame(srcXFrames[frame], srcYFrames[state][movingDirection]);
        }
        else
        {
            movingLeft = keyState[SDL_SCANCODE_A];
            movingRight = keyState[SDL_SCANCODE_D];
            movingUp = keyState[SDL_SCANCODE_W];
            movingDown = keyState[SDL_SCANCODE_S];

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
                state = 1;
            else
                state = 0;
        }
    }

    if (currentFrameTime - lastFrameTime > frameDistance[state])
    {
        if (frameDuration > 0)
            frameDuration--;

        frame++;

        if (state == 4 && frame == maxFrames[state])
            frame--;
        else
            frame %= maxFrames[state];

        updateFrame(srcXFrames[frame], srcYFrames[state][movingDirection]);

        lastFrameTime = currentFrameTime;
    }

    switch (state)
    {
            case 1:
            {
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

                break;
            }

            case 2:
            {
                if (frame == 2)
                    attackingState = 1;
                else
                    attackingState = 0;

                break;
            }

            case 3:
            {
                if (frame == 3)
                    attackingState = 2;
                else
                    attackingState = 0;

                break;
            }

            case 4:
            {
                 if (frameDuration == 0)
                     gameRunning = false;

                break;
            }
    }
}

void updateCamera(SDL_Rect &camera, Player &mainCharacter)
{
        camera.x = mainCharacter.getHitBox().x - SCREEN_WIDTH / 2;
        camera.y = mainCharacter.getHitBox().y - SCREEN_HEIGHT / 2;
        camera.w = SCREEN_WIDTH;
        camera.h = SCREEN_HEIGHT;

        if (camera.x < 0) camera.x = 0;
        if (camera.y < 0) camera.y = 0;
        if (camera.x > LEVEL_WIDTH - SCREEN_WIDTH) camera.x = LEVEL_WIDTH - SCREEN_WIDTH;
        if (camera.y > LEVEL_HEIGHT - SCREEN_HEIGHT) camera.y = LEVEL_HEIGHT - SCREEN_HEIGHT;
}

