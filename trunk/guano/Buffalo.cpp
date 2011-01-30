#include "Buffalo.h"
#include "Gamepad.h"
#include "Sprite.h"
#include "Terrain.h"
#include "Game.h"

Buffalo herd[kMaxBuffalo];

static int mFrame = 0;
static int mTime = 0;

void buffInit() {
}

void buffReset() {
	for (int i=0; i<kMaxBuffalo; ++i) {
		herd[i].scale = 1;
		herd[i].bActive = true;
		herd[i].pos = vector2f((rand()%kWorldWidth)*32,(rand()%kWorldHeight)*32);
		herd[i].rad = 12;
		herd[i].facing = (Facing)(Facing_South+rand()%4);
		herd[i].state = (Buffalo::State)( rand()%((int)Buffalo::State_NumStates) );

		herd[i].stage = 1;
		herd[i].ageMs = 0;
		herd[i].hp = 1;
	}
}

void buffRender(Sprite2d* sprite) {
	for (int i=0; i<kMaxBuffalo; ++i) {
		Buffalo& buff = herd[i];
		
		glPushMatrix();
			glTranslatef(buff.pos.x-16*buff.scale, buff.pos.y-16*buff.scale, 0);
			glScalef(buff.scale, buff.scale, 0);
		
		if (!buff.bActive) {
			sprite->draw(kBuffaloFrame+6);
			glPopMatrix();
			continue;
		}
		
		
		if (buff.state == Buffalo::State_Angry) {
			if ((Game::getInstance()->getTime()/666)%2)
				glColor3f(1.0, 0, 0);
			else
				glColor3f(1, 1, 1);
		}
		
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
		
			glColor3f(1, 1, 1);

		
		glPopMatrix();
	}
	
}


void buffUpdate(uint32_t elapsedMs, Gamepad* gamepad) {
	mTime += elapsedMs;
	mFrame = (mTime/300)%2;
	
	vector2f ppos = Game::getInstance()->getPlayerPos();

	for (int i=0; i<kMaxBuffalo; ++i) {
		Buffalo& buff = herd[i];
		if (!buff.bActive)
			continue;
		
		buff.ageMs += elapsedMs;

		if (buff.ageMs > kBuffaloAgeMs) {
			buff.ageMs -= kBuffaloAgeMs;
			if (buff.stage < 4) {
				buff.hp = ++buff.stage;
			}
		}

		// smooth animate the grow
		// set scale according to health, 1:1 for now
		if (buff.hp > buff.scale)
			buff.scale = buff.scale*0.45+buff.hp*0.55;
		
		vector2f dp = ppos-buff.pos;
		float mag = dp.length();
		
		
		if (mag > kBuffDistThreshold)
			continue;
		
		switch (buff.state) {
			case Buffalo::State_Grazing: {
				const float heading = (rand()/(float)RAND_MAX)*M_PI*2;
				buff.pos += vectorFromHeading(heading, kBuffSpeed);
				break;
			}
			case Buffalo::State_Afraid: {
				buff.pos -= dp.normalized()*kBuffSpeed;
				break;
			}
			case Buffalo::State_Angry: {
				buff.pos += dp.normalized()*kBuffSpeed;
				break;
			}
		}
		
	}		
}

void buffHit(int index) {
	if (!herd[index].bActive)
		return;
	
	--herd[index].hp;
	
	if (herd[index].hp <= 0) {
		herd[index].bActive = false;
	}
}

