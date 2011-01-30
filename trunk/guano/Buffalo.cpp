#include "Buffalo.h"
#include "Gamepad.h"
#include "Sprite.h"

Buffalo herd[kMaxBuffalo];

static int mFrame = 0;
static int mTime = 0;

void buffInit() {
}

void buffReset() {
	for (int i=0; i<kMaxBuffalo; ++i) {
		herd[i].bActive = true;
		herd[i].pos = vector2f(100,100);
		herd[i].rad = 10;
	}
}

void buffRender(Sprite2d* sprite) {
	glPushMatrix();
		glTranslatef(100, 100, 0);
	glScalef(10, 10, 0);
		sprite->draw(mFrame);
	glPopMatrix();
	
}

void buffUpdate(uint32_t elapsedMs, Gamepad* gamepad) {
	mTime += elapsedMs;
	mFrame = (mTime/300)%2;
}

void buffHit(int buffaloIndex) {
}

