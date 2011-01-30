#include "Man.h"
#include "main.h"
#include "Game.h"
#include "Gamepad.h"
#include "Sprite.h"

#define kManFrame 24
#define kCrosshairFrame	32

#define kMoveSpeed	7
#define kAimDistance 150

// so autocomplete works
#include <SDL_opengl.h>

Man::Man() {
	m_pos = vector2f(100,100);
	m_aim = vector2f(0,0);
	m_ready = false;
};

Man::~Man() {
}

	
void Man::render(Sprite2d* sprite) {
	glPushMatrix();
		glTranslatef(m_pos.x, m_pos.y, 0);
		sprite->draw(kManFrame);
	
		if (m_ready) {
			glTranslatef(m_aim.x*kAimDistance, m_aim.y*kAimDistance, 0);
			sprite->draw(kCrosshairFrame);
		}
	glPopMatrix();
	
	
	
	
//	glScalef(32, 32, 0);
//
//	glBegin(GL_TRIANGLES);
//		glVertex3f(  0.66f, 0.0f, 0.0f);
//		glVertex3f( -0.5f, -0.5f, 0.0f);
//		glVertex3f( -0.5f, +0.5f, 0.0f);
//	glEnd();
	
	
	
}

void Man::update(uint32_t elapsedMs, Gamepad* gamepad) {
	vector2f walk = vector2f(gamepad->getX1(), gamepad->getY1());
	vector2f face = vector2f(gamepad->getX2(), gamepad->getY2());
	
	printf("%2.2f, %2.2f\n", walk.length(), face.length());
	
	if (walk.length() > 5000) {
		m_pos += walk.normalized()*kMoveSpeed;
	}
	
	if (face.length() > 5000) {
		m_aim = face.normalized();
		m_ready = true;
	} else {
		m_ready = false;
	}
}
