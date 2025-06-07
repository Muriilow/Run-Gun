#include <stdlib.h>
#include "Pistol.h"
#include "Player.h"
#include "NormalEnemy.h"
#include "Utils.h"

struct Pistol* PistolCreate()
{
    struct Pistol* newPistol = malloc(sizeof(struct Pistol));
    if(newPistol == NULL)
        return NULL;

    newPistol->timer = 0;
    newPistol->shots = NULL;

    return newPistol;
}

struct Bullet* PistolShot(unsigned short x, unsigned short y, struct Vector2 trajectory, float velocity, struct Pistol* gun)
{
    struct Bullet* newBullet = BulletCreate(x, y, trajectory, velocity, gun->shots);
    if(newBullet == NULL)
        return NULL;

    return newBullet;
}
void PistolDestroy(struct Pistol* pistol)
{
    struct Bullet* sentinel;
    for(struct Bullet* index = pistol->shots; index != NULL; index = sentinel)
    {
        sentinel = index->next;
        BulletDestroy(index);
    }

    free(pistol);
}
void PlayerBulletUpdate(struct Player* player)
{
    struct Bullet* previous = NULL;

    for(struct Bullet* index = player->pistol->shots; index != NULL;)
    {
        index->timerToLive--;

        index->x += index->trajectory.x * index->velocity;
        index->y += index->trajectory.y * index->velocity;
        index->hitbox->x = (unsigned short)index->x;
        index->hitbox->y = (unsigned short)index->y;

        al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(255, 0, 0));

        if(index->x > player->position.screenX || index->timerToLive == 0)
        {
            if(previous)
            {
                previous->next = index->next;
                BulletDestroy(index);
                index = previous->next;
            }
            else
            {
                player->pistol->shots = index->next;
                BulletDestroy(index);
                index = player->pistol->shots;
            }
        }
        else
        {
            previous = index;
            index = index->next;
        }
    }
}
void EnemyBulletUpdate(struct NormalEnemy* enemy, struct Player* player)
{
    struct Bullet* previous = NULL;

    for(struct Bullet* index = enemy->pistol->shots; index != NULL;)
    {
        index->timerToLive--;

        index->x += index->trajectory.x * index->velocity;
        index->y += index->trajectory.y * index->velocity;
        index->hitbox->x = (unsigned short)index->x;
        index->hitbox->y = (unsigned short)index->y;
        
        al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(255, 0, 0));

        if(index->x > enemy->position.screenX || index->timerToLive == 0)
        {
            if(previous)
            {
                previous->next = index->next;
                BulletDestroy(index);
                index = previous->next;
            }
            else
            {
                enemy->pistol->shots = index->next;
                BulletDestroy(index);
                index = enemy->pistol->shots;
            }
        }
        else
        {
            previous = index;
            index = index->next;
        }
    }
}

