// Interaction.h
// Runs on LM4F120/TM4C123
// Michael Hernendez and Arkan Abuyazid
// This is a starter project for the EE319K Lab 10

// Last Modified: 5/3/2017
//*******************************************

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Collision.h"

void MoveY_Player(Player *ptplayer);
void MoveX_Player(Player *ptplayer);
void Turn_Player(Player *ptplayer, uint8_t newdirection);
void MoveX_Zombie (Zombie *ptzombie);
void MoveY_Zombie (Zombie *ptzombie);
void MoveY_Projectile(Projectile *ptprojectile);
void MoveX_Projectile(Projectile *ptprojectile);
void Turn_Zombie (Zombie *ptzombie, uint8_t newdirection);
void Attack(Zombie *ptzombie);
void Attack_Flag(Zombie *ptzombie);
void Shoot_Player(Projectile *ptprojectile, Player *ptplayer);
void Shoot_Zombie(Projectile *ptprojectile, Zombie *ptzombie);
void Shoot_Flag(Player *ptplayer);
void Damage_Player(Projectile *ptprojectile, Player *ptplayer);
void Spawn_Zombie(Level *ptlevel);
void Projectile_Off(Projectile *ptprojectile);
void Zombie_Off(Zombie *ptzombie);
void Player_Off(Player *ptplayer);
void Zombie_Collision(void);
void Player_Collision(void);
void Projectile_Collision(void);
void Revert_Pos_Player(Player *ptplayer);
void Revert_Pos_Zombie(Zombie *ptzombie);
void Damage_Zombie(Projectile *ptprojectile, Zombie *ptzombie);
