SRC_FILES = Main.o Player.o Joystick.o Pistol.o Bullet.o Hitbox.o NormalEnemy.o Ground.o Item.o GameManager.o

SRC_MAIN = $(addprefix Assets/Scripts/, Main.c Player.h NormalEnemy.h Item.h Ground.h GameManager.h)
SRC_PLAYER = $(addprefix Assets/Scripts/, Player.c Player.h Utils.h)
SRC_ENEMY = $(addprefix Assets/Scripts/, NormalEnemy.c NormalEnemy.h Hitbox.h Utils.h)
SRC_JOYSTICK = $(addprefix Assets/Scripts/, Joystick.c Joystick.h)
SRC_HITBOX = $(addprefix Assets/Scripts/, Hitbox.c Hitbox.h)
SRC_PISTOL = $(addprefix Assets/Scripts/, Pistol.c Pistol.h Utils.h)
SRC_BULLET = $(addprefix Assets/Scripts/, Bullet.c Bullet.h Hitbox.h Utils.h)
SRC_GROUND = $(addprefix Assets/Scripts/, Ground.c Ground.h Hitbox.h Utils.h Player.h)
SRC_ITEM = $(addprefix Assets/Scripts/, Item.c Item.h Hitbox.h Utils.h)
SRC_MANAGER = $(addprefix Assets/Scripts/, GameManager.c GameManager.h Player.h NormalEnemy.h Item.h)

LDFLAGS = $(shell pkg-config --libs allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5) 
CFLAGS = $(shell pkg-config --cflags allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5)
CPRIM = $(shell pkg-config --cflags allegro-5 allegro_primitives-5)

main: $(SRC_FILES)
	gcc -Wall -Wextra $(SRC_FILES) -o game $(LDFLAGS) -lm 

Main.o: $(SRC_MAIN)
	gcc -Wall -Wextra $(CFLAGS) -c Assets/Scripts/Main.c

Player.o: $(SRC_PLAYER)
	gcc -Wall -Wextra $(CPRIM) -c Assets/Scripts/Player.c

NormalEnemy.o: $(SRC_ENEMY)
	gcc -Wall -Wextra $(CPRIM) -c Assets/Scripts/NormalEnemy.c

Joystick.o: $(SRC_JOYSTICK) 
	gcc -Wall -Wextra -c Assets/Scripts/Joystick.c

Hitbox.o: $(SRC_HITBOX)
	gcc -Wall -Wextra -c Assets/Scripts/Hitbox.c

Pistol.o: $(SRC_PISTOL)
	gcc -Wall -Wextra -c Assets/Scripts/Pistol.c

Bullet.o: $(SRC_BULLET)
	gcc -Wall -Wextra $(CPRIM) -c Assets/Scripts/Bullet.c

Ground.o: $(SRC_GROUND)
	gcc -Wall -Wextra $(CPRIM) -c Assets/Scripts/Ground.c

Item.o: $(SRC_ITEM)
	gcc -Wall -Wextra $(CPRIM) -c Assets/Scripts/Item.c

GameManager.o: $(SRC_MANAGER)
	gcc -Wall -Wextra -c Assets/Scripts/GameManager.c

clean:
	rm -f *.o game
