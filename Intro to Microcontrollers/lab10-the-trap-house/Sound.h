// Sound.h
// Runs on TM4C123 or LM4F120
// Prototypes for basic functions to play sounds from the
// original Space Invaders.
// Jonathan Valvano
// November 17, 2014
#include "AI.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "DAC.h"

void Sound_Init(void);
void Sound_Play(uint32_t period);
void Sound_Shoot(void);
void Sound_Killed(void);
void Sound_Explosion(void);

void Sound_Fastinvader1(void);
void Sound_Fastinvader2(void);
void Sound_Fastinvader3(void);
void Sound_Fastinvader4(void);
void Sound_Highpitch(void);

