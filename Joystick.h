#ifndef __JOYSTICK__
#define __JOYSTICK__

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
struct Joystick* JoystickDestroy(struct Joystick* joystick);

struct Joystick* JoystickLeft(struct Joystick* joystick);  
struct Joystick* JoystickRight(struct Joystick* joystick);  
struct Joystick* JoystickDown(struct Joystick* joystick);  
struct Joystick* JoystickUp(struct Joystick* joystick);  
struct Joystick* JoystickFire(struct Joystick* joystick);  
struct Joystick* JoystickJump(struct Joystick* joystick);  

#endif
