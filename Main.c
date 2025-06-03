#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "Player.h"

void InputControl(ALLEGRO_EVENT event, struct Player* player)
{
    switch(event.keyboard.keycode)
    {
        case ALLEGRO_KEY_A:
            JoystickActivate(player->control, LEFT);
            break;
        case ALLEGRO_KEY_D:
            JoystickActivate(player->control, RIGHT);
            break;
        case ALLEGRO_KEY_W:
            JoystickActivate(player->control, UP);
            break;
        case ALLEGRO_KEY_S:
            JoystickActivate(player->control, DOWN);
            break;
        case ALLEGRO_KEY_SPACE:
            JoystickActivate(player->control, JUMP);
            break;
        case ALLEGRO_KEY_ENTER:
            JoystickActivate(player->control, FIRE);
            break;
    }
}

int main()
{
    al_init();
    al_init_image_addon();
    al_install_keyboard();
    al_set_new_display_flags(ALLEGRO_FULLSCREEN | ALLEGRO_OPENGL);
    
    //Initializing variables 
    ALLEGRO_MONITOR_INFO info;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* queue; 
    ALLEGRO_FONT* font; 
    ALLEGRO_EVENT event;
    struct Player* player;
    int screenW, screenH, imgW, imgH, newW, posX, bgOffset;
    float scale;


    timer = al_create_timer(1.0 / 60.0);
    queue = al_create_event_queue();
    font = al_create_builtin_font();

    //Finding information about the screen
    al_get_monitor_info(0, &info);
    screenW = info.x2 - info.x1;
    screenH = info.y2 - info.y1;

    //Creating a screen with the correct display size
    ALLEGRO_DISPLAY* display = al_create_display(screenW, screenH);
    ALLEGRO_BITMAP *background = al_load_bitmap("Assets/background.jpg");
    imgW = al_get_bitmap_width(background);
    imgH = al_get_bitmap_height(background);

    scale = (float)screenW / (float)imgH;
    newW = imgW * scale;
    posX = (screenW - newW) / 2;

    //Any keyboard, screen or timer events will be inserted in our queue
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    //Creating players
    player = PlayerCreate(20, RIGHT, 201, screenW/2, screenW, screenH);

    al_start_timer(timer);

    while(1)
    {
        al_wait_for_event(queue, &event);
        
        //Clock event
        if(event.type == ALLEGRO_EVENT_TIMER)
        {
            PlayerUpdate(player);
            int size = player->side/2;


            if(player->isRight)
                bgOffset -= 2;
            if(player->isLeft)
                bgOffset += 2;

            al_draw_bitmap(background, bgOffset, 0, 0);
            al_draw_filled_rectangle(player->x - size, player->y - size,
                player->x + size, player->y + size, al_map_rgb(255, 0, 0));

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

    al_destroy_bitmap(background);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    PlayerDestroy(player);
    return 0;
}
