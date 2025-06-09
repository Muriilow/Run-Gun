#ifndef __GROUND__
#define __GROUND__

#include "Utils.h"
#include "Hitbox.h"
struct Player;
struct Ground
{
    unsigned char side;
    struct Position position;
    struct Hitbox* hitbox;
};

struct Ground* GroundCreate(unsigned char side, struct Position position);
void GroundUpdate(struct Ground* ground, struct Player* player);
#endif
