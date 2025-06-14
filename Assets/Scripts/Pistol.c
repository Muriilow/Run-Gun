#include <stdlib.h>
#include "Pistol.h"
#include "Player.h"
#include "NormalEnemy.h"
#include "Boss.h"
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

struct Bullet* PistolShot(struct Position position, struct Vector2 trajectory, float velocity, struct Pistol* gun)
{
    struct Bullet* newBullet = BulletCreate(position, trajectory, velocity, gun->shots);
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

        index->position.x += index->trajectory.x * index->velocity;
        index->position.y += index->trajectory.y * index->velocity;
        index->position.worldX += index->trajectory.x * index->velocity;
        index->position.worldY += index->trajectory.y * index->velocity;
        index->hitbox->x = index->position.worldX;
        index->hitbox->y = index->position.worldY;

        al_draw_filled_circle(index->position.worldX - player->viewport->offsetX, index->position.worldY -
                              player->viewport->offsetY, 2, al_map_rgb(255, 0, 0));

        if(index->position.x > player->viewport->width || index->timerToLive == 0)
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
void BossBulletUpdate(struct Boss* boss, struct Player* player)
{
    struct Bullet* previous = NULL;

    for(struct Bullet* index = boss->pistol->shots; index != NULL;)
    {
        index->timerToLive--;

        index->position.x += index->trajectory.x * index->velocity;
        index->position.y += index->trajectory.y * index->velocity;
        index->position.worldX += index->trajectory.x * index->velocity;
        index->position.worldY += index->trajectory.y * index->velocity;
        index->hitbox->x = index->position.worldX;
        index->hitbox->y = index->position.worldY;
        
        al_draw_filled_circle(index->position.worldX - player->viewport->offsetX, index->position.worldY -
                              player->viewport->offsetY, 2, al_map_rgb(255, 0, 0));

        if(index->position.x > player->viewport->width || index->timerToLive == 0)
        {
            if(previous)
            {
                previous->next = index->next;
                BulletDestroy(index);
                index = previous->next;
            }
            else
            {
                boss->pistol->shots = index->next;
                BulletDestroy(index);
                index = boss->pistol->shots;
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

        index->position.x += index->trajectory.x * index->velocity;
        index->position.y += index->trajectory.y * index->velocity;
        index->position.worldX += index->trajectory.x * index->velocity;
        index->position.worldY += index->trajectory.y * index->velocity;
        index->hitbox->x = index->position.worldX;
        index->hitbox->y = index->position.worldY;
        
        al_draw_filled_circle(index->position.worldX - player->viewport->offsetX, index->position.worldY -
                              player->viewport->offsetY, 2, al_map_rgb(255, 0, 0));

        if(index->position.x > player->viewport->width || index->timerToLive == 0)
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

