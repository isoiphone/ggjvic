#ifndef _BUFFALO_H_INCLUDED
#define _BUFFALO_H_INCLUDED

#include "main.h"
class Gamepad;

#define	kMaxBuffalo 50

typedef struct Buffalo {
	vector2f pos;
	float rad;
	bool bActive;
};

extern Buffalo herd[kMaxBuffalo];

// one-time setup crap
void buffInit();

// set up the level
void buffReset();

// draw a frame
void buffRender();

// time passes
void buffUpdate(uint32_t elapsedMs, Gamepad* gamepad);

// a shot collided with some active buffalo
void buffHit(int buffaloIndex);

#endif