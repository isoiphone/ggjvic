#include "Man.h"
#include "main.h"
#include "Game.h"
#include "Gamepad.h"
#include "Sprite.h"

// so autocomplete works
#include <SDL_opengl.h>

Man::Man() {
	m_pos = vector2f(100,100);
	m_rot = 0;
	m_speed = 0;
	
	m_color[0] = 0.1;
	m_color[1] = 0.1;
	m_color[2] = 0.8;
};

Man::~Man() {
}

	
void Man::render() {
	glColor3fv(m_color);
	
	glPushMatrix();
	
	// position
	glTranslatef(m_pos.x, m_pos.y, 0);
	glRotatef(m_rot*RADIANS2DEGREES, 0, 0, 1);
	glScalef(32, 32, 0);

	glBegin(GL_TRIANGLES);
		glVertex3f(  0.66f, 0.0f, 0.0f);
		glVertex3f( -0.5f, -0.5f, 0.0f);
		glVertex3f( -0.5f, +0.5f, 0.0f);
	glEnd();
	
	glPopMatrix();
	
}

#define kMinSpeed	-13.0
#define kMaxSpeed	 20.0
#define kAccelSpeed	 12.0
#define kDecelSpeed	 3.0
#define kTurnSpeed	-0.1

void Man::update(uint32_t elapsedMs, Gamepad* gamepad) {
	if (gamepad->isHeld(GAMEPAD_LEFT)) {
		m_rot += kTurnSpeed;
	}
	
	if (gamepad->isHeld(GAMEPAD_RIGHT)) {
		m_rot -= kTurnSpeed;
	}
	
	if (gamepad->isHeld(GAMEPAD_UP)) {
		m_speed = MAX(kMinSpeed, MIN(kMaxSpeed, m_speed+(kAccelSpeed*elapsedMs*0.001)));
	} else if (gamepad->isHeld(GAMEPAD_DOWN)) {
		m_speed = MAX(kMinSpeed, MIN(kMaxSpeed, m_speed-(kAccelSpeed*elapsedMs*0.001)));
	} else {
		if (m_speed > kDecelSpeed) {
			m_speed -= kDecelSpeed;
		} else if (m_speed < -kDecelSpeed) {
			m_speed += kDecelSpeed;
		} else {
			m_speed = 0;
		}

	}
	
	m_pos += vectorFromHeading(m_rot, m_speed);
}
