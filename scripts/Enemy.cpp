#include "lib/Enemy.hpp"
#include<bits/stdc++.h>

using namespace std;

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