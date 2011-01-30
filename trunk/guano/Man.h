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

	vector2f m_pos;
	vector2f m_aim;
	bool m_ready;
	
	enum {
		Facing_South,
		Facing_West,
		Facing_North,
		Facing_East
	} m_facing;
};

#endif
