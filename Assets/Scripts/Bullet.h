#ifndef __BULLET__
#define __BULLET__

#define BULLET_MOVE 5
#define X_SCREEN 320 
#define Y_SCREEN 320
#define LEFT 0 
#define RIGHT 1

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "Hitbox.h"
#include "Utils.h"

struct Player;
struct Bullet 
{
    float velocity;
    struct Position position;
    unsigned char timerToLive;
    struct Vector2 trajectory;
    struct Hitbox* hitbox;
    struct Bullet* next;
};

struct Bullet* BulletCreate(struct Position position, struct Vector2 trajectory, float velocity, struct Bullet* next, unsigned int hitboxSize);
void BulletDestroy(struct Bullet* bullet);
#endif
