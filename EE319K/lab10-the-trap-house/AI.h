// AI.h
// Runs on LM4F120/TM4C123
// Michael Hernendez and Arkan Abuyazid
// This is a starter project for the EE319K Lab 10

// Last Modified: 5/3/2017
//*******************************************


#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Interaction.h"
#include "ADC.h"

uint8_t Compare_Zombie_Player(Zombie *ptzombie);
uint8_t Compare_Zombie_Coords(Zombie *ptzombie);
uint8_t Compare_Zombie_Update_Direction(Zombie *ptzombie);
void Zombie_Number(struct Zombie *Zombies,Zombie *ptzombie);
uint8_t Snipe_Zombie_Compare_Update_Direction(Zombie *ptzombie);
void Move_All_Objects(void);
void Move_Players(void);
void Move_Zombies(void);
void Move_Projectiles(void);
uint8_t Player1_Direction(void);
uint8_t Player2_Direction(void);
void Players_Move(void);
void Projectiles_Move(void);
void Players_Turn(void);
void Zombies_Turn(void);
void Zombies_Move(void);
void Players_Shoot(uint8_t button1, uint8_t button2);
