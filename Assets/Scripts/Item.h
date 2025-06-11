#ifndef __ITEM__
#define __ITEM__

#include "Hitbox.h"
#include "Utils.h"
struct Player;
struct Item 
{
    unsigned char hasCollide;
    unsigned char side;
    struct Hitbox* hitbox;
    struct Position position;
    struct Item* next;
};

struct Item* ItemCreate(unsigned char side, struct Position position, struct Item* next);

void ItemUpdate(struct Item* item, struct Player* player);
#endif
