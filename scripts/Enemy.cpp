#include "lib/Enemy.hpp"

using namespace std;

const int MonsterLimit = 15;

const int walkingFrames = 8;

Enemy::Enemy(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex)
:Entity(srcX, srcY, srcW, srcH, desX, desY, desW, desH, tex)
{
    movementSpeed = 1;

    frame = 0;

	lastFrameTime = lastFramePos = 0;

    actionStatus = 0;

	srcXFrames[0] = 16;

	for(int i = 1; i < walkingFrames; i++)
		srcXFrames[i] = srcXFrames[i - 1] + 64;
}

SDL_Texture* skeletonWarrior;
vector<Enemy> Enemies;

void setupEnemyTexture(RenderWindow& window)
{
	skeletonWarrior = window.loadTexture("resources/skeletonWarrior.png");
}

void buildEnemies()
{
	if ((int)Enemies.size() == MonsterLimit)
		return;

	Enemy skeleton(16, 40, 32, 4, 500, 500, 32 * 1.25, 4 * 1.25, skeletonWarrior);
    Enemies.push_back(skeleton);
}

vector<Enemy> getEnemies()
{
	return Enemies;
}

SDL_Rect Enemy::getRenderBoxValues()
{
	SDL_Rect renderBox;

	renderBox.x = -16;
	renderBox.y = -40;
	renderBox.w = 64;
	renderBox.h = 48;

	return renderBox;
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


float targetX = 0, targetY = 0;
void Enemy::moveEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime)
{

	if (currentFrameTime - lastFrameTime > 125)
	{
		frame++;
		if (frame == walkingFrames)
			frame = 0;
		lastFrameTime = currentFrameTime;
	}

	if (currentFrameTime - lastFramePos > 550)
	{
		targetX = player.getDesX();
		targetY = player.getDesY();
		lastFramePos = currentFrameTime;
	}

	setSrcX(srcXFrames[frame]);

	float curX = getDestFrame().x;
	float curY = getDestFrame().y;

	float prev_X = curX;
	float prev_Y = curY;

	if (curX > targetX + movementSpeed)
	{
		setFlip(SDL_FLIP_NONE);
		curX -= movementSpeed;
	}
	else if (curX < targetX - movementSpeed)
	{
		setFlip(SDL_FLIP_HORIZONTAL);
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

	SDL_Rect a = getHitBox();
	SDL_Rect b = player.getHitBox();

	if (SDL_HasIntersection(&a, &b))
	{
		float health = player.getHealthPoints();
		health -= 0.75;
		player.setHealthPoints(health);
	}
}

void moveEnemies(Player &player, vector<Entity> &Obstacles, float currentFrameTime)
{
	for (Enemy &e : Enemies)
		e.moveEnemy(player, Obstacles, currentFrameTime);
}

void renderEnemies(RenderWindow& window, SDL_Rect &camera)
{
	for (Enemy &e : Enemies)
		 window.render(e, camera);
}


