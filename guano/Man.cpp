#include "Man.h"
#include "main.h"
#include "Game.h"
#include "Gamepad.h"
#include "Sprite.h"

#define kMoveSpeed	7
#define kAimDistance 150

// so autocomplete works
#include <SDL_opengl.h>


Man::Man() {
	m_pos = vector2f(100,100);
	m_aim = vector2f(0,0);
	m_ready = false;
	m_facing = Facing_South;
	m_frame = 0;
};

Man::~Man() {
}


void Man::render(Sprite2d* sprite) {
	glPushMatrix();
		glTranslatef(m_pos.x, m_pos.y, 0);

	switch (m_facing) {
		case Facing_South:
			sprite->draw(kManFrame+m_frame);
			break;
		case Facing_West:
			sprite->draw(kManFrame+m_frame+2);
			break;
		case Facing_North:
			sprite->draw(kManFrame+m_frame+4);
			break;
		case Facing_East:
			glPushMatrix();
			glTranslatef(32, 0, 0);
			glScalef(-1, 1, 1);
			sprite->draw(kManFrame+m_frame+2);
			glPopMatrix();
			break;
	}

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

	if (walk.length() > 5000) {
		m_pos += walk.normalized()*kMoveSpeed;
		m_walktime += elapsedMs;
		m_frame = (m_walktime/250)%2;
	}

	if (face.length() > 5000) {
		m_aim = face.normalized();
		m_ready = true;

		static const float kThird = 1.0/3.0;
		if (m_aim.y < -kThird) {
			m_facing = Facing_North;
		} else if (m_aim.y > kThird) {
			m_facing = Facing_South;
		} else if (m_aim.x < 0) {
			m_facing = Facing_West;
		} else {
			m_facing = Facing_East;
		}
	} else {
		m_ready = false;
	}
}

void Man::hit() {
}
