#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Bullet.h"

struct Bullet* BulletCreate(float x, float y, struct Vector2 trajectory, float velocity, struct Bullet* next)
{
    struct Bullet* newBullet = malloc(sizeof(struct Bullet));
    if(newBullet == NULL)
        return NULL;

    newBullet->velocity = velocity;
    newBullet->x = x;
    newBullet->y = y;
    newBullet->timerToLive = 180;
    newBullet->trajectory = trajectory;
    newBullet->hitbox = HitboxCreate(5, 5, (unsigned short)x, (unsigned short)y);
    newBullet->next = next;

    return newBullet;
}


void BulletDestroy(struct Bullet* bullet)
{
    free(bullet);
}
