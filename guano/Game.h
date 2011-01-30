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

	vector2f getPlayerPos();	
	vector2f getCameraPos();
	float getCameraZoom();
	
	uint32_t getTime() { return m_elapsed; }
	
	void hitMan(int buffalo);
	
	void playBuffaloSound();
	void playShootSound();
	void playHitSound();
	void playPopSound();
	
private:
	Game();
	~Game();
	
	void startGame();
	void stopGame();
	
	void updateShots(uint32_t elapsedMs);
	void renderShots();
	void spawnShot(vector2f pos, vector2f heading, float speed);
	
	enum {
		GameState_Welcome,
		GameState_Playing,
		GameState_Dead,
		GameState_Extinct,
		GameState_Overpopulation,
	} m_state;
	
	Camera*	m_cam;
	Man*	m_man;
	Shot	m_shots[kMaxShots];
	int		m_shotIndex;
	GLint	m_sparkle;
	
	vector2f m_shake;
	
	// file names defined at top of Game.cpp
	enum Sound { 
		Sound_Bling=0,
		Sound_Bison,
		Sound_Shoot,
		Sound_Hit,
		Sound_Pop,
		Sound_NumSounds
	};
	
	static const char m_soundfile[Sound_NumSounds][32];
	Mix_Chunk* m_sounds[Sound_NumSounds];
	
	Sprite2d* m_font;
	Sprite2d* m_buffalos;
	Sprite2d* m_titles;
	
	int32_t m_elapsed;
	
	int32_t m_killed;
	int32_t m_remaining;
};

#endif
