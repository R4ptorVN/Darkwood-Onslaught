#include "lib/Enemy.hpp"
#include<bits/stdc++.h>

using namespace std;

SDL_Texture* skeletonTexture[5];
SDL_Texture* goblinTexture[5];
SDL_Texture* orcTexture[5];

SDL_Texture* projectileTexture[1];

vector<Enemy*> Enemies;

SDL_Rect spikes[8];
float srcXSpikeFrames[30];

void setupEnemyTexture(RenderWindow& window)
{
	skeletonTexture[0] = window.loadTexture("resources/Enemies/Skeleton/skeletonWalking.png");
	skeletonTexture[1] = window.loadTexture("resources/Enemies/Skeleton/skeletonAttacking.png");
	skeletonTexture[2] = window.loadTexture("resources/Enemies/Skeleton/skeletonBlocking.png");
	skeletonTexture[3] = window.loadTexture("resources/Enemies/Skeleton/skeletonDamaged.png");
	skeletonTexture[4] = window.loadTexture("resources/Enemies/Skeleton/skeletonDeath.png");

	goblinTexture[0] = window.loadTexture("resources/Enemies/Goblin/goblinWalking.png");
	goblinTexture[1] = window.loadTexture("resources/Enemies/Goblin/goblinRunning.png");
	goblinTexture[2] = window.loadTexture("resources/Enemies/Goblin/goblinAttacking.png");
	goblinTexture[3] = window.loadTexture("resources/Enemies/Goblin/goblinDamaged.png");
	goblinTexture[4] = window.loadTexture("resources/Enemies/Goblin/goblinDeath.png");

	orcTexture[0] = window.loadTexture("resources/Enemies/Orc/orcWalking.png");
	orcTexture[1] = window.loadTexture("resources/Enemies/Orc/orcAttacking.png");
	orcTexture[2] = window.loadTexture("resources/Enemies/Orc/orcSummonSpike.png");
	orcTexture[3] = window.loadTexture("resources/Enemies/Orc/orcDamaged.png");
	orcTexture[4] = window.loadTexture("resources/Enemies/Orc/orcDeath.png");

	projectileTexture[0] = window.loadTexture("resources/Enemies/Orc/orcSpike.png");

	spikes[0] = makeRec(5, 56, 10, 3);
	spikes[1] = makeRec(26, 45, 10, 3);
	spikes[2] = makeRec(35, 61, 10, 3);
	spikes[3] = makeRec(46, 75, 10, 3);
	spikes[4] = makeRec(48, 34, 10, 3);
	spikes[5] = makeRec(57, 49, 10, 3);
	spikes[6] = makeRec(69, 64, 10, 3);
	spikes[7] = makeRec(89, 53, 10, 3);
}

Enemy::Enemy(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex)
:Entity(srcX, srcY, srcW, srcH, desX, desY, desW, desH, tex)
{
	takingDamage = false;

    frame = 0;

	lastFrameTime = lastFramePos = 0;

	state = 0;

	frameDuration = 0;

	actionCooldown = 0;

	facingLeft = true;

	knockBack = 0;
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
	Enemies.clear();

	wave++;
	EnemiesCount = 0;
	EnemiesLimit = (wave * 5) + (wave * 2) + (wave / 3);
	EnemiesReserve.clear();
	for (int i = 1; i <= wave * 5; i++)
		 EnemiesReserve.push_back(0);
	for (int i = 1; i <= wave * 2; i++)
		 EnemiesReserve.push_back(1);
	for (int i = 1; i <= wave / 3; i++)
		 EnemiesReserve.push_back(2);

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

    	switch (EnemiesReserve[EnemiesCount])
    	{
    		case 0:
    		{
    			Enemies.push_back(new Skeleton(desX, desY));
    			break;
    		}

    		case 1:
    		{
    			Enemies.push_back(new Goblin(desX, desY));
    			break;
    		}

    		case 2:
    		{
    			Enemies.push_back(new Orc(desX, desY));
    			break;
    		}
    	}

    	EnemiesCount++;
    	EnemiesLeft++;

    	lastEnemyTime = currentFrameTime;
    }
}

vector<Enemy> getEnemies()
{
	vector<Enemy> EnemiesRef;
	EnemiesRef.clear();

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

bool contact[4];

void Enemy::checkSkill(SDL_Rect hitBox, Player &player)
{
	for (int i = 0; i < 4; i++)
		contact[i] = false;

	int contactBoxes = 0;

	for (int i = 0; i < 4; i++)
	{
		SDL_Rect box = player.getSkillBox(i);

		if (SDL_HasIntersection(&box, &hitBox))
		{
			contact[i] = true;
			contactBoxes++;
		}

	}

	if (contactBoxes > 0)
	{
		takingDamage = true;

		float health = getHealthPoints();
		health -= (player.getAttackingDamage() * 2);
		setHealthPoints(health);
	}

	if (contactBoxes > 2)
	{
		if (!facingLeft)
			knockBack = 7;
		else
			knockBack = 3;
		return;
	}
	if (contact[0] && contact[1])
		knockBack = 1;
	else if (contact[1] && contact[3])
		knockBack = 3;
	else if (contact[2] && contact[3])
		knockBack = 5;
	else if (contact[0] && contact[2])
		knockBack = 7;
	else if (contact[0])
		knockBack = 8;
	else if (contact[1])
		knockBack = 2;
	else if (contact[2])
		knockBack = 6;
	else if (contact[3])
		knockBack = 4;
}

void Enemy::moveEnemy(vector<Entity> &Obstacles)
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
}

void Enemy::knockBackEnemy(vector<Entity> &Obstacles)
{
	float curX = getDesX();
	float curY = getDesY();

	float prev_X = curX;
	float prev_Y = curY;

	switch (knockBack)
	{
		case 1:
		{
			curY -= (movementSpeed * 3);
			break;
		}

		case 2:
		{
			curX += (movementSpeed * 3);
			curY -= (movementSpeed * 3);
			break;
		}

		case 3:
		{
			curX += (movementSpeed * 3);
			break;
		}

		case 4:
		{
			curX += (movementSpeed * 3);
			curY += (movementSpeed * 3);
			break;
		}

		case 5:
		{
			curY += (movementSpeed * 3);
			break;
		}

		case 6:
		{
			curX -= (movementSpeed * 3);
			curY += (movementSpeed * 3);
			break;
		}

		case 7:
		{
			curX -= (movementSpeed * 3);
			break;
		}

		case 8:
		{
			curX -= (movementSpeed * 3);
			curY -= (movementSpeed * 3);
			break;
		}
	}

	setDesX(curX);
	setDesY(curY);

	if (checkCollision(getDestFrame(), Obstacles))
	{
		setDesX(prev_X);
		setDesY(prev_Y);
	}
}

void checkDamageEnemies(Player& player)
{
	for (Enemy* e : Enemies)
	{
		if (Skeleton* enemy = dynamic_cast<Skeleton*>(e))
			enemy->checkDamageEnemy(player);

		if (Goblin* enemy = dynamic_cast<Goblin*>(e))
			enemy -> checkDamageEnemy(player);

		if (Orc* enemy = dynamic_cast<Orc*>(e))
			enemy->checkDamageEnemy(player);
	}
}

void updateEnemies(Player &player, vector<Entity> &Obstacles, float currentFrameTime)
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
		bool isEnemy = 1;
		if (Projectile* enemy = dynamic_cast<Projectile*>(e))
		{
			enemy->updateEnemy(currentFrameTime);
			isEnemy = 0;
		}

		if (Skeleton* enemy = dynamic_cast<Skeleton*>(e))
			enemy->updateEnemy(player, Obstacles, currentFrameTime);

		if (Goblin* enemy = dynamic_cast<Goblin*>(e))
			enemy->updateEnemy(player, Obstacles, currentFrameTime);

		if (Orc* enemy = dynamic_cast<Orc*>(e))
			enemy->updateEnemy(player, Obstacles, currentFrameTime);

		if (isEnemy && (*e).getState() == 4 && (*e).getFrameDuration() == 0)
			player.increaseMana();
	}

	for (int i = 0; i < int(Enemies.size()); i++)
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

int cnt = 0;
Projectile::Projectile(int type, float srcX, float srcY, float desX, float desY)
:Enemy(srcX, srcY, 0, 0, desX, desY, 0, 0, NULL)
{
	if (type == 1)
	{
		maxFrames[4] = 35;

		srcXFrames[4][0] = srcX;
		srcYFrames[4] = srcY;
		srcWFrames[4] = 10;
		srcHFrames[4] = 3;

		for (int i = 1; i < maxFrames[4]; i++)
			srcXFrames[4][i] = srcXFrames[4][i - 1] + 99;

		renderBox[4] = makeRec(0, -34, 10, 37);


		hitBox.x = desX + (1 * 1.25);
		hitBox.y = desY - (22 * 1.25);
		hitBox.w = 8 * 1.25;
		hitBox.h = 12 * 1.25;
	}

	setTex(projectileTexture[type - 1]);

	state = 4;

	frameDuration = maxFrames[4];
}

void Projectile::updateEnemy(float currentFrameTime)
{
	if (currentFrameTime - lastFrameTime > 130)
	{
		if (frameDuration > 0)
			frameDuration--;

		frame++;
		frame %= maxFrames[state];

		updateFrame(srcXFrames[state][frame], srcYFrames[state], srcWFrames[state], srcHFrames[state]);


		lastFrameTime = currentFrameTime;
	}

	if (frame > 6 && frame < 16)
		damageBoxes.push_back({hitBox, 0.5});
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

void Skeleton::updateEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime)
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

void Goblin::updateEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime)
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
			movementSpeed = 1.75;
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
		damageBoxes.push_back({getBodyBox(), 0.5});
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

void Orc::updateEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime)
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

				damageBoxes.push_back({axe, 8});
			}

			break;
		}

		case 2:
		{
			if (actionCooldown == 349)
			{
				for (int i = 0; i < 8; i++)
					Enemies.push_back(new Projectile(1, spikes[i].x, spikes[i].y, player.getHitBox().x + (spikes[i].x * 1.25) - 60, player.getHitBox().y + (spikes[i].y * 1.25) - 20));

				EnemiesLeft += 8;
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