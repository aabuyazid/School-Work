// Interaction.c
// Runs on LM4F120/TM4C123
// Michael Hernendez and Arkan Abuyazid
// This is a starter project for the EE319K Lab 10

// Last Modified: 5/3/2017
//*******************************************

#include "Interaction.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"
extern uint8_t ZombieTimer1Status, ZombieTimer2Status, ZombieCounter;
extern uint8_t ZombieIndex, ProjectileIndex;
extern Zombie Zombies[15];
extern Projectile Projectiles[20];
extern Player Players[2];
extern Obstacle Obstacles[10];
extern uint8_t DEATH;
	// ************** MoveX_Player *********************
// Moves player in the x-directions only
// Input: Pointer to desired player
// Output: none
// After moving, turned flag will be set to 0

void MoveX_Player (Player *ptplayer){
	int16_t x = ptplayer->xcoor;
	uint8_t w = ptplayer->width;
	uint8_t direction = ptplayer->direction;
	ptplayer->prevx = x;
	if(ptplayer->turned == 1){
		ptplayer->turned = 0;
		return;
	}
	if(direction == left){
		if(x == 0){
			return;
		}
		else{
		x--;
		}
	}
	else if(direction == right){
		if(x == (128 - w + 1)){
			return;
		}
		else{
			x++;
		}
	}
	else{
		return;
	}
	ptplayer->xcoor = x;
	return;
}

// ************** MoveY_Player *********************
// Moves player in the y-directions only
// Input: Pointer to desired player
// Output: none
// After moving, turned flag will be set to 0

void MoveY_Player (Player *ptplayer){
	int16_t y = ptplayer->ycoor;
	uint8_t h = ptplayer->height;
	uint8_t direction = ptplayer->direction;
	ptplayer->prevy = y;
	if(ptplayer->turned == 1){
		ptplayer->turned = 0;
		return;
	}
	if(direction == up){
		if((y - h + 1) == 0 ){
			return;
		}
		else{
			y--;
		}
	}
	else if(direction == down){
		if(y == (160)){
			return;
		}
		else{
			y++;
		}
	}
	else{
		return;
	}
	ptplayer->ycoor = y;
	return;
}

// ************** Turn_Player *********************
// Turns the player to the desired direction
// Input: Pointer to desired player and the new direction
// Output: none
// After turning, turned flag will be set to 1

void Turn_Player(Player *ptplayer, uint8_t newdirection){
	if(newdirection == stay){
		return;
	}
	if(ptplayer->turned == 0 && ptplayer->direction != newdirection){
		ptplayer->direction = newdirection;
		ptplayer->turned = 1;
	}
	return;
}

// ************** MoveX_Zombie *********************
// Moves zombie in the x-directions only
// Input: Pointer to desired zombie
// Output: none
// After moving, turned flag will be set to 0
// If attacking flag is not 0, then the zombie will not move

void MoveX_Zombie (Zombie *ptzombie){
	int16_t x = ptzombie->xcoor;
	uint8_t w = ptzombie->width;
	uint8_t direction = ptzombie->direction;
	uint8_t movespeed = ptzombie->movespeed;
	ptzombie->prevx = x;
	if(ptzombie->attacking != 0){
		return;
	}
	if(ptzombie->turned == 1){
		ptzombie->turned = 0;
		return;
	}
	if(direction == left){
		if(x == 0){
			return;
		}
		else{
		x -= movespeed;
		}
	}
	else if(direction == right){
		if(x == (128 - w + 1)){
			return;
		}
		else{
			x += movespeed;
		}
	}
	else{
		return;
	}
	ptzombie->xcoor = x;
	return;
}

// ************** MoveY_Zombie *********************
// Moves zombie in the y-directions only
// Input: Pointer to desired zombie
// Output: none
// After moving, turned flag will be set to 0
// If attacking flag is not 0, then the zombie will not move

void MoveY_Zombie (Zombie *ptzombie){
	int16_t y = ptzombie->ycoor;
	uint8_t h = ptzombie->height;
	uint8_t direction = ptzombie->direction;
	uint8_t movespeed = ptzombie->movespeed;
	ptzombie->prevy = y;
	if(ptzombie->attacking != 0){
		return;
	}
	if(ptzombie->turned == 1){
		ptzombie->turned = 0;
		return;
	}
	if(direction == up){
		if((y - h + 1) == 0 ){
			return;
		}
		else{
			y -= movespeed;
		}
	}
	else if(direction == down){
		if(y == (160)){
			return;
		}
		else{
			y += movespeed;
		}
	}
	else{
		return;
	}
	ptzombie->ycoor = y;
	return;
}

// ************** Turn_Zombie *********************
// Turns the player to the desired direction
// Input: Pointer to desired zombie and the new direction
// Output: none
// After turning, turned flag will be set to 1
// If attacking flags is not 0, then the zombie will not turn

void Turn_Zombie(Zombie *ptzombie, uint8_t newdirection){
	if(newdirection == stay){
		return;
	}
	if(ptzombie->attacking != 0){
		return;
	}
	if(ptzombie->turned == 0 && ptzombie->direction != newdirection){
		ptzombie->direction = newdirection;
		ptzombie->turned = 1;
	}
	return;
}

void MoveY_Projectile(Projectile *ptprojectile){
	int16_t y = ptprojectile->ycoor;
	uint8_t direction = ptprojectile->direction;
	uint8_t shooter = ptprojectile->shooter;
	uint8_t shotspeed;
	
	if(shooter == player){
		shotspeed = ptprojectile->ptplayer->ptweapon->shotspeed;
	}
	else{
		shotspeed = 5;
	}
	ptprojectile->prevy = y;
	if(direction == up){
		y -= shotspeed;
	}
	else if(direction == down){
		y += shotspeed;
	}
	ptprojectile->ycoor = y;
	return;
}

void MoveX_Projectile(Projectile *ptprojectile){
	
	int16_t x = ptprojectile->xcoor;
	uint8_t direction = ptprojectile->direction;
	uint8_t shooter = ptprojectile->shooter;
	uint8_t shotspeed;
	ptprojectile->prevx = x;
	
	if(shooter == player){
		shotspeed = ptprojectile->ptplayer->ptweapon->shotspeed;
	}
	else{
		shotspeed = 5;
	}
	ptprojectile->prevx = x;
	if(direction == left){
		x -= shotspeed;
	}
	else if(direction == right){
		x += shotspeed;
	}
	ptprojectile->xcoor = x;
	return;
}

// **************** Attack_Flag *********************
// Sets zombie attacking flag to 1 - 3 depending on which timer is ready
// Input: Pointer to desired zombie
// Output: None
// This flag basically tells the timer to wait before the zombie actually attacks
// This is for both melee and shooting zombies

void Attack_Flag(Zombie *ptzombie){
	
	if(ZombieTimer1Status == 1){
		ptzombie->attacking = 1;
	}
	if(ZombieTimer2Status == 1){
		ptzombie->attacking = 2;
	}
	return;
	
}
// **************** ReloadFlag_Off *********************
// Sets player reloading flag to 1
// Input: Pointer to desired zombie
// Output: None
// This flag tells the timer to wait before the player can shoot again

void ReloadFlag_Off(Player *ptplayer){
	ptplayer->reloading = 0;
}

// **************** Attack *********************
// Makes zombie attack. If a player is in range, he/she will lose hp
// If zombie is ranged, it will shoot out projectile
// Input: Pointer to desired zombie, player 1, and player 2
// Output: None
// Resets attacking flags

void Attack (Zombie *ptzombie){
	int8_t result;
	Player *ptplayer1 = &Players[0];
	Player *ptplayer2 = &Players[1];
	if(ptzombie->attacking == 0){
		return;
	}
	result = In_Range(ptzombie);
	if(ptzombie->type != sniping){
		if(result == 1){
			ptplayer1->hp -= ptzombie->damage;
		}
		else if(result == 2){
			ptplayer2->hp -= ptzombie->damage;
		}
	}
	else{
		Shoot_Zombie(&Projectiles[ProjectileIndex], ptzombie);
	}
	ptzombie->attacking = 0;
	return;
}

// **************** Shoot_Player *********************
// Makes player shoot
// Essentially spawns projectile on player
// Input: Pointer to projectile and desired player
// Output: None
// Sets reloading flag
// Increments ProjectileIndex

void Shoot_Player(Projectile *ptprojectile, Player *ptplayer){
	
	Weapon *ptweapon;
	int16_t x1 = ptplayer->xcoor;
	int16_t y1 = ptplayer->ycoor;
	ptweapon = ptplayer->ptweapon;
	ptprojectile->direction = ptplayer->direction;
	ptprojectile->initialx = ptprojectile->prevx = ptprojectile->xcoor = x1;
	ptprojectile->initialy = ptprojectile->prevy = ptprojectile->ycoor = y1;
	ptprojectile->ptplayer = ptplayer;
	ptprojectile->shooter = player;
	ptprojectile->status = onfield;
	ptplayer->reloading = 1;
	ProjectileIndex = (ProjectileIndex + 1) % 20;
	return;
	
}

// **************** Shoot_Zombie *********************
// Makes zombie shoot
// Essentially spawns projectile on zombie
// Input: Pointer to projectile and desired zombie
// Output: None
// Sets reloading flag
// Increments ProjectileIndex

void Shoot_Zombie(Projectile *ptprojectile, Zombie *ptzombie){
	
	int16_t x1 = ptzombie->xcoor;
	int16_t y1 = ptzombie->ycoor;
	ptprojectile->direction = ptzombie->direction;
	ptprojectile->initialx = ptprojectile->prevx = ptprojectile->xcoor = x1;
	ptprojectile->initialy = ptprojectile->prevy = ptprojectile->ycoor = y1;
	ptprojectile->ptzombie = ptzombie;
	ptprojectile->shooter = zombie;
	ptprojectile->status = onfield;
	ProjectileIndex = (ProjectileIndex + 1) % 20;
	return;
	
}

// **************** Projectile_Off *************************
// Despawns projectile
// Input: Pointer to desired projectile
// Output: None
// Sets status to offfield

void Projectile_Off(Projectile *ptprojectile){
	ptprojectile->status = offfield;
	ptprojectile->dead = 1;
	return;
}

// **************** Zombie_Off *************************
// Despawns zombie
// Input: Pointer to desired zombie
// Output: None
// Sets status to offfield

void Zombie_Off(Zombie *ptzombie){
	ptzombie->status = offfield;
	return;
}

// **************** Player_Off *************************
// Despawns player
// Input: Pointer to desired player
// Output: None
// Sets status to offfield

void Player_Off(Player *ptplayer){
	ptplayer->status = offfield;
	return;
}

// **************** Revert_Pos_Player *************************
// Puts player back to previous position
// Input: Pointer to desired player
// Output: None

void Revert_Pos_Player(Player *ptplayer){
	ptplayer->xcoor = ptplayer->prevx;
	ptplayer->ycoor = ptplayer->prevy;
}
	
// **************** Revert_Pos_Zombie *************************
// Puts zombie back to previous position
// Input: Pointer to desired zombie
	
void Revert_Pos_Zombie(Zombie *ptzombie){
	ptzombie->xcoor = ptzombie->prevx;
	ptzombie->ycoor = ptzombie->prevy;
}
// Output: None	
	
void Zombie_Collision(void){
	uint8_t index0, index1;
	int16_t x1, x2, y1, y2;
	uint8_t w1, w2, h1, h2;
	Player *ptplayer;	
	
	for(index0 = 0; index0 < 15; index0++){
		
		x1 = Zombies[index0].xcoor;
		y1 = Zombies[index0].ycoor;
		w1 = Zombies[index0].width;
		h1 = Zombies[index0].height;
		
		/*
		if(In_Range(&Zombies[index0]) != 0 && Zombies[index0].status == 1){
			Attack_Flag(&Zombies[index0]);
		}
		*/
		
		for(index1 = 0; index1 < 2; index1++){
			
			x2 = Players[index1].xcoor;
			y2 = Players[index1].ycoor;
			w2 = Players[index1].width;
			h2 = Players[index1].height;
			ptplayer = &Players[index1];
			
			if(Zombies[index0].status == 1 && Players[index1].status == 1){
				if(Collision_Check(x1, y1, w1, h1, x2, y2, w2, h2) == 1){
					Revert_Pos_Zombie(&Zombies[index0]);
					ptplayer->hp = 0;
					ptplayer->status = 0;
					ptplayer->dead = 1;
					DEATH=1;
				}
			}
		}
		for(index1 = 0; index1 < 15; index1++){
			if(index1 != index0){
				x2 = Zombies[index1].xcoor;
				y2 = Zombies[index1].ycoor;
				w2 = Zombies[index1].width;
				h2 = Zombies[index1].height;
				
				if(Zombies[index1].status == 1 && Zombies[index0].status == 1){
					if(Collision_Check(x1, y1, w1, h1, x2, y2, w2, h2) == 1){
						Revert_Pos_Zombie(&Zombies[index0]);
					}
				}
			}
		}
		
		for(index1 = 0; index1 < 10; index1++){
			
			x2 = Obstacles[index1].xcoor;
			y2 = Obstacles[index1].ycoor;
			w2 = Obstacles[index1].width;
			h2 = Obstacles[index1].height;
			
			if(Obstacles[index1].status == 1){
				if(Collision_Check(x1, y1, w1, h1, x2, y2, w2, h2) == 1){
					Revert_Pos_Zombie(&Zombies[index0]);
				}
			}
		}
	}		
}

void Projectile_Collision(void){
	uint8_t index0, index1;
	uint8_t status, shooter;
	int16_t x2, y2;
	uint8_t w2, h2;
	Zombie *ptzombie;
	Player *ptplayer;
	
	for(index0 = 0; index0 < 20; index0++){
		
		status = Projectiles[index0].status;
		shooter = Projectiles[index0].shooter;
		
		for(index1 = 0; index1 < 15; index1++){
			x2 = Zombies[index1].xcoor;
			y2 = Zombies[index1].ycoor;
			w2 = Zombies[index1].width;
			h2 = Zombies[index1].height;
			ptzombie = &Zombies[index1];
			
			if(Zombies[index1].status == onfield && status == onfield && shooter == player){
				if(Projectile_Collision_Check(&Projectiles[index0], x2, y2, w2, h2) == 1){
					ptzombie->hp = 0;
					ptzombie->status = 0;
					ptzombie->dead = 1;
					Projectile_Off(&Projectiles[index0]);																										///COME BACK
				}
			}
		}
		
		for(index1 = 0; index1 < 2; index1++){
			x2 = Players[index1].xcoor;
			y2 = Players[index1].ycoor;
			w2 = Players[index1].width;
			h2 = Players[index1].height;
			ptplayer = &Players[index1];
			
			if(Players[index1].status == onfield && status == onfield && shooter == zombie){
				if(Projectile_Collision_Check(&Projectiles[index0], x2, y2, w2, h2) == 1){
					ptplayer->hp = 0;
					ptplayer->status = 0;
					ptplayer->dead = 1;
					Projectile_Off(&Projectiles[index0]);
				}
			}
		}
		
		for(index1 = 0; index1 < 10; index1++){
			x2 = Obstacles[index1].xcoor;
			y2 = Obstacles[index1].ycoor;
			w2 = Obstacles[index1].width;
			h2 = Obstacles[index1].height;
			
			if(Obstacles[index1].status == onfield && status == onfield){
				if(Projectile_Collision_Check(&Projectiles[index0], x2, y2, w2, h2) == 1){
					Projectile_Off(&Projectiles[index0]);
				}
			}
		}
	}
}

void Player_Collision(void){
	uint8_t index0, index1;
	int16_t x1, x2, y1, y2;
	uint8_t w1, w2, h1, h2, status;
	
	for(index0 = 0; index0 < 2; index0++){
		
		x1 = Players[index0].xcoor;
		y1 = Players[index0].ycoor;
		w1 = Players[index0].width;
		h1 = Players[index0].height;
		status = Players[index0].status;
		x2 = Players[((index0 + 1) % 2)].xcoor;
		y2 = Players[((index0 + 1) % 2)].ycoor;
		w2 = Players[((index0 + 1) % 2)].width;
		h2 = Players[((index0 + 1) % 2)].height;
		
		if(Players[((index0 + 1) % 2)].status == 1 && status == 1){
			if(Collision_Check(x1, y1, w1, h1, x2, y2, w2, h2) == 1){
				Revert_Pos_Player(&Players[index0]);
			}
		}
		
		for(index1 = 0; index1 < 15; index1++){
			
			x2 = Zombies[index1].xcoor;
			y2 = Zombies[index1].ycoor;
			w2 = Zombies[index1].width;
			h2 = Zombies[index1].height;
			
			if(Zombies[index1].status == 1 && status == 1){
				if(Collision_Check(x1, y1, w1, h1, x2, y2, w2, h2) == 1){
					Revert_Pos_Player(&Players[index0]);
					DEATH=1;
				}
			}
		}
		
		for(index1 = 0; index1 < 10; index1++){
			
			x2 = Obstacles[index1].xcoor;
			y2 = Obstacles[index1].ycoor;
			w2 = Obstacles[index1].width;
			h2 = Obstacles[index1].height;
			
			if(Obstacles[index1].status == 1 && status == 1){
				if(Collision_Check(x1, y1, w1, h1, x2, y2, w2, h2) == 1){
					Revert_Pos_Player(&Players[index0]);
				}
			}
		}
	}
}

void Damage_Zombie(Projectile *ptprojectile, Zombie *ptzombie){
	uint8_t damage = ptprojectile->ptplayer->ptweapon->damage;
	ptzombie->hp -= damage;
	if(ptzombie->hp <= 0){
		ptzombie->status = 0;
		ptprojectile->ptplayer->killcounter++;
	}
}

void Damage_Player(Projectile *ptprojectile, Player *ptplayer){
	uint8_t damage = ptprojectile->ptzombie->damage;
	ptplayer->hp -= damage;
	if(ptplayer->hp <= 0){
		ptplayer->status = 0;
	}
}

void Spawn_Zombie(Level *ptlevel){
	if(ZombieCounter < ptlevel->numzombies){
		Zombie *ptzombie = &Zombies[ZombieIndex];
		ptzombie->xcoor = ptlevel->spawnx[ZombieIndex%4];
		ptzombie->ycoor = ptlevel->spawny[ZombieIndex%4];
		ptzombie->status = onfield;
		ZombieCounter++;
		ZombieIndex = (ZombieIndex + 1) % 15;
	}
	else{
		return;
	}
}

void Turn_On_Timers(void){
	if(ZombieTimer1Status == 1){
		TIMER2_TAILR_R= Zombies[0].attackspeed; //Reset Timer
		TIMER2_CTL_R = 0x00000001;    // 10) enable TIMER1A
		ZombieTimer1Status = 0;
	}
	else if(ZombieTimer2Status == 1){
		TIMER3_TAILR_R= Zombies[0].attackspeed; //Reset Timer
		TIMER3_CTL_R = 0x00000001;    // 10) enable TIMER1A
		ZombieTimer2Status = 0;
	}
	if(Players[0].reloading == 1){
		TIMER4_TAILR_R= (&Players[0])->ptweapon->attackspeed; //Reset Timer
		TIMER4_CTL_R = 0x00000001;    // 10) enable TIMER1A
	}
	if(Players[1].reloading == 1){
		TIMER1_TAILR_R= (&Players[1])->ptweapon->attackspeed; //Reset Timer
		TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
	}
}
