#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "NormalEnemy.h"

struct NormalEnemy* NormalEnemyCreate(unsigned char side, struct Position position)
{
    struct NormalEnemy* enemy;

    enemy = malloc(sizeof(struct NormalEnemy));

    if(enemy == NULL)
        exit(EXIT_FAILURE);

    enemy->health = 5;
    enemy->side = side;
    enemy->position = position;
    enemy->hitbox = HitboxCreate(side, side, position.x, position.y);
    enemy->pistol = PistolCreate();
    return enemy;
}
void NormalEnemyShot(struct NormalEnemy* enemy, struct Player* player)
{
    struct Bullet* shot = NULL;
    short distanceX =  player->position.worldX - enemy->position.worldX;
    short distanceY = player->position.worldY - enemy->position.worldY;
    float angle = atan2(distanceY, distanceX);
    struct Vector2 traj = {cos(angle), sin(angle)};

    shot = PistolShot(enemy->position, traj, 6., enemy->pistol);

    if(shot)
        enemy->pistol->shots = shot;
}
void CheckDistance(struct NormalEnemy* enemy, struct Player* player)
{
    short distanceX = enemy->position.worldX - player->position.worldX;
    short distanceY = enemy->position.worldY - player->position.worldY;
    float distance = sqrt(distanceX*distanceX + distanceY*distanceY);
    
    if(distance < 200 && enemy->pistol->timer == 0)
    {
        enemy->pistol->timer = PISTOL_COOLDOWN_ENEMY;
        NormalEnemyShot(enemy, player);
    }
}
void CheckCollision(struct NormalEnemy* enemy, struct Player* player)
{
    struct Bullet* previous = NULL;

    /* Checking if any bullet is colliding with me */
    for(struct Bullet* index = player->pistol->shots; index != NULL;)
    {
        if(HitboxCheck(index->hitbox, enemy->hitbox))
        {
            enemy->health--;

            if(previous != NULL)
            {
                previous->next = index->next;
                BulletDestroy(index);
                index = previous->next;
                break;
            }
            else
            {
                player->pistol->shots = index->next;
                BulletDestroy(index);
                index = player->pistol->shots;
                break;
            }
        }
        else
        {
            previous = index;
            index = index->next;
        }
    }
}
void NormalEnemyUpdate(struct NormalEnemy* enemy, struct Player* player)
{
    if(enemy == NULL || player == NULL)
        return;

    unsigned char size = enemy->side/2;
    if(enemy->pistol->timer)
        enemy->pistol->timer--;
    
    /* If enemy died */
    if(enemy->health == 0)
    {
        PistolDestroy(enemy->pistol);
        HitboxDestroy(enemy->hitbox);
        free(enemy);

        enemy = NULL;
        return;
    }
    /*Updating our screen x and y positions*/
    enemy->position.x = enemy->position.worldX - player->viewport->offsetX;
    enemy->position.y = enemy->position.worldY - player->viewport->offsetY;
    /*Updating our hitbox based on the position of the word*/
    enemy->hitbox->x = enemy->position.worldX;
    enemy->hitbox->y = enemy->position.worldY;

    al_draw_filled_rectangle(enemy->position.x - size, enemy->position.y - size,
        enemy->position.x + size, enemy->position.y + size, al_map_rgb(0, 255, 0));

    CheckCollision(enemy, player);
    CheckDistance(enemy, player);
    EnemyBulletUpdate(enemy, player);
}
