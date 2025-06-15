#ifndef __PISTOL__
#define __PISTOL__
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "Bullet.h"
#include "Utils.h"

#define X_SCREEN 320 
#define Y_SCREEN 320
#define LEFT 0 
#define RIGHT 1

#define PISTOL_COOLDOWN 10
struct Player;
struct NormalEnemy;
struct Boss;
struct Pistol 
{
    ALLEGRO_BITMAP* sprite;
    unsigned char cooldown; 
    unsigned char timer;
    unsigned char hitboxSize;
    struct Bullet* shots;
};

void PlayerBulletUpdate(struct Player* player);
void EnemyBulletUpdate(struct NormalEnemy* enemy, struct Player* player);
void BossBulletUpdate(struct Boss* boss, struct Player* player);

struct Pistol* PistolCreate(unsigned char cooldown, ALLEGRO_BITMAP* sprite, unsigned char hitboxSize);
struct Bullet* PistolShot(struct Position position, struct Vector2 trajectory, float velocity, struct Pistol* gun);
void PistolDestroy(struct Pistol* pistol);

#endif
