#include <stdlib.h>
#include "Item.h"
#include "Player.h"


struct Item* ItemCreate(unsigned char side, struct Position position, struct Item* next, unsigned char type)
{
    struct Item* item = malloc(sizeof(struct Item));

    if(item == NULL)
        return NULL;
    
    item->type = type;
    item->hasCollide = FALSE;
    item->side = side;
    item->position = position;
    item->hitbox = HitboxCreate(side, side, position.x, position.y);
    item->next = next;

    return item;
}
void CheckCollisionA(struct Item* item, struct Player* player)
{
    if(HitboxCheck(player->hitbox, item->hitbox))
    {
        if(item->type == HEALTH)
            player->health++;

        if(item->type == 1)
            player->canDoubleJump++;

        
        item->hasCollide = TRUE;
    }
}
void ItemUpdate(struct Item* item, struct Player* player)
{
    int size = item->side/2;

    item->position.x = item->position.worldX - player->viewport->offsetX;
    item->position.y = item->position.worldY - player->viewport->offsetY;
    /*Updating our hitbox based on the position of the word*/
    item->hitbox->x = item->position.worldX;
    item->hitbox->y = item->position.worldY;

    HitboxDraw(item->hitbox, player);
    al_draw_filled_rectangle(item->position.x - size, item->position.y - size,
        item->position.x + size, item->position.y + size, al_map_rgb(0, 255, 0));

    CheckCollisionA(item, player);
}
