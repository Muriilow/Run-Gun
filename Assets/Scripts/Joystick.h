#ifndef __JOYSTICK__
#define __JOYSTICK__

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define FIRE 4
#define JUMP 5

struct Joystick
{
    unsigned char right;
    unsigned char left;
    unsigned char up;
    unsigned char down;
    unsigned char fire;
    unsigned char jump;
};

struct Joystick* JoystickCreate();  
void JoystickDestroy(struct Joystick* joystick);
void JoystickActivate(struct Joystick* joystick, int button);

#endif
