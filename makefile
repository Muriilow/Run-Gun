SRCS = Main.o Player.o Joystick.o Pistol.o Bullet.o Hitbox.o
LDFLAGS = $(shell pkg-config --libs allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5) 
CFLAGS = $(shell pkg-config --cflags allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5)

main: $(SRCS)
	gcc -Wall -Wextra $(SRCS) -o game $(LDFLAGS) 

Main.o: Main.c Player.h 
	gcc -Wall -Wextra $(CFLAGS) -c Main.c

Player.o: Player.c Player.h
	gcc -Wall -Wextra -c Player.c

Joystick.o: Joystick.c Joystick.h 
	gcc -Wall -Wextra -c Joystick.c

Hitbox.o: Hitbox.c Hitbox.h
	gcc -Wall -Wextra -c Hitbox.c

Pistol.o: Pistol.c Pistol.h
	gcc -Wall -Wextra -c Pistol.c

Bullet.o: Bullet.c Bullet.h
	gcc -Wall -Wextra -c Bullet.c

clean:
	rm -f *.o game
