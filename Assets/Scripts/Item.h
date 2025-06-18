#ifndef __ITEM__
#define __ITEM__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "Hitbox.h"
#include "Utils.h"

#define HEALTH 0 
#define DOUBLE_JUMP 1

struct Player;
struct Item 
{
    ALLEGRO_BITMAP* sprite;
    unsigned char type;
    unsigned char hasCollide;
    struct Hitbox* hitbox;
    struct Position position;
    struct Item* next;
};

struct Item* ItemCreate(struct Position position, struct Item* next, unsigned char type, ALLEGRO_BITMAP* sprite);

void ItemUpdate(struct Item* item, struct Player* player);
#endif
