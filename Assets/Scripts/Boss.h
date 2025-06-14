#ifndef __BOSS__
#define __BOSS__

#include "Hitbox.h"
#include "Pistol.h"
#include "Player.h"
#include "Utils.h"

#define PISTOL_COOLDOWN_BOSS 40
#define POWER_COOLDOWN_BOSS 400
#define POWER_WIDTH 50
struct Power
{
    unsigned char isActive;
    unsigned short timer;
    short timerToLive;
    struct Position position; 
    struct Hitbox* hitbox;
};
struct Boss
{
    unsigned char health;
    unsigned char side;
    struct Hitbox* hitbox;
    struct Position position;
    struct Pistol* pistol;
    struct Power* power;
};

struct Boss* BossCreate(unsigned char side, struct Position position);
void BossUpdate(struct Boss* boss, struct Player* player);
struct Power* PowerCreate(int side, struct Position position);
#endif
