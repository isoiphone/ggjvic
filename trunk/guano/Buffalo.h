#ifndef _BUFFALO_H_INCLUDED
#define _BUFFALO_H_INCLUDED

#include "main.h"
class Gamepad;
class Sprite2d;


#define NEIGHBOR_THRESHOLD 70

#define STATIC_WEIGHT 16
#define COHESION_WEIGHT 6.7f
#define ALIGNMENT_WEIGHT 2.3f
#define SEPARATION_WEIGHT 4.7f

#define TARGET_WEIGHT 13.2f

#define COLLISION_WEIGHT 10
#define RANDOM_WEIGHT 6.5f
#define PI 3.14159265


typedef struct Buffalo {
	vector2f pos;
	vector2f vel;
	float rad;


    float bearing;//0-2PI

	float speed;
	float scale;
	bool bActive;

	// 1: new born
	// 2: young
	// 3: adult
	// 4: elderly
	int stage;
	int ageMs;
	int hp;

	enum State {
		State_Grazing,
		State_Afraid,
		State_Angry,
		State_NumStates,
	} state;
	Facing facing;
};

extern Buffalo herd[kMaxBuffalo];

// one-time setup crap
void buffInit();

// set up the level
void buffReset();

// draw a frame
void buffRender(Sprite2d* sprite);

// time passes
void buffUpdate(uint32_t elapsedMs, Gamepad* gamepad);

// a shot collided with some active buffalo
void buffHit(int index);

//helper
float normalize(float i);

//mixes bearings
float mix(float base, float mix, int weight);

#endif
