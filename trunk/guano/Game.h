#ifndef _GAME_H_INCLUDED
#define _GAME_H_INCLUDED

#include "main.h"
class Gamepad;
class Sprite2d;
class Man;
class Camera;

typedef struct Shot {
	vector2f	m_pos;
	vector2f	m_vel;
	bool		m_bActive;
};

class Game
{
public:
	static Game* getInstance();
	
	void render();
	void update(uint32_t elapsedMs, Gamepad* gamepad);

	vector2f getCameraPos();
	float getCameraZoom();
	
private:
	Game();
	~Game();
	
	void startGame();
	void stopGame();
	
	void updateShots(uint32_t elapsedMs);
	void renderShots();
	void spawnShot(vector2f pos, vector2f heading, float speed);
	
	Camera*	m_cam;
	Man*	m_man;
	Shot	m_shots[kMaxShots];
	int		m_shotIndex;
	GLint	m_sparkle;
	
	Sprite2d* m_font;
	Sprite2d* m_buffalos;
	
	int32_t m_elapsed;
};

#endif
