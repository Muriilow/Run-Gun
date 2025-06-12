#ifndef __BOSS__
#define __BOSS__

#include "Hitbox.h"
#include "Pistol.h"
#include "Player.h"
#include "Utils.h"

#define PISTOL_COOLDOWN_BOSS 10;
struct Boss
{
    unsigned char health;
    unsigned char side;
    struct Hitbox* hitbox;
    struct Position position;
    struct Pistol* pistol;
};

struct Boss* BossCreate(unsigned char side, struct Position position);
void BossUpdate(struct Boss* boss, struct Player* player);
#endif
