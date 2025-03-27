
#include <iostream>

using namespace std;

#include "lib/RenderWindow.hpp"

RenderWindow::RenderWindow(const char* p_title)
:window(NULL), renderer(NULL)
{
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LEVEL_WIDTH, LEVEL_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL)
        SDL_Log("Window failed to initialize");

    fadeStatus = 0;

    Entities.clear();

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath)
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, p_filePath);

    if (texture == NULL)
        SDL_Log("Failed to load texture");

    return texture;
}

float tmpW = -1, tmpH = -1;
SDL_Texture* RenderWindow::loadText(const char* source, const char* word, int size, SDL_Color color)
{
    TTF_Font* loadFont = TTF_OpenFont(source, size);
    if (loadFont == NULL)
        SDL_Log("Failed to load font");

    SDL_Texture* texture = NULL;
    SDL_Surface* surface = TTF_RenderText_Solid(loadFont, word, color);
    if (surface == NULL)
        SDL_Log("Failed to render text surface");

    texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture != NULL)
    {
        tmpW = surface->w;
        tmpH = surface->h;
    }

    SDL_FreeSurface(surface);

    TTF_CloseFont(loadFont);

    return texture;
}

SDL_Texture* barFrame;
SDL_Texture* heartIcon;
SDL_Texture* skillIcon;
void RenderWindow::setUpHUD()
{
     barFrame = loadTexture("resources/Miscellaneous/Bar.png");
     heartIcon = loadTexture("resources/Miscellaneous/Heart.png");
     skillIcon = loadTexture("resources/Miscellaneous/Skill.png");
}

void RenderWindow::init()
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderClear(renderer);
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void RenderWindow::clear()
{
    SDL_RenderClear(renderer);
}

pair<int, int> RenderWindow::getMousePosition(int realMouseX, int realMouseY)
{
    int wWidth, wHeight;
    int rLogicalWidth, rLogicalHeight;
    int rRealWidth, rRealHeight;
    float rScaleX, rScaleY;
    int rMidpointY, wMidpointY;
    int rMidpointX, wMidpointX;
    int rY, rX;

    SDL_GetWindowSize(window, &wWidth, &wHeight);
    wMidpointY = wHeight / 2;
    wMidpointX = wWidth / 2;

    SDL_RenderGetLogicalSize(renderer, &rLogicalWidth, &rLogicalHeight);
    SDL_RenderGetScale(renderer, &rScaleX, &rScaleY);
    rRealWidth = (float)rLogicalWidth * (float)rScaleX;
    rRealHeight = (float)rLogicalHeight * (float)rScaleY;
    rMidpointY = rRealHeight / 2;
    rMidpointX = rRealWidth / 2;
    rY = wMidpointY - rMidpointY;
    rX = wMidpointX - rMidpointX;

    int adjustedMouseY = realMouseY - rY;
    int adjustedMouseX = realMouseX - rX;
    int logicalMouseX = (float)adjustedMouseX / (float)rRealWidth * (float)rLogicalWidth;
    int logicalMouseY = (float) adjustedMouseY / (float)rRealHeight * (float)rLogicalHeight;

    return make_pair(logicalMouseX, logicalMouseY);
}

void RenderWindow::render(Entity& p_entity, SDL_Rect &camera)
{
    SDL_Rect src = p_entity.getSrcFrame();
    SDL_Rect dest = p_entity.getDestFrame();
    dest.x -= camera.x;
    dest.y -= camera.y;
    SDL_RenderCopyEx(renderer, p_entity.getTex(), &src, &dest, 0, NULL, p_entity.getFlip());
}

void RenderWindow::renderTitle(Entity& p_entity, SDL_Rect &camera, bool buttonEffect)
{
    render(p_entity, camera);

    SDL_Rect button;
    button = makeRec(197, 253, 100, 45);

    if (!buttonEffect)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    else
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 70);

    SDL_RenderFillRect(renderer, &button);
}

void RenderWindow::setFade()
{
    fadeStatus = 1;
}

int screenAlpha = 0;

int RenderWindow::screenFade()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, screenAlpha);
    SDL_RenderFillRect(renderer, NULL);
    int fadeSpeed = 5;

    if (fadeStatus == 1)
    {
        screenAlpha += fadeSpeed;
        screenAlpha = min(screenAlpha, 255);

        if (screenAlpha == 255)
            fadeStatus = 2;
    }

    if (fadeStatus == 2)
    {
        screenAlpha -= fadeSpeed;
        screenAlpha = max(screenAlpha, 0);

        if (screenAlpha == 0)
            fadeStatus = 0;
    }

    return fadeStatus;
}

void RenderWindow::clearEntities()
{
    Entities.clear();
}

void RenderWindow::pushEntities(Entity& HitBox, SDL_Rect Values, float multiplier)
{
    Entities.push_back({{HitBox, Values}, multiplier});
}

bool compare(pair<pair<Entity, SDL_Rect>, float> x, pair<pair<Entity, SDL_Rect>, float> y)
{
    return(x.first.first.getDesY() + x.first.first.getDesH() < y.first.first.getDesY() + y.first.first.getDesH());
}

void RenderWindow::renderEntity(Entity& HitBox, SDL_Rect Values, float multiplier, SDL_Rect& camera)
{
    SDL_Rect src = HitBox.getSrcFrame();
    SDL_Rect dest = HitBox.getDestFrame();

    dest.x -= camera.x;
    dest.y -= camera.y;

    src.x += Values.x;
    src.y += Values.y;
    src.w = Values.w;
    src.h = Values.h;

    dest.x += (Values.x * multiplier);
    dest.y += (Values.y * multiplier);
    dest.w = (Values.w * multiplier);
    dest.h = (Values.h * multiplier);

    SDL_RenderCopyEx(renderer, HitBox.getTex(), &src, &dest, 0, NULL, HitBox.getFlip());
}

void RenderWindow::renderEntities(SDL_Rect &camera)
{
    sort(Entities.begin(), Entities.end(), compare);

    for (int i = 0; i < int(Entities.size()); i++)
        renderEntity(Entities[i].first.first, Entities[i].first.second, Entities[i].second, camera);
}

void RenderWindow::renderBars(SDL_Rect &healthBar, SDL_Rect &manaBar)
{
    healthBar.x = 30; healthBar.y = SCREEN_HEIGHT - 60;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &healthBar);
    SDL_RenderDrawRect(renderer, &healthBar);

    SDL_Rect srcFrame = makeRec(0, 0, 52, 9);
    SDL_Rect destFrame = makeRec(29, SCREEN_HEIGHT - 60, 101, 14);

    SDL_RenderCopyEx(renderer, barFrame, &srcFrame, &destFrame, 0, NULL, SDL_FLIP_NONE);

    srcFrame = makeRec(0, 0, 13, 12);
    destFrame = makeRec(11, SCREEN_HEIGHT - 65, 26, 24);

    SDL_RenderCopyEx(renderer, heartIcon, &srcFrame, &destFrame, 0, NULL, SDL_FLIP_NONE);

    manaBar.x = 30; manaBar.y = SCREEN_HEIGHT - 30;

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &manaBar);
    SDL_RenderDrawRect(renderer, &manaBar);

    srcFrame = makeRec(0, 0, 52, 9);
    destFrame = makeRec(29, SCREEN_HEIGHT - 30, 101, 14);

    SDL_RenderCopyEx(renderer, barFrame, &srcFrame, &destFrame, 0, NULL, SDL_FLIP_NONE);

    srcFrame = makeRec(0, 0, 360, 360);
    destFrame = makeRec(12, SCREEN_HEIGHT - 35, 23, 22);

    SDL_RenderCopyEx(renderer, skillIcon, &srcFrame, &destFrame, 0, NULL, SDL_FLIP_NONE);
}

void RenderWindow::renderWave(int wave)
{
    SDL_Color textColor = {0, 0, 0};
    string strWord = "Wave:" + to_string(wave);
    const char *Word = strWord.c_str();
    SDL_Texture* waveText = loadText("resources/Font/PublicPixel.ttf", Word, 20, textColor);

    SDL_Rect* src = NULL;
    SDL_Rect dest = makeRec(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 30, tmpW, tmpH);

    SDL_RenderCopy(renderer, waveText, src, &dest);
}

void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}

void RenderWindow::delay(float startTime)
{
    Uint32 endTime = SDL_GetPerformanceCounter();

    float elapsedTime = (endTime - startTime) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

    if (16.666f < elapsedTime)
        elapsedTime = 16.666f;

    SDL_Delay(floor(16.666f - elapsedTime));
}

void RenderWindow::cleanUp()
{
     SDL_DestroyRenderer(renderer);
     SDL_DestroyWindow(window);
}
