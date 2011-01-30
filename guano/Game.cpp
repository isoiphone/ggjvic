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

Game* Game::getInstance() {
	static Game inst;
	return &inst;
}

Game::Game()
{
    //linux hack
    #ifdef __linux
    chdir("/home/jeremy/ggjvic/guano");
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
	
	m_buffalos = new Sprite2d();
	m_buffalos->load("buffalos.png", 32, 32);

	m_sparkle = loadTexture("sparkle.png");

	buffInit();
	terrInit();
	
	startGame();
}


Game::~Game()
{
	stopGame();
	delete m_font;
}

void Game::startGame()
{
	m_elapsed = 0;
	buffReset();
	terrReset();
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

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,m_sparkle);

	const float kWidth=16.0f;

	for (int i=0; i<kMaxShots; ++i) {
		Shot& shot = m_shots[i];
		if (!shot.m_bActive)
			continue;

		float color[4] = {1,1,1,1};
		glColor4fv(color);
//		float color[3] = {genrand_real1()*0.65 + 0.35,genrand_real1()*0.65 + 0.35,genrand_real1()*0.65 + 0.35};
//		glColor3fv(color);

		glPushMatrix();
		glTranslatef(shot.m_pos.x, shot.m_pos.y, 0);
//		glScalef(2.0, 1.0, 0);
		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2d(1,1); glVertex3f(+kWidth,+kWidth,0); // Top Right
		glTexCoord2d(0,1); glVertex3f(-kWidth,+kWidth,0); // Top Left
		glTexCoord2d(1,0); glVertex3f(+kWidth,-kWidth,0); // Bottom Right
		glTexCoord2d(0,0); glVertex3f(-kWidth,-kWidth,0); // Bottom Left
		glEnd();
		glPopMatrix();
	}
	glDisable(GL_TEXTURE_2D);

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

void Game::spawnShot(vector2f pos, float heading, float speed) {
	Shot& shot = m_shots[m_shotIndex];
	shot.m_pos = pos;
	shot.m_vel = vectorFromHeading(heading, speed);
	shot.m_bActive = true;

	// advance shot index, so next shot fired uses next available shot
	m_shotIndex = (m_shotIndex+1)%kMaxShots;
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
	m_man->render();

	sprintf(buffer, "elapsed %d", m_elapsed/1000);
	glTranslatef(50,25,0);
	m_font->drawText(buffer);

	// RENDER HUD
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(50,25,0);
	glScalef(2, 2, 0);
		glTranslatef(0, 16, 0);

		sprintf(buffer, "buffalo");
		m_font->drawText(buffer);
	glPopMatrix();

	glFlush();
	SDL_GL_SwapBuffers();
}


void Game::update(uint32_t elapsedMs, Gamepad* gamepad)
{
	m_cam->update(elapsedMs, gamepad);
	m_man->update(elapsedMs, gamepad);

	if (gamepad->didPress(GAMEPAD_B)) {
		spawnShot(m_man->m_pos, m_man->m_rot, (m_man->m_speed*10.0)+kShotSpeed+(genrand_real1()*kShotSpeed*0.11));
	}

	buffUpdate(elapsedMs, gamepad);
	terrUpdate(elapsedMs, gamepad);
	updateShots(elapsedMs);

	// man moves, camera follows man
	vector2f dm = m_man->m_pos-m_cam->getCenter();
	m_cam->m_pos += dm * kCameraChaseSpeed;

	m_elapsed += elapsedMs;
}

vector2f Game::getCameraPos() {
	return m_cam->m_pos;
}

float Game::getCameraZoom() {
	return m_cam->m_zoom;
}
