main: 
	gcc Main.c Player.c Joystick.c Pistol.c Bullet.c Hitbox.c -o game $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs --cflags)
