#ifndef _GAME_H_INCLUDED
#define _GAME_H_INCLUDED

#include "main.h"

class Gamepad;
class Sprite2d;
class Man;
class Camera;
class Blob;

#define SHOT_FLAGS_ACTIVE	(1<<0)
#define SHOT_FLAGS_R		(1<<1)

typedef struct Shot {
	vector2f	m_pos;
	vector2f	m_vel;
	float		m_rot;
	float		m_rvel;
	uint32_t	m_flags;
};

class Game
{
public:
	Game();
	~Game();

	void render();
	void update(uint32_t elapsedMs, Gamepad* gamepad);

	void spawnRock(vector2f source, vector2f target, vector2f targetVel);

private:
	void startGame();
	void stopGame();
	
	void updateShots(uint32_t elapsedMs);
	void renderShots();
	void spawnShot(vector2f pos, float heading, float speed, uint32_t flags);
	
	Camera*	m_cam;
	Man*	m_man;
	Blob*	m_blobs[kMaxBlobs];
	Shot	m_shots[kMaxShots];
	int		m_shotIndex;
	GLint	m_sparkle;
	
	Sprite2d* m_font;
	
	int32_t m_elapsed;
	uint8_t m_score[4]; // R, G, B, A
};

#endif
