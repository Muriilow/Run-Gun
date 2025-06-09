#ifndef __PISTOL__
#define __PISTOL__

#define X_SCREEN 320 
#define Y_SCREEN 320
#define LEFT 0 
#define RIGHT 1

#include "Bullet.h"
#include "Utils.h"

#define PISTOL_COOLDOWN 10
struct Player;
struct NormalEnemy;
struct Pistol 
{
    unsigned char timer;
    struct Bullet* shots;
};

void PlayerBulletUpdate(struct Player* player);
void EnemyBulletUpdate(struct NormalEnemy* enemy, struct Player* player);

struct Pistol* PistolCreate();
struct Bullet* PistolShot(struct Position position, struct Vector2 trajectory, float velocity, struct Pistol* gun);
void PistolDestroy(struct Pistol* pistol);

#endif
