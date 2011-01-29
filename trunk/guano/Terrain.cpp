#include "Terrain.h"
#include "Gamepad.h"
#include "Sprite.h"
#include "Game.h"

// art frame position in sprite sheet
#define kGrassFrame 16
#define kDirtFrame	17
#define kRockFrame	18

Obstacle obstacles[kMaxObstacles];
static int mTime = 0;

void terrInit() {
}

void terrReset() {
	for (int i=0; i<kMaxObstacles; ++i) {
		obstacles[i].bActive = true;
		obstacles[i].pos = vector2f(200,200);
		obstacles[i].rad = 15;
	}
}

void terrRender(Sprite2d* sprite) {
	glPushMatrix();
		glTranslatef(200, 200, 0);
		sprite->draw(kGrassFrame); 
	glPopMatrix();
	
	// get game this way
	vector2f pos = Game::getInstance()->getCameraPos();
	float zoom = Game::getInstance()->getCameraZoom();
	
	// or if you prefer
	Game& game = *Game::getInstance();
	zoom = game.getCameraZoom();
}

void terrUpdate(uint32_t elapsedMs, Gamepad* gamepad) {
	mTime += elapsedMs;
}

void terrHit(int index) {
}

