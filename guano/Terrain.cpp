#include "Terrain.h"
#include "Gamepad.h"
#include "Sprite.h"
#include "Game.h"
#include "perlin.h"

static struct {
	uint8_t tile;
	uint8_t rock;
} mTile[kWorldWidth][kWorldHeight];


Obstacle obstacles[kMaxObstacles];
static int mTime = 0;
static Perlin mNoise(4,4,1,94); //Perlin(int octaves,float freq,float amp,int seed);
					 
void terrInit() {
	for (int y=0; y<kWorldHeight; ++y) {
	    for(int x = 0; x<kWorldWidth; ++x) {
			float v = mNoise.Get(x/(float)kWorldWidth, y/(float)kWorldHeight);
			float r = (v+1.0)*0.5;
			
			if (r < 0.5) {
				mTile[x][y].tile = kDirtFrame;
			} else {
				mTile[x][y].tile = kGrassFrame;
			}
			
			if (r < 0.35) {
				mTile[x][y].rock = kRockFrame+(rand()%2);
			} else {
				mTile[x][y].rock = 0;
			}
		}
	}	
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
				sprite->draw(mTile[x][y].tile);
				if (mTile[x][y].rock) {
					sprite->draw(mTile[x][y].rock);
				}
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

