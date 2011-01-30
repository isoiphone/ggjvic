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

	void hit();
	bool isDead();
	
	int m_health;
	vector2f m_pos;
	vector2f m_aim;
	bool m_ready;
	
	Facing m_facing;
	int m_frame;
	uint32_t m_walktime;
};

#endif
