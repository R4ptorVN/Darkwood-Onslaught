#include <iostream>
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
    SDL_Texture* girlWalk = window.loadTexture("resources/girlWalk.png");

    const int movementSpeed = 7;

    Entity background(0, 0, 1280, 720, 0, 0, grassTexture);
    Entity character(40, 50, 49, 79, 50, 50, girlWalk);

    if (grassTexture == NULL)
        SDL_Log("Failed to load texture");

    bool gameRunning = true;

    SDL_Event event;

    bool movingLeft = false;
    bool movingRight = false;
    bool movingUp = false;
    bool movingDown = false;

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
                        movingLeft = true;
                        break;
                    }

                    case SDLK_w:
                    {
                        movingUp = true;
                        break;
                    }

                    case SDLK_d:
                    {
                        movingRight = true;
                        break;
                    }

                    case SDLK_s:
                    {
                        movingDown = true;
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

            if (event.type == SDL_KEYUP)
            {
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_a:
                        {
                            movingLeft = false;
                            break;
                        }

                        case SDLK_w:
                        {
                            movingUp = false;
                            break;
                        }

                        case SDLK_d:
                        {
                            movingRight = false;
                            break;
                        }

                        case SDLK_s:
                        {
                            movingDown = false;
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

        window.clear();
        window.render(background);
        window.render(character);
        window.display();

    }

    window.cleanUp();
    SDL_Quit();
    return 0;
}
