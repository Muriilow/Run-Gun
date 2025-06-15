#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "Ground.h"
#include "Player.h"

struct Ground* GroundCreate(unsigned char side, struct Position position)
{
    struct Ground* ground = malloc(sizeof(struct Ground));

    if(ground == NULL)
        exit(EXIT_FAILURE);

    ground->side = side;
    ground->position = position;
    ground->hitbox = HitboxCreate(side, side, position.x, position.y);

    return ground;
}

void GroundUpdate(struct Ground* ground, struct Player* player)
{
    int size = ground->side/2;
    ground->position.x = ground->position.worldX - player->viewport->offsetX;
    ground->position.y = ground->position.worldY - player->viewport->offsetY;
    /*Updating our hitbox based on the position of the word*/
    ground->hitbox->x = ground->position.worldX;
    ground->hitbox->y = ground->position.worldY;

    al_draw_filled_rectangle(ground->position.x - size, ground->position.y - size,
        ground->position.x + size, ground->position.y + size, al_map_rgb(0, 255, 0));
}
