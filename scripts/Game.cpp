#include "lib/Game.hpp"

using namespace std;

vector<Enemy*> Enemies;

vector<pair<SDL_Rect, float> > damageBoxes;

void setupEnemyTexture(RenderWindow& window)
{
	setUpProjectileTexture(window);
	setUpSkeletonTexture(window);
	setUpGoblinTexture(window);
	setUpOrcTexture(window);
	setUpNecromancerTexture(window);
}

float lastEnemyTime = 0;

int EnemiesLimit = 0;
int EnemiesCount = 0;
int wave = 0;

vector<int> EnemiesReserve;

void newWave()
{
 	Enemies.clear();

 	wave++;
 	EnemiesCount = 0;
	EnemiesLimit = (wave * 5) + (wave * 2) + (wave / 3) + ((wave / 5) * 2);
 	EnemiesReserve.clear();
 	for (int i = 1; i <= wave * 5; i++)
 		 EnemiesReserve.push_back(0);
 	for (int i = 1; i <= wave * 2; i++)
 		 EnemiesReserve.push_back(1);
 	for (int i = 1; i <= wave / 3; i++)
 		 EnemiesReserve.push_back(2);
	for (int i = 1; i <= (wave / 5) * 2; i++)
		 EnemiesReserve.push_back(3);

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

     		case 3:
     		{
     			Enemies.push_back(new Necromancer(desX, desY));
     			break;
     		}
     	}

     	EnemiesCount++;

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

 		if (Goblin* enemy = dynamic_cast<Goblin*>(e))
 			enemy -> checkDamageEnemy(player);

 		if (Orc* enemy = dynamic_cast<Orc*>(e))
 			enemy->checkDamageEnemy(player);

 		if (Necromancer* enemy = dynamic_cast<Necromancer*>(e))
 			enemy->checkDamageEnemy(player);
 	}
 }

void updateEnemies(Player &player, vector<Entity> &Obstacles, float currentFrameTime)
 {
 	if ((int)Enemies.size() == 0 && EnemiesCount == EnemiesLimit)
 	{
 		newWave();
 		player.levelUp(wave);
 		return;
 	}

 	damageBoxes.clear();
 	for (Enemy *e : Enemies)
 	{
 		bool isEnemy = 1;
 		if (Projectile* enemy = dynamic_cast<Projectile*>(e))
 		{
 			enemy->updateEnemy(currentFrameTime, damageBoxes);
 			isEnemy = 0;
 		}

 		if (Skeleton* enemy = dynamic_cast<Skeleton*>(e))
 			enemy->updateEnemy(player, Obstacles, currentFrameTime, damageBoxes);

 		if (Goblin* enemy = dynamic_cast<Goblin*>(e))
 			enemy->updateEnemy(player, Obstacles, currentFrameTime, damageBoxes);

 		if (Orc* enemy = dynamic_cast<Orc*>(e))
 			enemy->updateEnemy(player, Obstacles, currentFrameTime, damageBoxes, Enemies);

 		if (Necromancer* enemy = dynamic_cast<Necromancer*>(e))
 			enemy->updateEnemy(player, Obstacles, currentFrameTime, damageBoxes, Enemies);

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