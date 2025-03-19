#include "lib/Orc.hpp"

using namespace std;

SDL_Texture* orcTexture[5];

SDL_Rect spikes[8];

void setUpOrcTexture(RenderWindow& window)
{
	orcTexture[0] = window.loadTexture("resources/Enemies/Orc/orcWalking.png");
	orcTexture[1] = window.loadTexture("resources/Enemies/Orc/orcAttacking.png");
	orcTexture[2] = window.loadTexture("resources/Enemies/Orc/orcSummonSpike.png");
	orcTexture[3] = window.loadTexture("resources/Enemies/Orc/orcDamaged.png");
	orcTexture[4] = window.loadTexture("resources/Enemies/Orc/orcDeath.png");

	spikes[0] = makeRec(5, 56, 10, 3);
	spikes[1] = makeRec(26, 45, 10, 3);
	spikes[2] = makeRec(35, 61, 10, 3);
	spikes[3] = makeRec(46, 75, 10, 3);
	spikes[4] = makeRec(48, 34, 10, 3);
	spikes[5] = makeRec(57, 49, 10, 3);
	spikes[6] = makeRec(69, 64, 10, 3);
	spikes[7] = makeRec(89, 53, 10, 3);
}


Orc::Orc(float desX, float desY)
:Enemy(48, 68, 32, 5, desX, desY, 32 * 1.25, 5 * 1.25, orcTexture[0])
{
 	movementSpeed = 0.75;

 	healthPoints = 300;

 	targetX = targetY = 0;
 	randomValueX = (mt() % 15) - (mt() % 15);
 	randomValueY = (mt() % 15) - (mt() % 15);

 	maxFrames[0] = 8;
 	maxFrames[1] = 10;
 	maxFrames[2] = 9;
 	maxFrames[3] = 1;
 	maxFrames[4] = 7;

 	srcXFrames[0][0] = 48;
 	srcYFrames[0] = 68;
 	srcWFrames[0] = 32;
 	srcHFrames[0] = 5;
 	for (int i = 1; i < maxFrames[0]; i++)
 		srcXFrames[0][i] = srcXFrames[0][i - 1] + 128;

 	srcXFrames[1][0] = 64;
 	srcYFrames[1] = 114;
 	srcWFrames[1] = 48;
 	srcHFrames[1] = 5;
 	for (int i = 1; i < maxFrames[1]; i++)
 		srcXFrames[1][i] = srcXFrames[1][i - 1] + 176;

 	srcXFrames[2][0] = 32;
 	srcYFrames[2] = 117;
 	srcWFrames[2] = 32;
 	srcHFrames[2] = 5;
 	for (int i = 1; i < maxFrames[2]; i++)
 		srcXFrames[2][i] = srcXFrames[2][i - 1] + 96;

 	srcXFrames[3][0] = 32;
 	srcYFrames[3] = 79;
 	srcWFrames[3] = 48;
 	srcHFrames[3] = 5;

 	srcXFrames[4][0] = 32;
 	srcYFrames[4] = 75;
 	srcWFrames[4] = 32;
 	srcHFrames[4] = 5;
 	for (int i = 1; i < maxFrames[4]; i++)
 		srcXFrames[4][i] = srcXFrames[4][i - 1] + 96;

 	renderBox[0] = makeRec(-48, -68, 128, 80);
 	renderBox[1] = makeRec(-64, -114, 176, 128);
 	renderBox[2] = makeRec(-32, -117, 96, 126);
 	renderBox[3] = makeRec(-32, -79, 112, 91);
 	renderBox[4] = makeRec(-32, -75, 96, 84);
}

SDL_Rect Orc::getHitBox()
{
 	SDL_Rect hitBox = getDestFrame();

 	hitBox.x += (3 * 1.25);
 	hitBox.y -= (49 * 1.25);
 	hitBox.w = (43 * 1.25);
 	hitBox.h = (38 * 1.25);

 	return hitBox;
}

SDL_Rect Orc::getBodyBox()
{
 	SDL_Rect BodyBox = getDestFrame();

 	BodyBox.x += (5 * 1.25);
 	BodyBox.y -= (43 * 1.25);
 	BodyBox.w = (37 * 1.25);
 	BodyBox.h = (20 * 1.25);

 	return BodyBox;
}

void Orc::checkDamageEnemy(Player &player)
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

 void Orc::updateEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime, vector<pair<SDL_Rect, float> > &damageBoxes, vector<Enemy*> &Enemies)
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
 		else if (actionCooldown == 0 && abs(targetX - getDesX()) <= 90 && abs(targetY - getDesY()) <= 35)
 		{
 			actionCooldown = 350;

 			state = 1;
 			frame = 0;
 			updateFrame(srcXFrames[state][frame], srcYFrames[state], srcWFrames[state], srcHFrames[state]);
 			frameDuration = maxFrames[state];
 		}
 		else if (actionCooldown == 0 && abs(targetX - getDesX()) <= 200 && abs(targetY - getDesY()) <= 200)
 		{
 			actionCooldown = 350;

 			state = 2;
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

 	setTex(orcTexture[state]);

 	if (currentFrameTime - lastFrameTime > 110)
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
 			moveEnemy(Obstacles);
 			break;
 		}

 		case 1:
 		{
 			if (frame == 7)
 			{
 				SDL_Rect axe;

 				if (facingLeft)
 				{
 					axe.x = getDesX() - (56 * 1.25);
 					axe.y = getDesY() - (24 * 1.25);
 					axe.w = 40 * 1.25;
 					axe.h = 38 * 1.25;
 				}
 				else
 				{
 					axe.x = getDesX() + (64 * 1.25);
 					axe.y = getDesY() - (24 * 1.25);
 					axe.w = 40 * 1.25;
 					axe.h = 38 * 1.25;
 				}

 				damageBoxes.push_back({axe, 6});
 			}

 			break;
 		}

 		case 2:
 		{
 			if (actionCooldown == 349)
 			{
 				for (int i = 0; i < 8; i++)
 					Enemies.push_back(new Projectile(1, spikes[i].x, spikes[i].y, player.getHitBox().x + (spikes[i].x * 1.25) - 60, player.getHitBox().y + (spikes[i].y * 1.25) - 20));
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