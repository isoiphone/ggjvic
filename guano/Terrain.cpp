#include "Terrain.h"
#include "Gamepad.h"
#include "Sprite.h"
#include "Game.h"

// art frame position in sprite sheet
#define kGrassFrame 16
#define kDirtFrame	17
#define kRockFrame	18

#define kWorldWidth		50
#define kWorldHeight	50

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

void terrRender(Sprite2d* sprite, Sprite2d* font) {
	Game& game = *Game::getInstance();
	vector2f cam = game.getCameraPos();
//	float zoom = game.getCameraZoom();

	// draw the world!
	for (int y=0; y<kWorldHeight; ++y) {
		glPushMatrix();
	    for(int x = 0; x<kWorldWidth; ++x) {
			vector2f screenPos = vector2f(-cam.x+(x*32), -cam.y+(y*32));
			
			if (screenPos.y < 0-32 ||
				screenPos.y > kScreenHeight || 
				screenPos.x < 0-32 ||
				screenPos.x > kScreenWidth) {
				// if the tile will appear off the edge of the screen, don't draw it
			} else {
				// draw
				sprite->draw(kGrassFrame);

			}
			
			// shift right
			glTranslatef(32, 0, 0);

	    }
		glPopMatrix();
		
		// shift down
		glTranslatef(0, 32, 0);
	}

}

void terrUpdate(uint32_t elapsedMs, Gamepad* gamepad) {
	mTime += elapsedMs;
}

void terrHit(int index) {
}

