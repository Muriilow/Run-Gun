#ifndef __HITBOX__
#define __HITBOX__

#define X_SCREEN 320 
#define Y_SCREEN 320
#define LEFT 0 
#define RIGHT 1

struct Hitbox 
{
    unsigned short vert; 
    unsigned short hor; 
    float x;
    float y;
};

struct Hitbox* HitboxCreate(unsigned short vert, unsigned short hor, float x, float y);
unsigned char HitboxCheck(struct Hitbox* box1, struct Hitbox* box2);
void HitboxDestroy(struct Hitbox* box);
#endif
