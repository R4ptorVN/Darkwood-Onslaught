#include "lib/Enemy.hpp"
#include<bits/stdc++.h>

using namespace std;

const int EnemiesLimit = 15;


Enemy::Enemy(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex)
:Entity(srcX, srcY, srcW, srcH, desX, desY, desW, desH, tex)
{
	healthPoints = 0;

	takingDamage = false;

    movementSpeed = 1;

    frame = 0;

	lastFrameTime = lastFramePos = 0;

	state = 0;

	targetX = targetY = 0;
	randomValueX = (mt() % 15) - (mt() % 15);
	randomValueY = (mt() % 15) - (mt() % 15);

	frameDuration = 0;

	attackCooldown = 0;

	facingLeft = false;

	maxFrames[0] = 8;
	maxFrames[1] = 11;
	maxFrames[2] = 1;

	srcXFrames[0][0] = 16;
	srcYFrames[0] = 40;
	srcWFrames[0] = 32;
	srcHFrames[0] = 4;
	for(int i = 1; i < maxFrames[0]; i++)
		srcXFrames[0][i] = srcXFrames[0][i - 1] + 64;

	srcXFrames[1][0] = 16;
	srcYFrames[1] = 48;
	srcWFrames[1] = 48;
	srcHFrames[1] = 4;
	for (int i = 1; i < maxFrames[1]; i++)
		srcXFrames[1][i] = srcXFrames[1][i - 1] + 80;

	srcXFrames[2][0] = 16;
	srcYFrames[2] = 43;
	srcWFrames[2] = 32;
	srcHFrames[2] = 4;

}

SDL_Texture* skeletonWarrior[4];
vector<Enemy> Enemies;

SDL_Rect renderBox[4];

void setupEnemyTexture(RenderWindow& window)
{
	skeletonWarrior[0] = window.loadTexture("resources/skeletonWarriorWalking.png");
	skeletonWarrior[1] = window.loadTexture("resources/skeletonWarriorAttacking.png");
	skeletonWarrior[2] = window.loadTexture("resources/skeletonWarriorDamaged.png");

	renderBox[0].x = -16;
	renderBox[0].y = -40;
	renderBox[0].w = 64;
	renderBox[0].h = 48;

	renderBox[1].x = -16;
	renderBox[1].y = -48;
	renderBox[1].w = 80;
	renderBox[1].h = 54;

	renderBox[2].x = -16;
	renderBox[2].y = -43;
	renderBox[2].w = 64;
	renderBox[2].h = 48;
}

int Enemy::getFrameDuration()
{
	return frameDuration;
}

float Enemy::getHealthPoints()
{
	return healthPoints;
}

void Enemy::setHealthPoints(float x)
{
	healthPoints = x;
}

bool Enemy::checkDeath()
{
	if (healthPoints <= 0)
		return true;
	return false;
}


float lastEnemyTime = 0;
void buildEnemies(float currentFrameTime)
{
	if ((int)Enemies.size() == EnemiesLimit)
		return;

    if (currentFrameTime - lastEnemyTime > 3000)
    {
    	int dir = mt() % 4;
    	int srcX;
    	int srcY;

    	switch (dir)
    	{
    			case 0:
    			{
    				srcX = mt() % LEVEL_WIDTH;
    				srcY = -(mt() % 201);
    				break;
    			}

    			case 1:
    			{
    				srcX = -(mt() % 201);
    				srcY = mt() % LEVEL_HEIGHT;
    				break;
    			}

    			case 2:
    			{
    				srcX = mt() % LEVEL_WIDTH;
    				srcY = LEVEL_HEIGHT + (mt() % 201);
    				break;
    			}

    			case 3:
    			{
    				srcX = LEVEL_WIDTH + (mt() % 201);
    				srcY = mt() % LEVEL_HEIGHT;
    				break;
    			}
    	}

    	Enemy skeleton(16, 40, 32, 4, srcX, srcY, 32 * 1.25, 4 * 1.25, skeletonWarrior[0]);
    	skeleton.setHealthPoints(60);
    	Enemies.push_back(skeleton);

    	lastEnemyTime = currentFrameTime;
    }
}

vector<Enemy> getEnemies()
{
	return Enemies;
}

SDL_Rect Enemy::getRenderBoxValues()
{
	return renderBox[state];
}

SDL_Rect Enemy::getHitBox()
{
	SDL_Rect hitBox = getDestFrame();

	hitBox.x += (12 * 1.25);
	hitBox.y -= (35 * 1.25);
	hitBox.w = (15 * 1.25);
	hitBox.h = (29 * 1.25);

	return hitBox;
}

SDL_Rect Enemy::getBodyBox()
{
	SDL_Rect BodyBox = getDestFrame();

	BodyBox.x += (16 * 1.25);
	BodyBox.y -= (26 * 1.25);
	BodyBox.w = (15 * 1.25);
	BodyBox.h = (19 * 1.25);

	return BodyBox;
}

void Enemy::updateFrame(float x, float y, float w, float h)
{
	setSrcX(x);
	setSrcY(y);
	setSrcW(w);
	setSrcH(h);
	setDesW(w * 1.25);
	setDesH(h * 1.25);
}

void Enemy::checkDamageEnemy(Player &player)
{
	if (frameDuration > 0 && state != 1)
		return;

	SDL_Rect a1 = player.getSwordBox(0);
	SDL_Rect a2 = player.getSwordBox(1);
	SDL_Rect b = getHitBox();
	if (SDL_HasIntersection(&a1, &b) || SDL_HasIntersection(&a2, &b))
	{
		takingDamage = true;

		float health = getHealthPoints();
		health -= 20;
		setHealthPoints(health);
	}
}



vector<pair<SDL_Rect, float> > damageBoxes;
bool checkDamagePlayer(Player& player)
{
	bool contact = false;

	for (auto b : damageBoxes)
	{
		SDL_Rect a = player.getHitBox();
		if (SDL_HasIntersection(&a, &b.first))
		{
			float health = player.getHealthPoints();
			health -= b.second;
			player.setHealthPoints(health);

			contact = true;
		}
	}

	return contact;
}

void Enemy::moveEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime)
{

	if (currentFrameTime - lastFramePos > 550)
	{
		targetX = player.getDesX() + randomValueX;
		targetY = player.getDesY() + randomValueY;
		lastFramePos = currentFrameTime;
	}

	if (frameDuration == 0 || takingDamage)
	{
		if (takingDamage)
		{
			if (state != 2)
				frameDuration = 5;

			state = 2;
			frame = 0;
			updateFrame(srcXFrames[state][frame], srcYFrames[state], srcWFrames[state], srcHFrames[state]);
		}
		else if (attackCooldown == 0 && abs(targetX - getHitBox().x) <= 50 && abs(targetY - getHitBox().y) <= 50)
		{
			attackCooldown = 200;

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

		if (attackCooldown > 0)
			attackCooldown--;
	}

	setTex(skeletonWarrior[state]);

	if (currentFrameTime - lastFrameTime > 85)
	{
		if (frameDuration > 0)
		{
			frameDuration--;
			if (frameDuration == 0)
				takingDamage = false;
		}

		frame++;
		frame %= maxFrames[state];

		updateFrame(srcXFrames[state][frame], srcYFrames[state], srcWFrames[state], srcHFrames[state]);

		lastFrameTime = currentFrameTime;
	}

	switch (state)
	{
		case 0:
		{
			float curX = getDestFrame().x;
			float curY = getDestFrame().y;

			float prev_X = curX;
			float prev_Y = curY;

			if (curX > targetX + movementSpeed)
			{
				facingLeft = false;
				curX -= movementSpeed;
			}
			else if (curX < targetX - movementSpeed)
			{
				facingLeft = true;
				curX += movementSpeed;
			}

			setDesX(curX);

			if (checkCollision(getDestFrame(), Obstacles))
				setDesX(prev_X);

			if (curY > targetY + movementSpeed)
				curY -= movementSpeed;
			else if (curY < targetY - movementSpeed)
				curY += movementSpeed;

			setDesY(curY);

			if (checkCollision(getDestFrame(), Obstacles))
				setDesY(prev_Y);

			break;
		}

		case 1:
		{
			if (frame == 7)
			{
				SDL_Rect sword;

				if (!facingLeft)
				{
					sword.x = getDesX() - (15 * 1.25);
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
	}

	if (!facingLeft)
		setFlip(SDL_FLIP_NONE);
	else
		setFlip(SDL_FLIP_HORIZONTAL);

	damageBoxes.push_back({getBodyBox(), 0.5});
}

void checkDamageEnemies(Player& player)
{
	for (Enemy &e : Enemies)
		e.checkDamageEnemy(player);
}

void moveEnemies(Player &player, vector<Entity> &Obstacles, float currentFrameTime)
{
	damageBoxes.clear();
	for (int i = 0; i < Enemies.size(); i++)
	{
		Enemies[i].moveEnemy(player, Obstacles, currentFrameTime);
		if (Enemies[i].checkDeath() && Enemies[i].getFrameDuration() == 0)
		{
			swap(Enemies[i], Enemies.back());
			Enemies.pop_back();
			i--;
		}
	}
}

void checkContactPlayer(Player &player)
{
	if (checkDamagePlayer(player))
		player.setStateTexture(1);
	else
		player.setStateTexture(0);
}

void renderEnemies(RenderWindow& window, SDL_Rect &camera)
{
	for (Enemy &e : Enemies)
		 window.render(e, camera);
}


