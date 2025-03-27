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

    SDL_Texture* titleScreen = window.loadTexture("resources/Miscellaneous/TitleScreen.png");

    SDL_Rect button;
    button = makeRec(197, 253, 100, 45);

    SDL_Rect camera;

    Map map(window);

    setupEnemyTexture(window);

    Player player = setupPlayerTexture(window);

    window.setUpHUD();

    bool gameRunning = true;

    bool gameStarting = false;

    bool buttonEffect = false;

    newWave(0);

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
            else if (!gameStarting && event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                pair<int, int> mouse = window.getMousePosition(x, y);

                if (checkInside(mouse.first, mouse.second, button))
                {
                    gameStarting = true;

                    window.setFade();

                    player.levelUp(1);
                    player.setDesX(300); player.setDesY(500);

                    newWave(0);
                }
            }
            else if (!gameStarting && event.type == SDL_MOUSEMOTION)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                pair<int, int> mouse = window.getMousePosition(x, y);

                if (checkInside(mouse.first, mouse.second, button))
                    buttonEffect = true;
                else
                    buttonEffect = false;
            }
        }

        if (pauseGame || !gameRunning)
            continue;

        if (window.screenFade() == 1)
        {
            window.display();

            window.delay(startTime);

            continue;
        }

        window.init();

        if (!gameStarting)
        {
            camera.x = camera.y = 0;

            Entity screen(0, 0, 480, 270, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, titleScreen);

            window.renderTitle(screen, camera, buttonEffect);

            window.screenFade();

            window.display();

            window.delay(startTime);

            continue;
        }

        float currentFrameTime = SDL_GetPerformanceCounter() / (float)SDL_GetPerformanceFrequency() * 1000.0f;

        player.updatePlayerMovement(map.getHitBoxes(), currentFrameTime, gameStarting);

        if (player.checkDeath() && gameStarting == false)
        {
            window.setFade();
            buttonEffect = false;
        }

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

        window.screenFade();

        window.display();

        window.delay(startTime);
    }

    window.init();
    window.cleanUp();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
