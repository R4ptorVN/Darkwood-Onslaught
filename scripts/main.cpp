#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

#include "RenderWindow.hpp"
#include "Entity.hpp"

int main(int argc, char* args [])
{

    if (SDL_Init(SDL_INIT_VIDEO))
        SDL_Log("Failed to initialize SDL2");
    if (!(IMG_Init(IMG_INIT_PNG)))
        SDL_Log("Failed to initialize SDL2_image");

    RenderWindow window("GAME v1.0", 1280, 720);

    SDL_Texture* grassTexture = window.loadTexture("resources/grass.png");
    SDL_Texture* girlWalk = window.loadTexture("resources/GirlWalk.png");
    SDL_Texture* girlIdle = window.loadTexture("resources/GirlIdle.png");
    SDL_Texture* rockTexture = window.loadTexture("resources/Rock.png");

    int movementSpeed = 5;
    const int frameDistance = 128;

    Entity background(0, 0, 1280, 720, 0, 0, 1, 1, grassTexture, SDL_FLIP_NONE);
    Entity rock(0, 0, 32, 32, 300, 300, 3, 3, rockTexture, SDL_FLIP_NONE);

    vector<Entity> walkingFrames;
    vector<Entity> idleFrames;

    int srcX = 42, srcY = 55;
    for (int i = 0; i < 12; i++)
    {
         Entity walking(srcX, srcY, 41, 75, 550, 250, 1, 1, girlWalk, SDL_FLIP_NONE);
         walkingFrames.push_back(walking);
         srcX += frameDistance;
    }
    Entity idle(42, 55, 41, 75, 550, 250, 1, 1, girlIdle, SDL_FLIP_NONE);
    Entity character = idle;

    if (grassTexture == NULL)
        SDL_Log("Failed to load texture");

    bool gameRunning = true;

    SDL_Event event;

    bool movingLeft = false;
    bool movingRight = false;
    bool movingUp = false;
    bool movingDown = false;

    int frame = 0;
    bool facingLeft = false;

    int movingDirections = 0;
    while (gameRunning)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                gameRunning = false;
            if (event.type == SDL_KEYDOWN)
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
                    int curX = character.getX();
                    int curY = character.getY();
                    character = walkingFrames[frame];
                    if (facingLeft)
                        character.setFlip(SDL_FLIP_HORIZONTAL);
                    character.setX(curX);
                    character.setY(curY);
                }
                if (movingDirections > 1)
                    movementSpeed = 2.5;
                else
                    movementSpeed = 5;
                if (movingLeft)
                    character.setX(character.getX() - movementSpeed);
                if (movingUp)
                    character.setY(character.getY() - movementSpeed);
                if (movingRight)
                    character.setX(character.getX() + movementSpeed);
                if (movingDown)
                    character.setY(character.getY() + movementSpeed);
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
                if (movingLeft)
                    character.setX(character.getX() - movementSpeed);
                if (movingUp)
                    character.setY(character.getY() - movementSpeed);
                if (movingRight)
                    character.setX(character.getX() + movementSpeed);
                if (movingDown)
                    character.setY(character.getY() + movementSpeed);
            }
        }
        if (!movingLeft && !movingUp && !movingRight && !movingDown)
        {
            int curX = character.getX();
            int curY = character.getY();
            character = idle;
            if (facingLeft)
                character.setFlip(SDL_FLIP_HORIZONTAL);
            character.setX(curX);
            character.setY(curY);
        }
        window.clear();
        window.render(background);
        window.render(rock);
        window.render(character);
        window.display();

    }

    window.cleanUp();
    SDL_Quit();
    return 0;
}
