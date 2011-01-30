#ifndef _MAN_H_INCLUDED
#define _MAN_H_INCLUDED

#include "main.h"
class Gamepad;
class Sprite2d;

class Man
{
public:
	Man();
	~Man();

	void render(Sprite2d* sprite);
	void update(uint32_t elapsedMs, Gamepad* gamepad);

	// position of back/bottom
	vector2f m_pos;
	
	// heading
	float	m_rot;
	float	m_speed;
	float	m_color[3];
};

#endif
