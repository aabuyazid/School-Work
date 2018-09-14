// Collision.h
// Runs on LM4F120/TM4C123
// Michael Hernendez and Arkan Abuyazid
// This is a starter project for the EE319K Lab 10

// Last Modified: 5/3/2017
//*******************************************

#include "Struct.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"
#define player 1
#define zombie 2

int8_t Collision_Check(int16_t x1, int16_t y1, uint8_t w1, uint8_t h1, int16_t x2, int16_t y2, uint8_t w2, uint8_t h2);
int8_t In_Range(Zombie *ptzombie);
int8_t Projectile_Collision_Check(Projectile *ptprojectile, int8_t x2, int8_t y2, int8_t w2, int8_t h2);
int8_t Within_Range(Projectile *ptprojectile);
int8_t abs(int8_t v);
