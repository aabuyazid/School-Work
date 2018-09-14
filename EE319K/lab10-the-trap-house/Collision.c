// Collision.c
// Runs on LM4F120/TM4C123
// Michael Hernendez and Arkan Abuyazid
// This is a starter project for the EE319K Lab 10

// Last Modified: 5/3/2017
//*******************************************
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Collision.h"
#define x 0
#define y 1
extern Zombie Zombies[15];
extern Player Players[2];
// ************** Collision_Check *********************
// Checks whether 2 objects collide
// Input: x, y, widths, and heights of both objects
// Output: If they collide, return 1. If not, return 0;
/*
int8_t Collision_Check(int16_t x1, int16_t y1, uint8_t w1, uint8_t h1, int16_t x2, int16_t y2, uint8_t w2, uint8_t h2){
	
	int16_t topright1 [2]; int16_t topright2 [2]; int16_t topleft1 [2]; int16_t topleft2 [2]; int16_t bottomright1 [2]; int16_t bottomright2 [2]; int16_t bottomleft1 [2]; int16_t bottomleft2 [2]; 
	topleft1 [x] = x1; topleft1 [y] = (y1 - h1 ); topleft2 [x] = x2; topleft2 [y] = (y2 - h2 + 1); 																					//0 = x; 1 = y
	topright1 [x] = (x1 + w1 ); topright1 [y] = (y1 - h1 ); topright2 [x] = (x2 + w2 ); topright2 [y] = (y2 - h2 );
	bottomleft1 [x] = x1; bottomleft1 [y] = y1; bottomleft2 [x] = x2; bottomleft2 [y] = y2;
	bottomright1 [x] = (x1 + w1 ); bottomright1 [y] = y1; bottomright2 [x] = (x2 + w2 ); bottomright2 [y] = y2;
	
	if(topleft1[x] >= bottomleft2[x] && topleft1[x] <= bottomright2 [x] && topleft1[y] <= bottomleft2[y] && topleft1[y] >= topleft2[y]){
		return 1;
	}
	else if(topright1[x] >= bottomleft2[x] && topright1[x] <= bottomright2[x] && topright1[y] <= bottomleft2[y] && topright1[y] >= topleft2[y]){
		return 1;
	}
	else if(bottomleft1[x] <= topright2[x] && bottomleft1[x] >= topleft2[x] && bottomleft1[y] >= topright2[y] && bottomleft1[y] <= bottomright2[y]){
		return 1;
	}
	else if(bottomright1[x] >= topleft2[x] && bottomright1[x] <= topright2[x] && bottomright1[y] >= topleft2[y] && bottomright1[y] <= bottomleft2[y]){
		return 1;
	}
	else{
		return 0;
	}
}
*/

int8_t Collision_Check(int16_t x1, int16_t y1, uint8_t w1, uint8_t h1, int16_t x2, int16_t y2, uint8_t w2, uint8_t h2){
	uint8_t check1, check2;
	if((x1 + w1 >= x2) && (x2 + w2 >= x1)){
		check1 = 1;
	}
	else{
		check1 = 0;
	}
	if((y2 >= y1 - h1) && (y1 >= y2 - h2)){
		check2 = 1;
	}
	else{
		check2 = 0;
	}
	return (check1 & check2);
}


// ************** In_Range *********************
// Checks if a zombie's attack hit a player
// Input: Pointer to desired zombie
// Output: If it hits a player, return 1. If not, return 0;

int8_t In_Range(Zombie *ptzombie){
	
	int8_t x1player1 = Players[0].xcoor; 					int8_t x2player1 = Players[0].width + x1player1 - 1;
	int8_t y2player1 = Players[0].ycoor; 					int8_t y1player1 = y2player1 - Players[0].height + 1;

	int8_t x1 = ptzombie->xcoor;											int8_t x2 = x1 + (ptzombie->width) - 1;
	int8_t y2 = ptzombie->ycoor;											int8_t y1 = y2 - (ptzombie->height) + 1;
	int8_t xavg = (x1 + x2) / 2;												int8_t yavg = (y1 + y2) / 2;
	uint8_t direction = ptzombie->direction;	
	uint8_t range = ptzombie->range;
	
	if(direction == up){
		if(xavg >= x1player1 && xavg <= x2player1 && (y1 - range) <= y2player1 && y1 >= y2player1){
			return 1;
		}

	}
	if(direction == down){
		if(xavg >= x1player1 && xavg <= x2player1 && (y2 + range) >= y1player1 && y2 <= y1player1){
			return 1;
		}
	}
	if(direction == left){
		if(yavg <= y1player1 && yavg >= y2player1 && (x1 - range) <= x1player1 && x1 >= x1player1){
			return 1;
		}
	}
	if(direction == right){
		if(yavg <= y1player1 && yavg >= y2player1 && (x2 + range) >= x2player1 && x2 <= x2player1){
			return 1;
		}

	}
	return 0;
}

// ************** Within_Range *********************
// Checks if a projectile has traveled farther than its maximum range
// Input: Pointer to desired projectile
// Output: If it's within range, return 1. If not, return 0;

int8_t Within_Range(Projectile *ptprojectile){
	int8_t deltax = abs((ptprojectile->xcoor) - (ptprojectile->initialx));
	int8_t deltay = abs((ptprojectile->ycoor) - (ptprojectile->initialy));
	uint8_t range1 = ptprojectile->ptplayer->ptweapon->range;
	uint8_t range2 = ptprojectile->ptzombie->range;
	
	if(ptprojectile->direction == up || ptprojectile->direction == down){
		if(ptprojectile->shooter == player){
			if(deltay <= range1){
				return 1;
			}
			else{
				return 0;
			}
		}
		if(ptprojectile ->shooter == zombie){
			if(deltay <= range2){
				return 1;
			}
			else{
				return 0;
			}
		}
	}
	if(ptprojectile->direction == left || ptprojectile->direction == right){
		if(ptprojectile->shooter == player){
			if(deltax <= range1){
				return 1;
			}
			else{
				return 0;
			}
		}
		if(ptprojectile->shooter == zombie){
			if(deltax <= range2){
				return 1;
			}
			else{
				return 0;
			}
		}
	}
	return 0;
}

// ************** abs *********************
// Returns the absolute value of input
// Input: signed integer
// Output: absolute value of input

int8_t abs(int8_t v){
  return v * ( (v<0) * (-1) + (v>0));
}

int8_t Projectile_Collision_Check(Projectile *ptprojectile, int8_t x2, int8_t y2, int8_t w2, int8_t h2){
	
	int8_t topright2 [2]; int8_t topleft2 [2]; int8_t bottomright2 [2]; int8_t bottomleft2 [2];
	int8_t xcoor = ptprojectile->xcoor;
	int8_t ycoor = ptprojectile->ycoor;
	int8_t prevx = ptprojectile->prevx;
	int8_t prevy = ptprojectile->prevy;
	topleft2 [x] = x2; topleft2 [y] = (y2 - h2 + 1); 																					//0 = x; 1 = y
	topright2 [x] = (x2 + w2 - 1); topright2 [y] = (y2 - h2 + 1);
	bottomleft2 [x] = x2; bottomleft2 [y] = y2;
	bottomright2 [x] = (x2 + w2 - 1); bottomright2 [y] = y2;
	
	if(xcoor >= topleft2[x] && prevx <= topleft2[x] && ycoor >= topleft2[y] && prevy <= topleft2[y]){
		return 1;
	}
	else if(xcoor <= topright2[x] && prevx >= topright2[x] && ycoor >= topright2[y] && prevy <= topright2[y]){
			return 1;
	}
	else if(xcoor <= bottomright2[x] && prevx >= bottomright2[x] && ycoor <= bottomright2[y] && prevy >= bottomright2[y]){
		return 1;
	}
	else if(xcoor >= bottomleft2[x] && prevx <= bottomleft2[x] && ycoor <= bottomright2[y] && prevy >= bottomright2[y]){
		return 1;
	}
	else if(xcoor == prevx && xcoor >= topleft2[x] && xcoor <= topright2[x] && ycoor >= topleft2[y] && prevy <= topleft2[y]){
		return 1;
	}
	else if(xcoor == prevx && xcoor >= bottomleft2[x] && xcoor <= bottomright2[x] && ycoor <= bottomleft2[y] && prevy >= bottomleft2[y]){
		return 1;
	}
	else if(ycoor == prevy && ycoor <= bottomleft2[y] && ycoor >= topleft2[y] && xcoor >= topleft2[x] && prevx <= topleft2[x]){
		return 1;
	}
	else if(ycoor == prevy && ycoor <= bottomright2[y] && ycoor >= topright2[y] && xcoor <= topright2[x] && prevx >= topright2[x]){
		return 1;
	}
	else{
			return 0;
	}
}
