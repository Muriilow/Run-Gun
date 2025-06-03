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

void JoystickActivate(struct Joystick* joystick, int button)
{
    switch(button)
    {
        case UP:
            joystick->up = joystick->up ^ 1;
            break;
        case LEFT:
            joystick->left = joystick->left ^ 1;
            break;
        case RIGHT:
            joystick->right = joystick->right ^ 1;
            break;
        case DOWN:
            joystick->down = joystick->down ^ 1;
            break;
        case FIRE:
            joystick->fire = joystick->fire ^ 1;
            break;
        case JUMP:
            joystick->jump = joystick->jump ^ 1;
            break;
    }
}
void JoystickDestroy(struct Joystick* joystick)
{
    free(joystick);
}
