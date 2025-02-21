﻿#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "Functions.hpp"
#include "Obstacle.hpp"

int main(int argc, char* args [])
{

    if (SDL_Init(SDL_INIT_VIDEO))
        SDL_Log("Failed to initialize SDL2");
    if (!(IMG_Init(IMG_INIT_PNG)))
        SDL_Log("Failed to initialize SDL2_image");

    RenderWindow window("GAME v1.0");

    SDL_Texture* grassTexture = window.loadTexture("resources/grass.png");
    SDL_Texture* rock1Texture = window.loadTexture("resources/Rock1.png");
    SDL_Texture* rock2Texture = window.loadTexture("resources/Rock2.png");
    SDL_Texture* rock3Texture = window.loadTexture("resources/Rock3.png");
    SDL_Texture* treeTexture = window.loadTexture("resources/Tree1.png");
    SDL_Texture* girlIdle = window.loadTexture("resources/GirlIdle.png");
    SDL_Texture* girlWalk = window.loadTexture("resources/GirlWalk.png");
    SDL_Texture* girlAttack = window.loadTexture("resources/GirlAttack.png");

    SDL_Rect camera;

    Entity background(0, 0, 1280, 720, 0, 0, 1280, 720, grassTexture);

    Obstacle rock1(0, 0, 32, 32, 300, 300, 32 * 4, 32 * 4, rock1Texture);
    Obstacle rock2(0, 0, 37, 33, 800, 200, 37 * 3.5, 33 * 3.5, rock2Texture);
    Obstacle rock3(0, 0, 25, 26, 700, 550, 25 * 4, 26 * 4, rock3Texture);
    Obstacle tree1(0, 0, 62, 57, 450, 450, 62 * 3, 57 * 3, treeTexture);

    vector<Entity> ObstaclesLower;
    ObstaclesLower.clear();
    ObstaclesLower.push_back(rock1.getLowerHalf());
    ObstaclesLower.push_back(rock2.getLowerHalf());
    ObstaclesLower.push_back(rock3.getLowerHalf());
    ObstaclesLower.push_back(tree1.getLowerHalf());

    vector<Entity> ObstaclesUpper;
    ObstaclesUpper.clear();
    ObstaclesUpper.push_back(rock1.getUpperHalf());
    ObstaclesUpper.push_back(rock2.getUpperHalf());
    ObstaclesUpper.push_back(rock3.getUpperHalf());
    ObstaclesUpper.push_back(tree1.getUpperHalf());

    vector<SDL_Texture*> mainCharacterAnimations;
    mainCharacterAnimations.clear();
    mainCharacterAnimations.push_back(girlIdle);
    mainCharacterAnimations.push_back(girlWalk);
    mainCharacterAnimations.push_back(girlAttack);

    Player mainCharacter(42, 55, 41, 75, 300, 500, 41, 75, mainCharacterAnimations);

    bool gameRunning = true;

    SDL_Event event;

    while (gameRunning)
    {
        Uint32 startTime = SDL_GetPerformanceCounter();

        bool pauseGame = false;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
                gameRunning = false;
            else if (event.type == SDL_WINDOWEVENT)
            {
                switch (event.window.event)
                {
                        case SDL_WINDOWEVENT_FOCUS_LOST:
                            pauseGame = true;
                            break;

                        case SDL_WINDOWEVENT_FOCUS_GAINED:
                            pauseGame = false;
                            break;

                        case SDL_WINDOWEVENT_MOVED:
                            pauseGame = true;
                            break;

                        case SDL_WINDOWEVENT_RESIZED:
                            pauseGame = true;
                            break;

                        case SDL_WINDOWEVENT_ICCPROF_CHANGED:
                            pauseGame = true;
                            break;
                }
            }
        }

        if (pauseGame || !gameRunning)
            continue;

        window.init();

        float currentFrameTime = SDL_GetPerformanceCounter() / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        mainCharacter.updateMovement(ObstaclesLower, ObstaclesUpper, currentFrameTime);

        updateCamera(camera, mainCharacter);

        window.render(background, camera);
        for (Entity &object : ObstaclesLower)
            window.render(object, camera);
        window.render(mainCharacter, camera);
        for (Entity &object : ObstaclesUpper)
            window.render(object, camera);

        window.display();

        Uint32 endTime = SDL_GetPerformanceCounter();

        float elapsedTime = (endTime - startTime) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

        SDL_Delay(floor(16.666f - elapsedTime));
    }

    window.init();
    window.cleanUp();
    SDL_Quit();

    return 0;
}
