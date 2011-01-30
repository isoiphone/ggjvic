#include "main.h"
#include "Game.h"
#include "Gamepad.h"
#include "Sprite.h"
#include "Man.h"
#include "Camera.h"
#include "Buffalo.h"
#include "Terrain.h"

//linux hack
#ifdef __linux
#include <unistd.h>
#endif

// order / enums defined in Game.h
const char Game::m_soundfile[Sound_NumSounds][32] = {
	"bling.wav",
	"bison1.wav",
	"shoot.wav",
	"hit.wav",
	"pop.wav"
};

Game* Game::getInstance() {
	static Game inst;
	return &inst;
}

Game::Game() : m_shake(0,0)
{
    //linux hack
    #ifdef __linux
    chdir("../guano");
	#endif

	// init random
	init_genrand(time(NULL));

	m_man = new Man();
	m_cam = new Camera();

	memset(m_shots, 0, sizeof(m_shots));
	m_shotIndex = 0;

	// load resources
	m_font = new Sprite2d();
	m_font->load("visitor_16px.png", 16, 16);

    #ifdef __linux
    chdir("art");
    #endif

	m_buffalos = new Sprite2d();
	m_buffalos->load("buffalos.png", 32, 32);
	
	m_titles = new Sprite2d();
	m_titles->load("titles.png", 512, 512);

	#ifdef __linux
	chdir("sound");
	#endif
	
	for (int i=0; i<Sound_NumSounds; ++i) {
		m_sounds[i] = loadSound(m_soundfile[i]);
	}

	buffInit();
	terrInit();

	startGame();
}


Game::~Game()
{
	stopGame();
	delete m_font;
}

void Game::playBuffaloSound()
{
	playSound(m_sounds[Sound_Bison],-1);
}

void Game::playShootSound()
{
	playSound(m_sounds[Sound_Shoot],-1);
}

void Game::playHitSound()
{
	playSound(m_sounds[Sound_Hit],-1);
}

void Game::playPopSound()
{
	playSound(m_sounds[Sound_Pop],-1);
}

void Game::startGame()
{
	m_elapsed = 0;
	buffReset();
	terrReset();
	m_man->m_health = 100;
	
	m_shake = vector2f(0,0);
	m_killed = 0;
	m_remaining = 0;
	for (int i=0; i<kMaxBuffalo; ++i) {
		if (herd[i].bActive) 
			++m_remaining;
	}
	
	memset(m_shots, 0, sizeof(m_shots));
	m_shotIndex = 0;
	
	m_state = GameState_Welcome;
	m_man->m_pos = vector2f(kWorldWidth*32*0.5,kWorldHeight*32*0.5);
	m_cam->m_pos = vector2f(kWorldWidth*32*0.5-kScreenWidth*0.5,kWorldHeight*32*0.5-kScreenHeight*0.5);
	
}

void Game::stopGame()
{
}

void Game::updateShots(uint32_t elapsedMs) {
	for (int i=0; i<kMaxShots; ++i) {
		Shot& shot = m_shots[i];
		if (!shot.m_bActive)
			continue;

		shot.m_pos += (shot.m_vel*elapsedMs)*0.001;	// velocity is in units per second

		if (shot.m_pos.x < -5000 || shot.m_pos.x > 5000 ||
			shot.m_pos.y < -5000 || shot.m_pos.y > 5000)
			shot.m_bActive = false;

		// TODO: remove if it is too far away
	}
}

void Game::renderShots() {

//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D,m_sparkle);
//
//	const float kWidth=16.0f;
//
//	for (int i=0; i<kMaxShots; ++i) {
//		Shot& shot = m_shots[i];
//		if (!shot.m_bActive)
//			continue;
//
//		float color[4] = {1,1,1,1};
//		glColor4fv(color);
////		float color[3] = {genrand_real1()*0.65 + 0.35,genrand_real1()*0.65 + 0.35,genrand_real1()*0.65 + 0.35};
////		glColor3fv(color);
//
//		glPushMatrix();
//		glTranslatef(shot.m_pos.x, shot.m_pos.y, 0);
////		glScalef(2.0, 1.0, 0);
//		glBegin(GL_TRIANGLE_STRIP);
//		glTexCoord2d(1,1); glVertex3f(+kWidth,+kWidth,0); // Top Right
//		glTexCoord2d(0,1); glVertex3f(-kWidth,+kWidth,0); // Top Left
//		glTexCoord2d(1,0); glVertex3f(+kWidth,-kWidth,0); // Bottom Right
//		glTexCoord2d(0,0); glVertex3f(-kWidth,-kWidth,0); // Bottom Left
//		glEnd();
//		glPopMatrix();
//	}
//	glDisable(GL_TEXTURE_2D);
	
	for (int i=0; i<kMaxShots; ++i) {
		Shot& shot = m_shots[i];
		if (!shot.m_bActive)
			continue;
		
		glPushMatrix();
		glTranslatef(shot.m_pos.x+16, shot.m_pos.y+16, 0);
		float angle = headingFromVector(shot.m_vel);
		glRotatef(angle*RADIANS2DEGREES, 0, 0, 1);
		glTranslatef(-16, -16, 0);
		m_buffalos->draw(kSpearFrame);
		glPopMatrix();
	}
	

	/*
	glBegin(GL_POINTS);
	for (int i=0; i<kMaxShots; ++i) {
		Shot& shot = m_shots[i];
		if (!(shot.m_flags&SHOT_FLAGS_ACTIVE))
			continue;

//		float color[3] = {1,1,1};
		float color[3] = {genrand_real1()*0.65 + 0.35,genrand_real1()*0.65 + 0.35,genrand_real1()*0.65 + 0.35};

		glColor3fv(color);
		glVertex3f(shot.m_pos.x, shot.m_pos.y, 0);
	}
	glEnd();
	 */
}

void Game::spawnShot(vector2f pos, vector2f heading, float speed) {
	if (m_state == GameState_Welcome) {
		m_state = GameState_Playing;
		return;
	}
	
	Shot& shot = m_shots[m_shotIndex];
	shot.m_pos = pos;
	shot.m_vel = heading*speed;
	shot.m_bActive = true;

	// advance shot index, so next shot fired uses next available shot
	m_shotIndex = (m_shotIndex+1)%kMaxShots;
	
	//playSound(m_sounds[Sound_Bling], 0);
	playShootSound();
}



void Game::render()
{
	char buffer[64];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// translate based on camera pos
	glTranslatef((int)-m_cam->m_pos.x, (int)-m_cam->m_pos.y, 0);
	glScalef(m_cam->m_zoom, m_cam->m_zoom, 0);

	// RENDER GAME
	glPushMatrix();
	terrRender(m_buffalos, m_font);
	glPopMatrix();

	glPushMatrix();
	buffRender(m_buffalos);
	glPopMatrix();

	renderShots();
	m_man->render(m_buffalos);

//	sprintf(buffer, "elapsed %d", m_elapsed/1000);
//	glTranslatef(50,25,0);
//	m_font->drawText(buffer);

	// RENDER HUD
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(10,0,0);
		glTranslatef(0, 16, 0);
		sprintf(buffer, "killed %d", m_killed);
		m_font->drawText(buffer);
	
		glTranslatef(0, 16, 0);
		sprintf(buffer, "remaining %d", m_remaining);
		m_font->drawText(buffer);
	
		glTranslatef(0, 16, 0);
	
		float hpf = m_man->m_health/100.0;
		glScalef(144*hpf, 16, 1);
		glColor3f(0.8, 0.1, 0.1);
		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2d(1,1); glVertex3f(1,1,0); // Top Right
		glTexCoord2d(0,1); glVertex3f(0,1,0); // Top Left
		glTexCoord2d(1,0); glVertex3f(1,0,0); // Bottom Right
		glTexCoord2d(0,0); glVertex3f(0,0,0); // Bottom Left
		glEnd();
		glColor3f(1, 1, 1);
		glPopMatrix();
	
	glPopMatrix();
	
	
	
// TITLES!!! OMG OVERLAY EM ON THE SCREEN
	glLoadIdentity();
	glTranslatef((kScreenWidth-512)*0.5, (kScreenHeight-512)*0.5, 0);
	switch (m_state) {
		case GameState_Welcome:
			m_titles->draw(kTitle_TitleFrame);
			break;
		case GameState_Playing:
			// nada
			break;
		case GameState_Overpopulation:
			m_titles->draw(kTitle_OverpopFrame);
			break;
		case GameState_Extinct:
			m_titles->draw(kTitle_ExtinctFrame);
			break;
		case GameState_Dead:
			m_titles->draw(kTitle_DeadFrame);
			break;
	}

	glFlush();
	SDL_GL_SwapBuffers();
}

void Game::update(uint32_t elapsedMs, Gamepad* gamepad)
{	
	if(m_state != GameState_Welcome && m_state != GameState_Playing) {
		if(gamepad->didPress(GAMEPAD_X)) startGame();
		return;
	}
	
	m_cam->update(elapsedMs, gamepad);
	m_man->update(elapsedMs, gamepad);

	if (gamepad->didPress(GAMEPAD_X) && m_man->m_ready) {
		spawnShot(m_man->m_pos, m_man->m_aim, kShotSpeed+(genrand_real1()*kShotSpeed*0.11));
	}

	buffUpdate(elapsedMs, gamepad);
	terrUpdate(elapsedMs, gamepad);
	updateShots(elapsedMs);

	// man moves, camera follows man
	vector2f dm = m_man->m_pos-m_cam->getCenter();
	m_cam->m_pos += dm * kCameraChaseSpeed;

	m_cam->m_pos.x = MAX(0, MIN(kWorldWidth*32-kScreenWidth, m_cam->m_pos.x));
	m_cam->m_pos.y = MAX(0, MIN(kWorldHeight*32-kScreenHeight, m_cam->m_pos.y));

	m_cam->m_pos += m_shake;
	
	m_man->m_pos.x = MAX(0, MIN(kWorldWidth*32-32, m_man->m_pos.x));
	m_man->m_pos.y = MAX(0, MIN(kWorldHeight*32-32, m_man->m_pos.y));
	

	// shots kill stuff
	for (int i=0; i<kMaxShots; ++i) {
		if (!m_shots[i].m_bActive)
			continue;

		for (int buff=0; buff<kMaxBuffalo; ++buff) {
			if (!herd[buff].bActive)
				continue;

			vector2f dp = herd[buff].pos-m_shots[i].m_pos;
			if (dp.length() < herd[buff].rad*herd[buff].scale) {
				buffHit(buff);
				// break tha spear
				m_shots[i].m_bActive = false;
				
				// he gone done dieded
				if (!herd[buff].bActive)
					++m_killed;
			}
		}
	}

	// count the alive ones
	m_remaining = 0;
	for (int buff=0; buff<kMaxBuffalo; ++buff) {
		if (!herd[buff].bActive)
			continue;
		
		++m_remaining;
	}
	
	// win / lose
	if (m_remaining == 0) {
		m_state = GameState_Extinct;
	} else if (m_man->isDead()) {
		m_state = GameState_Dead;
	} else if (m_remaining == kMaxBuffalo) {
		m_state = GameState_Overpopulation;
	}
	m_elapsed += elapsedMs;
}

vector2f Game::getCameraPos() {
	return m_cam->m_pos;
}

vector2f Game::getPlayerPos() {
	return m_man->m_pos;
}

float Game::getCameraZoom() {
	return m_cam->m_zoom;
}

void Game::hitMan(int buffalo) {
	m_man->hit();
	m_shake.x = genrand_real1() * 2 - 1;
	m_shake.y = genrand_real1() * 2 - 1;
	m_shake *= 5;
}

