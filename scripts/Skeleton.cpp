#include "lib/Skeleton.hpp"

using namespace std;

SDL_Texture* skeletonTexture[5];

void setUpSkeletonTexture(RenderWindow& window)
{
	skeletonTexture[0] = window.loadTexture("resources/Enemies/Skeleton/skeletonWalking.png");
	skeletonTexture[1] = window.loadTexture("resources/Enemies/Skeleton/skeletonAttacking.png");
	skeletonTexture[2] = window.loadTexture("resources/Enemies/Skeleton/skeletonBlocking.png");
	skeletonTexture[3] = window.loadTexture("resources/Enemies/Skeleton/skeletonDamaged.png");
	skeletonTexture[4] = window.loadTexture("resources/Enemies/Skeleton/skeletonDeath.png");
}

Skeleton::Skeleton(float desX, float desY)
:Enemy(32, 41, 16, 4, desX, desY, 16 * 1.25, 4 * 1.25, skeletonTexture[0])
{
 	movementSpeed = 1;

 	healthPoints = 60;

 	targetX = targetY = 0;
 	randomValueX = (mt() % 15) - (mt() % 15);
 	randomValueY = (mt() % 15) - (mt() % 15);

 	maxFrames[0] = 8;
 	maxFrames[1] = 11;
 	maxFrames[2] = 1;
 	maxFrames[3] = 1;
 	maxFrames[4] = 1;

 	srcXFrames[0][0] = 32;
 	srcYFrames[0] = 41;
 	srcWFrames[0] = 16;
 	srcHFrames[0] = 4;
 	for(int i = 1; i < maxFrames[0]; i++)
 		srcXFrames[0][i] = srcXFrames[0][i - 1] + 80;

 	srcXFrames[1][0] = 32;
 	srcYFrames[1] = 48;
 	srcWFrames[1] = 32;
 	srcHFrames[1] = 4;
 	for (int i = 1; i < maxFrames[1]; i++)
 		srcXFrames[1][i] = srcXFrames[1][i - 1] + 96;

 	srcXFrames[2][0] = 16;
 	srcXFrames[2][1] = 16 + 64;
 	srcYFrames[2] = 43;
 	srcWFrames[2] = 32;
 	srcHFrames[2] = 4;

 	srcXFrames[3][0] = 16;
 	srcYFrames[3] = 43;
 	srcWFrames[3] = 32;
 	srcHFrames[3] = 4;

 	srcXFrames[4][0] = 32;
 	srcYFrames[4] = 43;
 	srcWFrames[4] = 16;
 	srcHFrames[4] = 4;
 	for (int i = 1; i < 3; i++)
 		srcXFrames[4][i] = srcXFrames[4][i - 1] + 96;

 	renderBox[0] = makeRec(-32, -41, 80, 48);
 	renderBox[1] = makeRec(-32, -48, 96, 54);
 	renderBox[2] = makeRec(-16, -43, 64, 48);
 	renderBox[3] = makeRec(-16, -43, 64, 48);
 	renderBox[4] = makeRec(-32, -43, 97, 63);
}

SDL_Rect Skeleton::getHitBox()
{
 	SDL_Rect hitBox = getDestFrame();

 	hitBox.x -= (4 * 1.25);
 	hitBox.y -= (35 * 1.25);
 	hitBox.w = (15 * 1.25);
 	hitBox.h = (29 * 1.25);

 	return hitBox;
}

SDL_Rect Skeleton::getBodyBox()
{
 	SDL_Rect BodyBox = getDestFrame();

 	BodyBox.x -= (4 * 1.25);
 	BodyBox.y -= (35 * 1.25);
 	BodyBox.w = (15 * 1.25);
 	BodyBox.h = (19 * 1.25);

 	return BodyBox;
 }

 void Skeleton::checkDamageEnemy(Player &player)
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
 		if (state == 2 && ((player.getDesX() < getDesX() && facingLeft) || ((player.getDesX() > getDesX() && !facingLeft))))
 		{
 			frame = 1;
 			return;
 		}
 		takingDamage = true;

 		float health = getHealthPoints();
 		health -= player.getAttackingDamage();
 		setHealthPoints(health);
 	}
 }

 void Skeleton::updateEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime, vector<pair<SDL_Rect, float> > &damageBoxes)
 {

 	if (currentFrameTime - lastFramePos > 550)
 	{
 		targetX = player.getDesX() + randomValueX;
 		targetY = player.getDesY() + randomValueY;
 		lastFramePos = currentFrameTime;
 	}

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
 		else if (actionCooldown == 0 && abs(targetX - getDesX()) <= 55 && abs(targetY - getDesY()) <= 35)
 		{
 			actionCooldown = 250;

 			state = 1 + (mt() % 2);
 			frame = 0;
 			updateFrame(srcXFrames[state][frame], srcYFrames[state], srcWFrames[state], srcHFrames[state]);
 			frameDuration = 11;
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

 	setTex(skeletonTexture[state]);

 	if (currentFrameTime - lastFrameTime > 85)
 	{
 		if (frameDuration > 0)
 		{
 			frameDuration--;
 			if (frameDuration == 0)
 			{
 				takingDamage = false;
 				knockBack = 0;
 			}
 		}

 		if (state < 2)
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
 			if (frame == 7)
 			{
 				SDL_Rect sword;

 				if (facingLeft)
 				{
 					sword.x = getDesX() - (31 * 1.25);
 					sword.y = getDesY() - (16 * 1.25);
 					sword.w = 31 * 1.25;
 					sword.h = 5 * 1.25;
 				}
 				else
 				{
 					sword.x = getDesX() + (32 * 1.25);
 					sword.y = getDesY() - (16 * 1.25);
 					sword.w = 31 * 1.25;
 					sword.h = 5 * 1.25;
 				}

 				damageBoxes.push_back({sword, 5});
 			}

 			break;
 		}

 		case 3:
 		{
 			if (knockBack > 0)
 				knockBackEnemy(Obstacles);

 			break;
 		}
 	}

 	if (facingLeft)
 		setFlip(SDL_FLIP_NONE);
 	else
 		setFlip(SDL_FLIP_HORIZONTAL);

 	if (state != 4)
 	    damageBoxes.push_back({getBodyBox(), 0.5});
}