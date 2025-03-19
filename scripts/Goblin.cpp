#include "lib/Goblin.hpp"

using namespace std;

SDL_Texture* goblinTexture[5];

void setUpGoblinTexture(RenderWindow& window)
{
	goblinTexture[0] = window.loadTexture("resources/Enemies/Goblin/goblinWalking.png");
	goblinTexture[1] = window.loadTexture("resources/Enemies/Goblin/goblinRunning.png");
	goblinTexture[2] = window.loadTexture("resources/Enemies/Goblin/goblinAttacking.png");
	goblinTexture[3] = window.loadTexture("resources/Enemies/Goblin/goblinDamaged.png");
	goblinTexture[4] = window.loadTexture("resources/Enemies/Goblin/goblinDeath.png");
}

Goblin::Goblin(float desX, float desY)
:Enemy(34, 45, 12, 3, desX, desY, 12 * 1.25, 3 * 1.25, goblinTexture[0])
{
 	movementSpeed = 1;

 	healthPoints = 35;

 	targetX = targetY = 0;
 	randomValueX = (mt() % 15) - (mt() % 15);
 	randomValueY = (mt() % 15) - (mt() % 15);

 	maxFrames[0] = 6;
 	maxFrames[1] = 6;
 	maxFrames[2] = 7;
 	maxFrames[3] = 1;
 	maxFrames[4] = 7;

 	srcXFrames[0][0] = 34;
 	srcYFrames[0] = 45;
 	srcWFrames[0] = 12;
 	srcHFrames[0] = 3;
 	for (int i = 1; i < maxFrames[0]; i++)
 		srcXFrames[0][i] = srcXFrames[0][i - 1] + 80;

 	srcXFrames[1][0] = 34;
 	srcYFrames[1] = 45;
 	srcWFrames[1] = 12;
 	srcHFrames[1] = 3;
 	for (int i = 1; i < maxFrames[1]; i++)
 		srcXFrames[1][i] = srcXFrames[1][i - 1] + 80;

 	srcXFrames[2][0] = 34;
 	srcYFrames[2] = 45;
 	srcWFrames[2] = 12;
 	srcHFrames[2] = 3;
 	for (int i = 1; i < maxFrames[2]; i++)
 		srcXFrames[2][i] = srcXFrames[2][i - 1] + 80;

 	srcXFrames[3][0] = 34;
 	srcYFrames[3] = 45;
 	srcWFrames[3] = 12;
 	srcHFrames[3] = 3;

 	srcXFrames[4][0] = 34;
 	srcYFrames[4] = 45;
 	srcWFrames[4] = 12;
 	srcHFrames[4] = 3;
 	for (int i = 1; i < maxFrames[4]; i++)
 		srcXFrames[4][i] = srcXFrames[4][i - 1] + 80;

 	renderBox[0] = makeRec(-34, -45, 80, 64);
 	renderBox[1] = makeRec(-34, -45, 80, 64);
 	renderBox[2] = makeRec(-34, -45, 80, 64);
 	renderBox[3] = makeRec(-34, -45, 80, 64);
 	renderBox[4] = makeRec(-34, -45, 80, 64);
}

SDL_Rect Goblin::getHitBox()
{
 	SDL_Rect hitBox = getDestFrame();

 	hitBox.x -= (6 * 1.25);
 	hitBox.y -= (23 * 1.25);
 	hitBox.w = (20 * 1.25);
 	hitBox.h = (20 * 1.25);

 	return hitBox;
}

SDL_Rect Goblin::getBodyBox()
{
 	SDL_Rect BodyBox = getDestFrame();

 	BodyBox.x -= (1 * 1.25);
 	BodyBox.y -= (18 * 1.25);
 	BodyBox.w = (11 * 1.25);
 	BodyBox.h = (13 * 1.25);

 	return BodyBox;
}

void Goblin::checkDamageEnemy(Player &player)
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

void Goblin::updateEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime, vector<pair<SDL_Rect, float> > &damageBoxes)
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
 			frame = 0;
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
 		 	actionCooldown = 350;

 		 	state = 2;
 		 	frame = 0;
 		 	updateFrame(srcXFrames[state][frame], srcYFrames[state], srcWFrames[state], srcHFrames[state]);
 		 	frameDuration = maxFrames[state];
 		 }
 		 else if (abs(targetX - getDesX()) <= 175 && abs(targetY - getDesY()) <= 175)
 		 {
 		 	state = 1;
 		 	frame %= maxFrames[1];
 		 	updateFrame(srcXFrames[state][frame], srcYFrames[state], srcWFrames[state], srcHFrames[state]);
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

 	setTex(goblinTexture[state]);

 	if (currentFrameTime - lastFrameTime > 90)
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

 		if (state == 4 && frame == maxFrames[4] - 1)
 			frame--;
 		frame++;
 		frame %= maxFrames[state];

 		updateFrame(srcXFrames[state][frame], srcYFrames[state], srcWFrames[state], srcHFrames[state]);

 		lastFrameTime = currentFrameTime;
 	}

 	switch (state)
 	{
 		case 0:
 		{
 			movementSpeed = 1;
 			moveEnemy(Obstacles);
 			break;
 		}

 		case 1:
 		{
 			movementSpeed = 1.5;
 			moveEnemy(Obstacles);
 			break;
 		}

 		case 2:
 		{
 			if (frame == 5)
 			{
 				SDL_Rect club;

 				if (facingLeft)
 				{
 					club.x = getDesX() - (27 * 1.25);
 					club.y = getDesY() - (12 * 1.25);
 					club.w = 16 * 1.25;
 					club.h = 11 * 1.25;
 				}
 				else
 				{
 					club.x = getDesX() + (23 * 1.25);
 					club.y = getDesY() - (12 * 1.25);
 					club.w = 16 * 1.25;
 					club.h = 11 * 1.25;
 				}

 				damageBoxes.push_back({club, 3});
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
 		damageBoxes.push_back({getBodyBox(), 0.25});
}