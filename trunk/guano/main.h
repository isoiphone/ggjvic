#ifndef _MAIN_H_INCLUDED
#define _MAIN_H_INCLUDED

#include <SDL.h>
#include <SDL_opengl.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "mt19937ar.h"
#include "math/Utility.h"
#include "math/vector2.h"
#include "math/Rect.h"

// global constants
#define kWindowTitle	"GGJ"

#define	kScreenWidth	640
#define	kScreenHeight	480
#define	kScreenBpp		32
#define kScreenFlags	SDL_OPENGL

/*
#define	kScreenWidth	1280
#define	kScreenHeight	1024
#define	kScreenBpp		32
#define kScreenFlags	SDL_OPENGL|SDL_FULLSCREEN
*/

#define kMsPerStep		(1000/30)

// game constants
#define kCameraChaseSpeed		0.066
#define kCameraMovePerTick		8
#define kManRotateSpeed			0.2

#define kManPerTick				6

#define	kMaxShots				128
#define kShotSpeed				800.0		// pixels/sec

#define kParticleSize			8


// art constants
#define kBuffaloFrame		0
#define kOldBuffaloFrame	8
#define kManFrame			16
#define kGrassFrame			24
#define kDirtFrame			25
#define kRockFrame			32
#define kCrosshairFrame		40

enum Facing {
	Facing_South,
	Facing_West,
	Facing_North,
	Facing_East
};


// helper functions
class Mix_Chunk;
void initVideo();
void initAudio();
Mix_Chunk* loadSound(const char* const filename);
void unloadSound(Mix_Chunk** sound);
void playSound(Mix_Chunk* sound, int channel=-1);
GLuint loadTexture(const char* filename, uint16_t* width=NULL, uint16_t* height=NULL);
void unloadTexture(GLuint* texture);
void get_bundle_path(const char* const filename, char* newpath, size_t maxlen);

#define RANDRANGE(a,b) (genrand_real1()*((b)-(a))+(a))
#define SMOOTHSTEP(x) ((x) * (x) * (3 - 2 * (x)))
#define	RADIANS2DEGREES	57.2957795

// heading in radians -> unit vector
inline vector2f vectorFromHeading(float heading, float scale=1.0f) {
	return vector2f( cosf(heading)*scale, sinf(heading)*scale );
}

inline float headingFromVector(vector2f vector) {
//	if (vector.x == 0 && vector.y == 0)
//		return 0;
	
	float theta = atan2f(vector.y, vector.x);
	if (theta < 0)
		theta += M_PI*2.0f;
	return theta;
}

#endif
