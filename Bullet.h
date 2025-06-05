#ifndef __BULLET__
#define __BULLET__

#define BULLET_MOVE 5
#define X_SCREEN 320 
#define Y_SCREEN 320
#define LEFT 0 
#define RIGHT 1

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "Hitbox.h"

struct Player;
struct Bullet 
{
    unsigned short x;
    unsigned short y;
    unsigned char timerToLive;
    unsigned char trajectory;
    struct Hitbox* hitbox;
    struct Bullet* next;
};

struct Bullet* BulletCreate(unsigned short x, unsigned short y, unsigned char trajectory, struct Bullet* next);
void BulletUpdate(struct Player* player);
void BulletDestroy(struct Bullet* bullet);
#endif
