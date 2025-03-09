#include "lib/Enemy.hpp"

using namespace std;

const int EnemiesLimit = 15;


Enemy::Enemy(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex)
:Entity(srcX, srcY, srcW, srcH, desX, desY, desW, desH, tex)
{
    movementSpeed = 1;

    frame = 0;

	lastFrameTime = lastFramePos = 0;

	state = 0;

	frameDuration = 0;

	attackCooldown = 0;

	facingLeft = false;

	maxFrames[0] = 8;
	maxFrames[1] = 11;

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
}

SDL_Texture* skeletonWarrior[4];
vector<Enemy> Enemies;

void setupEnemyTexture(RenderWindow& window)
{
	skeletonWarrior[0] = window.loadTexture("resources/skeletonWarriorWalking.png");
	skeletonWarrior[1] = window.loadTexture("resources/skeletonWarriorAttacking.png");
}

float lastEnemyTime = 0;
void buildEnemies(float currentFrameTime)
{
	if ((int)Enemies.size() == EnemiesLimit)
		return;

    if (currentFrameTime - lastEnemyTime > 3000)
    {
    	int dir = Rand(0, 3);
    	float srcX;
    	float srcY;

    	switch (dir)
    	{
    			case 0:
    			{
    				srcX = Rand(0, LEVEL_WIDTH);
    				srcY = Rand(-200, -1);
    				break;
    			}

    			case 1:
    			{
    				srcX = Rand(-200, -1);
    				srcY = Rand(0, LEVEL_HEIGHT);
    				break;
    			}

    			case 2:
    			{
    				srcX = Rand(0, LEVEL_WIDTH);
    				srcY = Rand(LEVEL_HEIGHT + 1, LEVEL_HEIGHT + 200);
    				break;
    			}

    			case 3:
    			{
    				srcX = Rand(LEVEL_WIDTH + 1, LEVEL_WIDTH + 200);
    				srcY = Rand(0, LEVEL_HEIGHT);
    				break;
    			}
    	}

    	Enemy skeleton(16, 40, 32, 4, srcX, srcY, 32 * 1.25, 4 * 1.25, skeletonWarrior[0]);
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
	SDL_Rect renderBox[4];

	renderBox[0].x = -16;
	renderBox[0].y = -40;
	renderBox[0].w = 64;
	renderBox[0].h = 48;

	renderBox[1].x = -16;
	renderBox[1].y = -48;
	renderBox[1].w = 80;
	renderBox[1].h = 54;

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

void Enemy::updateFrame(float x, float y, float w, float h)
{
	setSrcX(x);
	setSrcY(y);
	setSrcW(w);
	setSrcH(h);
	setDesW(w * 1.25);
	setDesH(h * 1.25);
}

vector<pair<SDL_Rect, float> > damageBoxes;
bool checkDamage(Player& player)
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

float targetX = 0, targetY = 0;
void Enemy::moveEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime)
{

	if (currentFrameTime - lastFramePos > 550)
	{
		targetX = player.getDesX();
		targetY = player.getDesY();
		lastFramePos = currentFrameTime;
	}

	if (frameDuration == 0)
	{
		if (attackCooldown == 0 && abs(targetX - getHitBox().x) <= 50 && abs(targetY - getHitBox().y) <= 50)
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
			frameDuration--;

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

			if (curY > targetY + movementSpeed)
				curY -= movementSpeed;
			else if (curY < targetY - movementSpeed)
				curY += movementSpeed;

			setDesX(curX);
			setDesY(curY);

			if (checkCollision(getDestFrame(), Obstacles))
			{
				setDesX(prev_X);
				setDesY(prev_Y);
			}

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

	damageBoxes.push_back({getHitBox(), 0.5});
}

void moveEnemies(Player &player, vector<Entity> &Obstacles, float currentFrameTime)
{
	damageBoxes.clear();
	for (Enemy &e : Enemies)
		e.moveEnemy(player, Obstacles, currentFrameTime);
}

void checkContactEnemies(Player &player)
{
	if (checkDamage(player))
		player.setStateTexture(1);
	else
		player.setStateTexture(0);
}

void renderEnemies(RenderWindow& window, SDL_Rect &camera)
{
	for (Enemy &e : Enemies)
		 window.render(e, camera);
}


