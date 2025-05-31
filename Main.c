#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "Player.h"

#define GRAVITY 0.8
void InputControl(ALLEGRO_EVENT event, struct Player* player)
{
    if(event.keyboard.keycode == ALLEGRO_KEY_A)
        JoystickLeft(player->control);

    else if(event.keyboard.keycode == ALLEGRO_KEY_D)
        JoystickRight(player->control);

    else if(event.keyboard.keycode == ALLEGRO_KEY_W)
        JoystickUp(player->control);

    else if(event.keyboard.keycode == ALLEGRO_KEY_S)
        JoystickDown(player->control);

    else if(event.keyboard.keycode == ALLEGRO_KEY_SPACE)
        JoystickJump(player->control);

    else if(event.keyboard.keycode == ALLEGRO_KEY_ENTER)
        JoystickFire(player->control);
}

int main()
{
    al_init();
    al_install_keyboard();
    al_set_new_display_flags(ALLEGRO_FULLSCREEN | ALLEGRO_OPENGL);
    
    ALLEGRO_MONITOR_INFO info;
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_FONT* font = al_create_builtin_font();

    int w, h, cameraX, cameraY;

    //Finding information about the screen
    al_get_monitor_info(0, &info);
    w = info.x2 - info.x1;
    h = info.y2 - info.y1;
    //Creating a screen with the correct display size
    ALLEGRO_DISPLAY* display = al_create_display(w, h);

    //Any keyboard, screen or timer events will be inserted in our queue
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    //Creating players
    struct Player* player = PlayerCreate(20, RIGHT, 10, w/2, w, h);
    if(player == NULL)
        return 1;

    cameraX = player->x - w/2;
    cameraY = player->y - h/2;

    ALLEGRO_EVENT event;
    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);
        
        //Clock event
        if(event.type == ALLEGRO_EVENT_TIMER)
        {
            PlayerUpdate(player);
            int size = player->side/2;

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_filled_rectangle(player->x - cameraX, player->y - cameraY, player->x + cameraX, player->y + cameraY, al_map_rgb(255, 0, 0));
            al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 10, ALLEGRO_ALIGN_LEFT, "STATE: %d", player->state);
            for(struct Bullet* index = player->pistol->shots; index != NULL; index = index->next)
                al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(255, 0, 0)); 


            al_flip_display();
        }
        else if(event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP)
            InputControl(event, player);
        //Close the program
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
    }

    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    PlayerDestroy(player);
    return 0;
}
