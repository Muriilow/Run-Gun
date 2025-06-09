#include <stdlib.h>
#include "Hitbox.h"

struct Hitbox* HitboxCreate(unsigned short vert, unsigned short hor, float x, float y)
{
   struct Hitbox* box = malloc(sizeof(struct Hitbox));

   box->vert = vert;
   box->hor = hor;
   box->x = x;
   box->y = y;

   return box;
}

unsigned char HitboxCheck(struct Hitbox* box1, struct Hitbox* box2)
{
    int top1 = box1->y + box1->vert/2;
    int top2 = box2->y + box2->vert/2;
    
    int bottom1 = box1->y - box1->vert/2;
    int bottom2 = box2->y - box2->vert/2;

    int left1 = box1->x - box1->hor/2;
    int left2 = box2->x - box2->hor/2;
    
    int right1 = box1->x + box1->hor/2;
    int right2 = box2->x + box2->hor/2;

    int box1Top = (top1 >= bottom2) && (bottom2 >= bottom1);
    int box2Top = (top2 >= bottom1) && (bottom1 >= bottom2);

    int box1Right = (right1 >= left2) && (left2 >= left1);
    int box2Right = (right2 >= left1) && (left1 >= left2);

    if((box1Top || box2Top) && (box1Right || box2Right))
        return 1;
    
    return 0;
}

void HitboxDestroy(struct Hitbox* box)
{
    free(box);
}
