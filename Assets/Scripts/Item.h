#ifndef __ITEM__
#define __ITEM__

#include "Hitbox.h"
#include "Utils.h"

#define HEALTH 0 
#define DOUBLE_JUMP 1

struct Player;
struct Item 
{
    unsigned char type;
    unsigned char hasCollide;
    unsigned char side;
    struct Hitbox* hitbox;
    struct Position position;
    struct Item* next;
};

struct Item* ItemCreate(unsigned char side, struct Position position, struct Item* next, unsigned char type);

void ItemUpdate(struct Item* item, struct Player* player);
#endif
