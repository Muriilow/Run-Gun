#ifndef __NORMAL_EN__
#define __NORMAL_EN__

#define PISTOL_COOLDOWN_ENEMY 20
#include "Hitbox.h"
#include "Pistol.h"
#include "Player.h"
#include "Utils.h"

struct NormalEnemy
{
    unsigned char health;
    unsigned char side;
    struct Hitbox* hitbox;
    struct Position position;
    struct Pistol* pistol;
};

struct NormalEnemy* NormalEnemyCreate(unsigned char side, struct Position position);
void NormalEnemyUpdate(struct NormalEnemy* enemy, struct Player* player);
#endif
