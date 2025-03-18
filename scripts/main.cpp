#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;

#include "lib/Map.hpp"
#include "lib/Game.hpp"

int main(int argc, char* args [])
{
    if (SDL_Init(SDL_INIT_VIDEO))
        SDL_Log("Failed to initialize SDL2");
    if (!(IMG_Init(IMG_INIT_PNG)))
        SDL_Log("Failed to initialize SDL2_image");
    if (TTF_Init() == -1)
        SDL_Log("Failed to initialize SDL2_ttf");

    RenderWindow window("GAME v1.0");

    SDL_Rect camera;

    Map map(window);

    setupEnemyTexture(window);

    Player player = setupPlayerTexture(window);

    window.setUpHUD();

    bool gameRunning = true;

    newWave();

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
                    case SDL_WINDOWEVENT_CLOSE:
                        gameRunning = false;
                    break;

                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        pauseGame = true;
                    break;

                    case SDL_WINDOWEVENT_MOVED:
                        pauseGame = true;
                    break;

                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        pauseGame = false;
                    break;
                }
            }
        }

        if (pauseGame || !gameRunning)
            continue;

        window.init();

        float currentFrameTime = SDL_GetPerformanceCounter() / (float)SDL_GetPerformanceFrequency() * 1000.0f;

        player.updatePlayerMovement(map.getHitBoxes(), currentFrameTime, gameRunning);

         if (player.getAttackingState() > 0)
             checkDamageEnemies(player);

        spawnEnemies(currentFrameTime);

        updateEnemies(player, map.getHitBoxes(), currentFrameTime);

         checkContactPlayer(player);

        updateCamera(camera, player);

        map.renderLayer(window, camera, 0);
        map.renderLayer(window, camera, 1);

        window.clearEntities();

        vector<Entity> obstaclesHitBox = map.getHitBoxes();
        vector<SDL_Rect> obstacleDisplay = map.getDisplayBoxesValues();

        for (int i = 0; i < int(obstacleDisplay.size()); i++)
             window.pushEntities(obstaclesHitBox[i], obstacleDisplay[i], 2);

        window.pushEntities(map.getFire(currentFrameTime), makeRec(0, 0, 16, 29), 2);

        window.pushEntities(player, player.getRenderBoxValues(), 1.75);

        vector<Enemy> enemies = getEnemies();

         for (int i = 0; i < int(enemies.size()); i++)
             window.pushEntities(enemies[i], enemies[i].getRenderBoxValues(), 1.25);

        window.renderEntities(camera);

        SDL_Rect healthBar = player.getHealthBar();
        SDL_Rect manaBar = player.getManaBar();
        window.renderBars(healthBar, manaBar);

        int wave = getWave();
        window.renderWave(wave);

        window.display();

        Uint32 endTime = SDL_GetPerformanceCounter();

        float elapsedTime = (endTime - startTime) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

        if (16.666f < elapsedTime)
            elapsedTime = 16.666f;

        SDL_Delay(floor(16.666f - elapsedTime));
    }

    window.init();
    window.cleanUp();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
