#include <stdlib.h>
#include "Joystick.h"

struct Joystick* JoystickCreate()
{
    struct Joystick* element = malloc(sizeof(struct Joystick));
    element->right = 0;
    element->left = 0;
    element->up = 0;
    element->down = 0;
    element->fire = 0;
    element->jump = 0;
    return element;
}

struct Joystick* JoystickDestroy(struct Joystick* joystick)
{
    free(joystick);
}

struct Joystick* JoystickLeft(struct Joystick* joystick)
{
    joystick->left = joystick->left ^ 1;
}

struct Joystick* JoystickRight(struct Joystick* joystick)
{
    joystick->right = joystick->right ^ 1;
}

struct Joystick* JoystickDown(struct Joystick* joystick) 
{
    joystick->down = joystick->down ^ 1;
}

struct Joystick* JoystickUp(struct Joystick* joystick) 
{
    joystick->up = joystick->up ^ 1;
}
struct Joystick* JoystickFire(struct Joystick* joystick) 
{
    joystick->fire = joystick->fire ^ 1;
}
struct Joystick* JoystickJump(struct Joystick* joystick) 
{
    joystick->jump = joystick->jump ^ 1;
}
