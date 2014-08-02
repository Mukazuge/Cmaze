 #include "player.h"

 void animateTo(Player *player, int nuevoX, int nuevoY, int miliSec){
	 player->x += nuevoX;
	 player->y += nuevoY;
}