#ifndef _CAMERA_H_INCLUDED
#define _CAMERA_H_INCLUDED

#include "main.h"
class Gamepad;

class Camera
{
public:
	Camera();
	~Camera();

	void update(uint32_t elapsedMs, Gamepad* gamepad);
	
	vector2f getCenter();
	
	vector2f m_pos;
	float m_zoom;
};

#endif
