// AI.c
// Runs on LM4F120/TM4C123
// Michael Hernandez and Arkan Abuyazid
// This is a starter project for the EE319K Lab 10

// Last Modified: 5/3/2017
//*******************************************

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "AI.h"
extern Animation Animations[2];
extern Zombie Zombies[15];
extern Player Players[2];
extern Projectile Projectiles[20];
extern uint8_t ProjectileIndex;
extern uint8_t UARTMailboxDir;
extern uint8_t UARTMailboxBut;
extern uint8_t PORTFMailbox;
//------Checks to see which Player the zombie is closer to-----
//Input: zombie
// Output: 0, if dead  or both players dead, 1 if player 1, 2 if player 2.
uint8_t Compare_Zombie_Player(Zombie *ptzombie){
	int16_t comparex, comparey;
	if (ptzombie->status==1){
		if(Players[0].status ==1 && Players[1].status ==1){
		comparex = (Players[0].xcoor-ptzombie->xcoor);
		comparey = (Players[0].ycoor-ptzombie->ycoor);
			if((comparex>Players[1].xcoor-ptzombie->xcoor)||(comparey>Players[1].ycoor-ptzombie->ycoor)){
				return 2;
			}
			else{
			return 1;
			}
		}
		if(Players[0].status ==1){
			return 1;
		}
		if(Players[1].status ==1){
			return 2;
		}
	}
  return 0;	
}

//Checks which coordinate the zombie is closer to-----
//Input: Zombie
//Output: if player 1 coordinate check are x = (1) or y = 2 or 5 if equal, if player 2 x(3), y(4), or 6 if equal, 0 for does not matter
uint8_t Compare_Zombie_Coords(Zombie *ptzombie){
	int16_t Coord;
	uint8_t Check;
	Check= Compare_Zombie_Player(ptzombie);
	if(Check==0){
		return 0;
	}
	if(Check==1){
		if(abs(Players[0].xcoor-ptzombie->xcoor)>abs(Players[0].ycoor-ptzombie->ycoor)){
			return 2;
		}
		if(abs(Players[0].xcoor-ptzombie->xcoor)==abs(Players[0].ycoor-ptzombie->ycoor)){
			return 5;
		}
		else{
			return 1;
		}
	}
	if(abs(Players[1].xcoor-ptzombie->xcoor)>abs(Players[1].ycoor-ptzombie->ycoor)){
		return 4;
	}
	if(abs(Players[1].xcoor-ptzombie->xcoor)==abs(Players[1].ycoor-ptzombie->ycoor)){
	  return 6;
	}
	return 3;
}

//Moves zombie towards those coordinates-----
//Input: Zombie
//Output: returns the new direction of zombie
//if player 1 the coordinate x (1) or y (2) or 5 if equal, if player 2 x(3), y(4), or 6 if equal, 0 for does not matter
uint8_t Compare_Zombie_Update_Direction(Zombie *ptzombie){
	uint8_t Check;
	Check= Compare_Zombie_Coords(ptzombie);
	if(Check ==0){
		return (stay);
	}
	if(Check ==1){
		if(Players[0].xcoor-ptzombie->xcoor>0){
			//ptzombie->xcoor+=ptzombie->movespeed; 
			return right;
		}
		if(Players[0].xcoor-ptzombie->xcoor<0){
			//ptzombie->xcoor-=ptzombie->movespeed; 
			return left;
		}
		if(Players[0].ycoor-ptzombie->ycoor>0){
			//ptzombie->ycoor+=ptzombie->movespeed;
			return down;
		}
		if(Players[0].ycoor-ptzombie->ycoor<0){
			//ptzombie->ycoor-=ptzombie->movespeed; 
			return up;
		} //may have to come back and update y since xPlayer and xZombie equal
	}
	if(Check ==2){
		if(Players[0].ycoor-ptzombie->ycoor>0){
			//ptzombie->ycoor+=ptzombie->movespeed; 
			return down;
		}
		if(Players[0].ycoor-ptzombie->ycoor<0){
			//ptzombie->ycoor-=ptzombie->movespeed; 
			return up;
		}
		if(Players[0].xcoor-ptzombie->xcoor>0){
			ptzombie->xcoor+=ptzombie->movespeed; 
			return right;
		}
		if(Players[0].xcoor-ptzombie->xcoor<0){
			ptzombie->xcoor-=ptzombie->movespeed; 
			return left;
		}
		//return; //may have to come back and update y since xPlayer and xZombie equal
	}
	if(Check ==3){
		if(Players[1].xcoor-ptzombie->xcoor>0){
			//ptzombie->xcoor+=ptzombie->movespeed;
			return right;
		}
		if(Players[1].xcoor-ptzombie->xcoor<0){
			//ptzombie->xcoor-=ptzombie->movespeed;
			return left;
		}
		if(Players[1].ycoor-ptzombie->ycoor>0){
			//ptzombie->ycoor+=ptzombie->movespeed; 
			return down;
		}
		if(Players[1].ycoor-ptzombie->ycoor<0){
			//ptzombie->ycoor-=ptzombie->movespeed; 
			return up;
		}
		//return; //may have to come back and update y since xPlayer and xZombie equal
	}
	if(Check ==4){
		if(Players[1].ycoor-ptzombie->ycoor>0){
			//ptzombie->ycoor+=ptzombie->movespeed;
			return down;			
		}
		if(Players[1].ycoor-ptzombie->ycoor<0){
			//ptzombie->ycoor-=ptzombie->movespeed; 
			return up ;
		}
		if(Players[1].xcoor-ptzombie->xcoor>0){
			//ptzombie->xcoor+=ptzombie->movespeed;
			return right;
		}
		if(Players[1].xcoor-ptzombie->xcoor<0){
			//ptzombie->xcoor-=ptzombie->movespeed;
			return left;
		}
		//return; //may have to come back and update y since xPlayer and xZombie equal		
	}
	if(Check ==5){
		return stay;
	}
	if(Check ==6){
		return stay;
	}
	return stay;
}
//Moves zombie towards those coordinates-----
//Input: Zombie
//Output: none, updates zombie struct
//if player 1 the coordinate x (1) or y (2) or 5 if equal, if player 2 x(3), y(4), or 6 if equal, 0 for does not matter
uint8_t Snipe_Zombie_Compare_Update_Direction(Zombie *ptzombie){
		uint8_t Check;
	Check= Compare_Zombie_Coords(ptzombie);
	if(Check ==0){
		return stay;
	}
	if(Check ==1){
		if(Players[0].xcoor-ptzombie->xcoor>0){
			//ptzombie->xcoor+=ptzombie->movespeed; 
			return right;
		}
		if(Players[0].xcoor-ptzombie->xcoor<0){
			//ptzombie->xcoor-=ptzombie->movespeed; 
			return left;
		}
//ZOMBIE SHOULD ATTACK HERE, SO WHAT FLAG DO I set?
		//return; //may have to come back and update y since xPlayer and xZombie equal
	}
	if(Check ==2){
		if(Players[0].ycoor-ptzombie->ycoor>0){
			//ptzombie->ycoor+=ptzombie->movespeed; 
			return down;
		}
		if(Players[0].ycoor-ptzombie->ycoor<0){
			//ptzombie->ycoor-=ptzombie->movespeed; 
			return up;
		}
//ZOMBIE SHOULD ATTACK HERE, SO WHAT FLAG DO I set?
		//return; //may have to come back and update y since xPlayer and xZombie equal
	}
	if(Check ==3){
		if(Players[1].xcoor-ptzombie->xcoor>0){
			//ptzombie->xcoor+=ptzombie->movespeed;
			return right;
		}
		if(Players[1].xcoor-ptzombie->xcoor<0){
			//ptzombie->xcoor-=ptzombie->movespeed;
			return left;
		}
//ZOMBIE SHOULD ATTACK HERE, SO WHAT FLAG DO I set?
		//return; //may have to come back and update y since xPlayer and xZombie equal
	}
	if(Check ==4){
		if(Players[1].ycoor-ptzombie->ycoor>0){
			//ptzombie->ycoor+=ptzombie->movespeed;
			return right;			
		}
		if(Players[1].ycoor-ptzombie->ycoor<0){
			//ptzombie->ycoor-=ptzombie->movespeed; 
			return left;
		}
//ZOMBIE SHOULD ATTACK HERE, SO WHAT FLAG DO I set?
		//return; //may have to come back and update y since xPlayer and xZombie equal		
	}
	if(Check ==5){
		return stay;
	}
	if(Check ==6){
		return stay;
	}
	return stay;
}
/*void Zombie_Number(struct Zombie *Zombies, Zombie *ptzombie){// how do I pass the struct of the zombie? //Passed by address or Zombies
	uint8_t i;
	if(ptzombie->type==sniping){
		for (i=0;i<15;i++){
			Snipe_Zombie_Compare(Zombies +i);
		}
	}
	else{
	  for(i=0;i<15;i++){
		  Compare_Zombie_Update(Zombies + i); // Is this right  // Or &Zombies?
	  }
  }
}*/
void Move_Players(void){	
	Players_Move();
}
void Move_Zombies(void){
	Zombies_Turn();
	Zombies_Move();
}
void Move_Projectiles(void){
	Projectiles_Move();
}
void Move_All_Objects(void){
	
	//Players_Turn();
	Players_Move();
	Zombies_Turn();
	Zombies_Move();
	Projectiles_Move();
}

uint8_t Player1_Direction(void){
	uint32_t k=0, l=0;
	char coordinatesJS=0x30;
	CoordPot temp =	ADC_In();
	k= temp.x;
	l= temp.y;
	if((k>1500)&&(k<2800)){
		if(l<1500){
	  	coordinatesJS=0x34;
		}
		if(l>2800){
	  	coordinatesJS=0x32;
		}
	}
	if((l>1500)&&(l<2800)){
		if(k<1500){
	 		coordinatesJS=0x31;
		}
		if(k>2800){
	 		coordinatesJS=0x33;
		}
	}
	if(((k>1500)&&(k<2800))&&((l>1500)&&(l<2800))){
		coordinatesJS=0x30;
	}
	if(coordinatesJS == 0x34){
		return(down);
	}
	else if(coordinatesJS == 0x33){
		return(right);
	}
	else if (coordinatesJS == 0x32){
		return(up);
	}
	else if(coordinatesJS == 0x31){
		return(left);
	}
	else if (coordinatesJS == 0x30){
		return(0);
	}
}


uint8_t Player2_Direction(void){
	uint8_t coordinatesJS2 =	UARTMailboxDir;
		if(coordinatesJS2 == 0x34){
		return(down);
	}
	else if(coordinatesJS2 == 0x33){
		return(right);
	}
	else if (coordinatesJS2 == 0x32){
		return(up);
	}
	else if(coordinatesJS2 == 0x31){
		return(left);
	}
	else{
		return(0);
	}

} // work on this function Michael pls
void Players_Turn(void){
	Turn_Player(&Players[0], Player1_Direction());
  Turn_Player(&Players[1], Player2_Direction());
}

void Players_Move(void){
	uint8_t direction1 = Player1_Direction();
 uint8_t direction2 = Player2_Direction();
	if(direction1 != stay){
		Turn_Player(&Players[0], direction1);
		MoveX_Player(&Players[0]);
		MoveY_Player(&Players[0]);
	}
	if(direction2 != stay){
		Turn_Player(&Players[1], direction2);
		MoveX_Player(&Players[1]);
		MoveY_Player(&Players[1]);
	}
}

void Zombies_Turn(void){
	uint8_t i;
	Zombie *ptzombie;
	for(i = 0; i < 15; i++){
		ptzombie = &Zombies[i];
		if(ptzombie->type != sniping){
			Turn_Zombie(ptzombie, Compare_Zombie_Update_Direction(ptzombie));
			}
		else{
			Turn_Zombie(ptzombie, Snipe_Zombie_Compare_Update_Direction(ptzombie));
		}
	}
}

void Zombies_Move(void){
	uint8_t i;
	Zombie *ptzombie;
	for(i = 0; i <1; i++){
		ptzombie = &Zombies[i];
		if(ptzombie->status == 1){
			MoveX_Zombie(ptzombie); 
			MoveY_Zombie(ptzombie);
		}
	}
}

void Projectiles_Move(void){
	uint8_t i;
	Projectile *ptprojectile;
	for(i = 0; i < 20; i++){
		ptprojectile = &Projectiles[i];
		if(ptprojectile->status == 1){
			MoveX_Projectile(ptprojectile);
			MoveY_Projectile(ptprojectile);
		}
	}
}
		
void Players_Shoot(uint8_t button1, uint8_t button2){
	Player *ptplayer1 = &Players[0];
	Player *ptplayer2 = &Players[1];
	if(button1 == 0x35){
		Shoot_Player(&Projectiles[ProjectileIndex], ptplayer1);
		ptplayer1->reloading = 1;
	}
	if(button2 == 0x40){
		Shoot_Player(&Projectiles[ProjectileIndex], ptplayer2);
		ptplayer2->reloading = 1;
	}
	PORTFMailbox = 0;
	UARTMailboxBut = 0;
}
