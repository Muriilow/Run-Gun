#include <stdlib.h>
#include "Item.h"
#include "Player.h"


struct Item* ItemCreate(struct Position position, struct Item* next, unsigned char type, ALLEGRO_BITMAP* sprite)
{
    struct Item* item = malloc(sizeof(struct Item));

    if(item == NULL)
        return NULL;
    
    item->type = type;
    item->hasCollide = FALSE;
    item->position = position;
    item->hitbox = HitboxCreate(20, 20, position.x, position.y);
    item->next = next;
    item->sprite = sprite;

    return item;
}
void CheckCollisionItem(struct Item* item, struct Player* player)
{
    if(HitboxCheck(player->hitbox, item->hitbox))
    {
        if(item->type == HEALTH)
            player->health++;

        if(item->type == DOUBLE_JUMP)
            player->canDoubleJump++;

        
        item->hasCollide = TRUE;
    }
}
void ItemUpdate(struct Item* item, struct Player* player)
{
    item->position.x = item->position.worldX - player->viewport->offsetX;
    item->position.y = item->position.worldY - player->viewport->offsetY;
    /*Updating our hitbox based on the position of the word*/
    item->hitbox->x = item->position.worldX;
    item->hitbox->y = item->position.worldY;

    //HitboxDraw(item->hitbox, player);
    al_draw_bitmap(item->sprite, item->position.x - 25, item->position.y - 25, 0);

    CheckCollisionItem(item, player);
}
