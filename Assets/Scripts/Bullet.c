#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Bullet.h"

struct Bullet* BulletCreate(struct Position position, struct Vector2 trajectory, float velocity, struct Bullet* next)
{
    struct Bullet* newBullet = malloc(sizeof(struct Bullet));
    if(newBullet == NULL)
        return NULL;

    newBullet->velocity = velocity;
    newBullet->position = position;
    newBullet->timerToLive = 180;
    newBullet->trajectory = trajectory;
    newBullet->hitbox = HitboxCreate(5, 5, position.x, position.y);
    newBullet->next = next;

    return newBullet;
}


void BulletDestroy(struct Bullet* bullet)
{
    free(bullet);
}
