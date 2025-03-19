#include "lib/Necromancer.hpp"

#include "lib/Orc.hpp"

using namespace std;

SDL_Texture* necromancerTexture[5];

void setUpNecromancerTexture(RenderWindow& window)
{
      necromancerTexture[0] = window.loadTexture("resources/Enemies/Necromancer/necromancerFloating.png");
      necromancerTexture[1] = window.loadTexture("resources/Enemies/Necromancer/necromancerAttacking.png");
      necromancerTexture[3] = window.loadTexture("resources/Enemies/Necromancer/necromancerDamaged.png");
      necromancerTexture[4] = window.loadTexture("resources/Enemies/Necromancer/necromancerDeath.png");
}

Necromancer::Necromancer(float desX, float desY)
:Enemy(64, 93, 16, 3, desX, desY, 16 * 1.25, 3 * 1.25, necromancerTexture[0])
{
      movementSpeed = 1.25;

      healthPoints = 120;

      isAttacking = false;

      targetX = targetY = 0;
      randomValueX = (mt() % 15) - (mt() % 15);
      randomValueY = (mt() % 15) - (mt() % 15);

      maxFrames[0] = 16;
      maxFrames[1] = 9;
      maxFrames[3] = 1;
      maxFrames[4] = 1;

      srcXFrames[0][0] = 64;
      srcYFrames[0] = 93;
      srcWFrames[0] = 16;
      srcHFrames[0] = 3;
      for (int i = 1; i < maxFrames[0]; i++)
            srcXFrames[0][i] = srcXFrames[0][i - 1] + 144;

      srcXFrames[1][0] = 64;
      srcYFrames[1] = 93;
      srcWFrames[1] = 16;
      srcHFrames[1] = 3;
      for (int i = 1; i < maxFrames[1]; i++)
            srcXFrames[1][i] = srcXFrames[1][i - 1] + 144;

      srcXFrames[3][0] = 64;
      srcYFrames[3] = 93;
      srcWFrames[3] = 16;
      srcHFrames[3] = 3;

      srcXFrames[4][0] = 32;
      srcYFrames[4] = 28;
      srcWFrames[4] = 16;
      srcHFrames[4] = 4;
      for (int i = 1; i < 3; i++)
            srcXFrames[4][i] = srcXFrames[4][i - 1] + 96;

      renderBox[0] = makeRec(-64, -93, 144, 128);
      renderBox[1] = makeRec(-64, -93, 144, 128);
      renderBox[3] = makeRec(-64, -93, 144, 128);
      renderBox[4] = makeRec(-32, -28, 96, 48);
}

SDL_Rect Necromancer::getHitBox()
{
      SDL_Rect hitBox = getDestFrame();

      hitBox.x += (1 * 1.25);
      hitBox.y -= (47 * 1.25);
      hitBox.w = (14 * 1.25);
      hitBox.h = (48 * 1.25);

      return hitBox;
}

SDL_Rect Necromancer::getBodyBox()
{
      SDL_Rect bodyBox = getDestFrame();

      bodyBox.x += (3 * 1.25);
      bodyBox.y -= (36 * 1.25);
      bodyBox.w = (10 * 1.25);
      bodyBox.h = (25 * 1.25);

      return bodyBox;
}

pair<int, int> Necromancer::getLaserPos()
{
      int x, y;
      if (!facingLeft)
      {
            x = getDesX() + (35 * 1.25);
            y = getDesY() - (6 * 1.25);
      }
      else
      {
            x = getDesX() - (500 * 1.25);
            y = getDesY() - (6 * 1.25);
      }

      return make_pair(x, y);
}

void Necromancer::checkDamageEnemy(Player &player)
{
      if (state > 2)
            return;

      if (player.getAttackingState() == 2)
      {
            checkSkill(getHitBox(), player);
            return;
      }

      SDL_Rect a1 = player.getSwordBox(0);
      SDL_Rect a2 = player.getSwordBox(1);
      SDL_Rect b = getHitBox();
      if (SDL_HasIntersection(&a1, &b) || SDL_HasIntersection(&a2, &b))
      {
            takingDamage = true;

            float health = getHealthPoints();
            health -= player.getAttackingDamage();
            setHealthPoints(health);
      }
}

void Necromancer::updateEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime, vector<pair<SDL_Rect, float> > &damageBoxes, vector<Enemy *> &Enemies)
{
      if (currentFrameTime - lastFramePos > 550)
      {
            targetX = player.getDesX() + randomValueX;
            targetY = player.getDesY() + randomValueY;
            lastFramePos = currentFrameTime;
      }

      if (abs(targetX - getDesX()) <= 250)
            targetX = getDesX();

      if (frameDuration == 0 || takingDamage)
      {
            if (checkDeath() && frameDuration == 0)
            {
                  frameDuration = 15;

                  state = 4;
                  frame = mt() % 3;
                  updateFrame(srcXFrames[state][frame], srcYFrames[state], srcWFrames[state], srcHFrames[state]);
            }
            else if (takingDamage)
            {
                  if (state != 3)
                        frameDuration = 5;
                  state = 3;
                  frame = 0;
                  updateFrame(srcXFrames[state][frame], srcYFrames[state], srcWFrames[state], srcHFrames[state]);
            }
            else if (actionCooldown == 0 && abs(targetY - getDesY()) <= 50 && abs(targetX - getDesX()) <= 250)
            {
                  actionCooldown = 300;

                  state = 1;
                  frame = 0;
                  updateFrame(srcXFrames[state][frame], srcYFrames[state], srcWFrames[state], srcHFrames[state]);
                  frameDuration = maxFrames[state];
            }
            else
            {
                  state = 0;
                  frame %= maxFrames[0];
                  updateFrame(srcXFrames[state][frame], srcYFrames[state], srcWFrames[state], srcHFrames[state]);
            }
      }

      if (actionCooldown > 0)
            actionCooldown--;

      setTex(necromancerTexture[state]);


      if (currentFrameTime - lastFrameTime > 110)
      {
            if (frameDuration > 0)
            {
                  frameDuration--;
                  if (frameDuration == 0)
                  {
                        takingDamage = false;
                        knockBack = 0;
                        isAttacking = false;
                  }
            }


            if (state < 3)
            {
                  frame++;
                  frame %= maxFrames[state];
            }

            updateFrame(srcXFrames[state][frame], srcYFrames[state], srcWFrames[state], srcHFrames[state]);

            lastFrameTime = currentFrameTime;
      }

      switch (state)
      {
            case 0:
            {
                moveEnemy(Obstacles);
                break;
            }

            case 1:
            {
                  if (frame == 4 && !isAttacking)
                  {
                        if (!facingLeft)
                              Enemies.push_back(new Projectile(2, 0, 48, getLaserPos().first, getLaserPos().second));
                        else
                              Enemies.push_back(new Projectile(3, 0, 48, getLaserPos().first, getLaserPos().second));

                        isAttacking = true;
                  }
            }

            case 3:
            {
                  if (knockBack > 0)
                        knockBackEnemy(Obstacles);

                  break;
            }
      }

      if (targetX == getDesX())
      {
            if (player.getDesX() < getDesX())
                  facingLeft = true;
            else
                  facingLeft = false;
      }

      if (facingLeft)
            setFlip(SDL_FLIP_NONE);
      else
            setFlip(SDL_FLIP_HORIZONTAL);

      if (state != 4)
            damageBoxes.push_back({getBodyBox(), 0.25});
}

