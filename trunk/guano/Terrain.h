#ifndef _TERRAIN_H_INCLUDED
#define _TERRAIN_H_INCLUDED

#include "main.h"
class Gamepad;
class Sprite2d;

#define	kMaxObstacles 10

typedef struct Obstacle {
	vector2f pos;
	float rad;
	bool bActive;
};

extern Obstacle obstacles[kMaxObstacles];

// one-time setup crap
void terrInit();

// set up the level
void terrReset();

// draw a frame
void terrRender(Sprite2d* sprite);

// time passes
void terrUpdate(uint32_t elapsedMs, Gamepad* gamepad);

// a shot collided with some active obstacle
void terrHit(int index);

#endif
