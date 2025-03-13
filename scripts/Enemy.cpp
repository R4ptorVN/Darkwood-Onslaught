#include "lib/Enemy.hpp"
#include<bits/stdc++.h>

using namespace std;

SDL_Texture* skeletonTexture[5];
SDL_Texture* orcTexture[5];
vector<Enemy*> Enemies;


void setupEnemyTexture(RenderWindow& window)
{
	skeletonTexture[0] = window.loadTexture("resources/Enemies/Skeleton/skeletonWalking.png");
	skeletonTexture[1] = window.loadTexture("resources/Enemies/Skeleton/skeletonAttacking.png");
	skeletonTexture[2] = window.loadTexture("resources/Enemies/Skeleton/skeletonBlocking.png");
	skeletonTexture[3] = window.loadTexture("resources/Enemies/Skeleton/skeletonDamaged.png");
	skeletonTexture[4] = window.loadTexture("resources/Enemies/Skeleton/skeletonDeath.png");

	orcTexture[0] = window.loadTexture("resources/Enemies/Orc/orcWalking.png");
	orcTexture[1] = window.loadTexture("resources/Enemies/Orc/orcAttacking.png");
	orcTexture[3] = window.loadTexture("resources/Enemies/Orc/orcDamaged.png");
	orcTexture[4] = window.loadTexture("resources/Enemies/Orc/orcDeath.png");
}

Enemy::Enemy(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex)
:Entity(srcX, srcY, srcW, srcH, desX, desY, desW, desH, tex)
{
	takingDamage = false;

    frame = 0;

	lastFrameTime = lastFramePos = 0;

	state = 0;

	targetX = targetY = 0;
	randomValueX = (mt() % 15) - (mt() % 15);
	randomValueY = (mt() % 15) - (mt() % 15);

	frameDuration = 0;

	actionCooldown = 0;

	facingLeft = true;
}

int Enemy::getState()
{
	return state;
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

int EnemiesLimit = 0;
int EnemiesCount = 0;
int EnemiesLeft = 0;
int wave = 0;

vector<int> EnemiesReserve;
void newWave()
{
	wave++;
	EnemiesCount = 0;
	EnemiesLimit = (wave * 5) + (wave / 3);
	EnemiesReserve.clear();
	for (int i = 1; i <= 5 * wave; i++)
		 EnemiesReserve.push_back(0);
	for (int i = 1; i <= wave / 3; i++)
		 EnemiesReserve.push_back(1);

	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	shuffle(EnemiesReserve.begin(), EnemiesReserve.end(), default_random_engine(seed));
}

int getWave()
{
	return wave;
}

void spawnEnemies(float currentFrameTime)
{
	if (EnemiesCount == EnemiesLimit)
		return;

    if (currentFrameTime - lastEnemyTime > 2000)
    {
    	int dir = mt() % 4;
    	int desX;
    	int desY;

    	switch (dir)
    	{
    			case 0:
    			{
    				desX = mt() % LEVEL_WIDTH;
    				desY = -(mt() % 201);
    				break;
    			}

    			case 1:
    			{
    				desX = -(mt() % 201);
    				desY = mt() % LEVEL_HEIGHT;
    				break;
    			}

    			case 2:
    			{
    				desX = mt() % LEVEL_WIDTH;
    				desY = LEVEL_HEIGHT + (mt() % 201);
    				break;
    			}

    			case 3:
    			{
    				desX = LEVEL_WIDTH + (mt() % 201);
    				desY = mt() % LEVEL_HEIGHT;
    				break;
    			}
    	}

    	if (EnemiesReserve[EnemiesCount] == 0)
    		Enemies.push_back(new Skeleton(desX, desY));
    	if (EnemiesReserve[EnemiesCount] == 1)
    		Enemies.push_back(new Orc(desX, desY));

    	EnemiesCount++;
    	EnemiesLeft++;

    	lastEnemyTime = currentFrameTime;
    }
}

vector<Enemy> getEnemies()
{
	vector<Enemy> EnemiesRef;
	for (Enemy* e: Enemies)
		 EnemiesRef.push_back(*e);
	return EnemiesRef;
}

SDL_Rect Enemy::getRenderBoxValues()
{
	return renderBox[state];
}

void Enemy::updateFrame(float x, float y, float w, float h)
{
	setSrcX(x); setSrcY(y);
	setSrcW(w); setSrcH(h);
	setDesW(w * 1.25); setDesH(h * 1.25);
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

void checkDamageEnemies(Player& player)
{
	for (Enemy* e : Enemies)
	{
		if (Skeleton* enemy = dynamic_cast<Skeleton*>(e))
			enemy->checkDamageEnemy(player);
		if (Orc* enemy = dynamic_cast<Orc*>(e))
			enemy->checkDamageEnemy(player);
	}
}

void moveEnemies(Player &player, vector<Entity> &Obstacles, float currentFrameTime)
{
	if (EnemiesLeft == 0 && EnemiesCount == EnemiesLimit)
	{
		newWave();
		player.levelUp();
		return;
	}

	damageBoxes.clear();
	for (Enemy *e : Enemies)
	{
		if (Skeleton* enemy = dynamic_cast<Skeleton*>(e))
			enemy->moveEnemy(player, Obstacles, currentFrameTime);

		if (Orc* enemy = dynamic_cast<Orc*>(e))
			enemy->moveEnemy(player, Obstacles, currentFrameTime);
	}

	for (int i = 0; i < Enemies.size(); i++)
	{
		if ((*Enemies[i]).getState() == 4 && (*Enemies[i]).getFrameDuration() == 0)
		{
			swap(Enemies[i], Enemies.back());
			Enemies.pop_back();
			i--;

			EnemiesLeft--;
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
	for (Enemy* e : Enemies)
		 window.render(*e, camera);
}

Skeleton::Skeleton(float desX, float desY)
:Enemy(32, 41, 16, 4, desX, desY, 16 * 1.25, 4 * 1.25, skeletonTexture[0])
{
	movementSpeed = 1;

	healthPoints = 60;

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

void Skeleton::moveEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime)
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
			actionCooldown = 200;

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

		if (actionCooldown > 0)
			actionCooldown--;
	}

	setTex(skeletonTexture[state]);

	if (currentFrameTime - lastFrameTime > 85)
	{
		if (frameDuration > 0)
		{
			frameDuration--;
			if (frameDuration == 0)
				takingDamage = false;
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
			float curX = getDesX();
			float curY = getDesY();

			float prev_X = curX;
			float prev_Y = curY;

			if (curX > targetX + movementSpeed)
			{
				facingLeft = true;
				curX -= movementSpeed;
			}
			else if (curX < targetX - movementSpeed)
			{
				facingLeft = false;
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
	}

	if (facingLeft)
		setFlip(SDL_FLIP_NONE);
	else
		setFlip(SDL_FLIP_HORIZONTAL);

	if (state != 4)
	    damageBoxes.push_back({getBodyBox(), 0.5});
}

Orc::Orc(float desX, float desY)
:Enemy(48, 68, 32, 5, desX, desY, 32 * 1.25, 5 * 1.25, orcTexture[0])
{
	movementSpeed = 0.75;

	healthPoints = 300;

	maxFrames[0] = 8;
	maxFrames[1] = 10;
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

void Orc::moveEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime)
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
			actionCooldown = 200;

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

		if (actionCooldown > 0)
			actionCooldown--;
	}

	setTex(orcTexture[state]);

	if (currentFrameTime - lastFrameTime > 110)
	{
		if (frameDuration > 0)
		{
			frameDuration--;
			if (frameDuration == 0)
				takingDamage = false;
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
			float curX = getDesX();
			float curY = getDesY();

			float prev_X = curX;
			float prev_Y = curY;

			if (curX > targetX + movementSpeed)
			{
				facingLeft = true;
				curX -= movementSpeed;
			}
			else if (curX < targetX - movementSpeed)
			{
				facingLeft = false;
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

				damageBoxes.push_back({axe, 8});
			}
		}
	}

	if (facingLeft)
		setFlip(SDL_FLIP_NONE);
	else
		setFlip(SDL_FLIP_HORIZONTAL);

	if (state != 4)
		damageBoxes.push_back({getBodyBox(), 0.5});
}
