#include "Buffalo.h"
#include "Gamepad.h"
#include "Sprite.h"
#include "Terrain.h"

Buffalo herd[kMaxBuffalo];

static int mFrame = 0;
static int mTime = 0;

void buffInit() {
}

void buffReset() {
	for (int i=0; i<kMaxBuffalo; ++i) {
		herd[i].scale = 2;
		herd[i].bActive = true;
		herd[i].pos = vector2f((rand()%kWorldWidth)*32,(rand()%kWorldHeight)*32);
		herd[i].rad = 12;
		herd[i].facing = (Facing)(Facing_South+rand()%4);
	}
}

void buffRender(Sprite2d* sprite) {
	for (int i=0; i<kMaxBuffalo; ++i) {
		Buffalo& buff = herd[i];
		if (!buff.bActive)
			continue;

		glPushMatrix();
			glTranslatef(buff.pos.x, buff.pos.y, 0);
			glScalef(buff.scale, buff.scale, 0);
		
		switch (buff.facing) {
			case Facing_South:
				sprite->draw(kBuffaloFrame+mFrame);
				break;
			case Facing_West:
				sprite->draw(kBuffaloFrame+mFrame+2);
				break;
			case Facing_North:
				sprite->draw(kBuffaloFrame+mFrame+4);
				break;
			case Facing_East:
				glPushMatrix();
				glTranslatef(32, 0, 0);
				glScalef(-1, 1, 1);
				sprite->draw(kBuffaloFrame+mFrame+2);
				glPopMatrix();
				break;
		}

		
		glPopMatrix();
	}
	
}

void buffUpdate(uint32_t elapsedMs, Gamepad* gamepad) {
	mTime += elapsedMs;
	mFrame = (mTime/300)%2;
}

void buffHit(int buffaloIndex) {
}

