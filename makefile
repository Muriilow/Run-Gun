SRCS = Main.o Player.o Joystick.o Pistol.o Bullet.o Hitbox.o NormalEnemy.o
LDFLAGS = $(shell pkg-config --libs allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5) 
CFLAGS = $(shell pkg-config --cflags allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5)
CPRIM = $(shell pkg-config --cflags allegro-5 allegro_primitives-5)

main: $(SRCS)
	gcc -Wall -Wextra $(SRCS) -o game $(LDFLAGS) -lm 

Main.o: Main.c Player.h Player.h NormalEnemy.h
	gcc -Wall -Wextra $(CFLAGS) -c Main.c

Player.o: Player.c Player.h Utils.h
	gcc -Wall -Wextra $(CPRIM) -c Player.c

NormalEnemy.o: NormalEnemy.c NormalEnemy.h Player.h Hitbox.h Utils.h
	gcc -Wall -Wextra $(CPRIM) -c NormalEnemy.c

Joystick.o: Joystick.c Joystick.h 
	gcc -Wall -Wextra -c Joystick.c

Hitbox.o: Hitbox.c Hitbox.h
	gcc -Wall -Wextra -c Hitbox.c

Pistol.o: Pistol.c Pistol.h Utils.h
	gcc -Wall -Wextra -c Pistol.c

Bullet.o: Bullet.c Bullet.h Hitbox.h Utils.h
	gcc -Wall -Wextra $(CPRIM) -c Bullet.c

clean:
	rm -f *.o game
