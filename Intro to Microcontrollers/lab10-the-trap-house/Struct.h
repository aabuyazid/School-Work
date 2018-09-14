// AI.h
// Runs on LM4F120/TM4C123
// Michael Hernendez and Arkan Abuyazid
// This is a starter project for the EE319K Lab 10

// Last Modified: 5/3/2017
//*******************************************

#include <stdint.h>
#include "tm4c123gh6pm.h"
#define normal 0
#define fast 1
#define fat 2
#define sniping 3
#define onfield 1
#define offfield 0
#define slow 1
#define regular 2
#define quick 3
#define stay 0
#define up  1
#define down 3
#define left 4
#define right 2
#define quartersecond 0xEE6B280
#define halfsecond   0x1DCD6500
void Round2_Init(void);
void Round3_Init(void);
void Round4_Init(void);
void Round2_Zombies_Init(void);
void Round3_Zombies_Init(void);
void Round4_Zombies_Init(void);
void Round2_Players_Init(void);
void Round3_Players_Init(void);
void Round4_Players_Init(void);
void Round1_Animation_Init(void);
void Round2_Animation_Init(void);
void Round3_Animation_Init(void);
void Round4_Animation_Init(void);
void Print_AllObjects (void);
void Print_Player (void);
void Print_Zombies (void);
void Print_Projectiles (void);

void Player1_ADC (void);
void Game_Init (void);
struct Animation{
const unsigned short *UP;
const unsigned short *RIGHT;
const unsigned short *DOWN;
const unsigned short *LEFT;
};
struct Player_Animation{
const unsigned short *UP;
const unsigned short *RIGHT;
const unsigned short *DOWN;
const unsigned short *LEFT;
};
struct Zombie{
	
	int8_t hp;
	uint8_t type;
	uint8_t movespeed;
	uint32_t attackspeed;
	uint8_t damage;
	uint8_t range;
	uint8_t status;
	uint8_t attacking;
	uint8_t direction;
	int16_t xcoor;
	int16_t ycoor;
	int16_t prevx;
	int16_t prevy;
	uint8_t width;
	uint8_t height;
	uint8_t turned;
	uint8_t dead;
	
};

struct Obstacle{
	
	int16_t xcoor;
	int16_t ycoor;  //these were int8_t
	uint8_t width;
	uint8_t height;
	const uint16_t *image;
	uint8_t status;// 
	
};

void Round2_Init(void);
struct Weapon{
	
	uint8_t damage;
	uint8_t range;
	uint8_t shotspeed;
	uint32_t attackspeed;
	char *ptname;
	
};

struct Player{
	
	int8_t hp;
	struct Weapon *ptweapon;
	uint8_t killcounter;
	uint8_t direction;
	uint8_t status;
	uint8_t turned;
	uint8_t reloading;
	int16_t xcoor;
	int16_t ycoor;
	int16_t prevx;
	int16_t prevy;
	uint8_t width;
	uint8_t height;
	const uint16_t *ptimageup;
	const uint16_t *ptimagedown; 
	const uint16_t *ptimageleft; 
	const uint16_t *ptimageright; 	//&name
	uint8_t dead;
};

struct Projectile{
	
	struct Player *ptplayer;
	struct Zombie *ptzombie;
	uint8_t shooter;
	uint8_t status;
	uint8_t width;
	uint8_t height;
	int16_t xcoor;
	int16_t ycoor;
	int16_t initialx;
	int16_t initialy;
	int16_t prevx;
	int16_t prevy;
	uint8_t direction;
	const uint16_t *ptimagex;
	const uint16_t *ptimagey;
	uint8_t dead;
};

typedef struct Zombie Zombie;
typedef struct Player Player;
typedef struct Projectile Projectile;
typedef struct Obstacle Obstacle;
typedef struct Weapon Weapon;
typedef struct Animation Animation;
typedef struct Player_Animation Player_Animation;
struct Level{  //map level 1-4
	
	uint8_t maplevel; 
	uint8_t numzombies; //15
	uint8_t playerx [2]; //initial coordinates for x
	uint8_t playery [2]; //
	uint8_t spawnx [4];  //initial zombie spawns  
	uint8_t spawny [4];
	
};


typedef const struct Level Level;



