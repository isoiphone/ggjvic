#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

#include "main.h"
#include "Game.h"
#include "Gamepad.h"

int main(int argc, char *argv[])
{
	// init SDL
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK) < 0) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_WM_SetCaption(kWindowTitle, NULL);
	atexit(SDL_Quit);

	initVideo();
	initAudio();

	// init game
	Game* game = new Game();
	Gamepad* gamepad = new Gamepad();
	const Uint8* keystate = SDL_GetKeyState(NULL);

	// loop
	uint32_t elapsedTicks = 0;
	uint32_t lastFrame = SDL_GetTicks();

	// use fixed simulation steps so we can record replays or network sync
	while (1) {
		// SDL events
		SDL_PumpEvents();
		SDL_JoystickUpdate();
		if (keystate[SDLK_ESCAPE] || SDL_PeepEvents(NULL,0,SDL_PEEKEVENT,SDL_QUITMASK))
			break;

		// time
		uint32_t curTicks = SDL_GetTicks();
		elapsedTicks += curTicks-lastFrame;
		lastFrame = curTicks;

		while (elapsedTicks >= kMsPerStep) {
			gamepad->poll();
			game->update(kMsPerStep, gamepad);
			elapsedTicks -= kMsPerStep;
		}

		game->render();

		// play nice with OS
		SDL_Delay(0);
	}

	// destroy game
	delete game;
	delete gamepad;

	// destroy SDL and Mixer
	Mix_CloseAudio();
	SDL_Quit();

	return 0;
}

// set up OpenGL with ortho projection.
// (0,0) in top left, (kScreenWidth,kScreenHeight) in bottom right. 1 pixel = 1 unit.
void initVideo()
{
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
	if (!SDL_SetVideoMode(kScreenWidth, kScreenHeight, kScreenBpp, kScreenFlags)) { 
		fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
		exit(1);
	}

	// set up screen
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, kScreenWidth, kScreenHeight, 0.0);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f,0.0f,0.0f,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPointSize(kParticleSize);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	
//	GLfloat attenuation[] = {1.0f,0.0f,0.0f};
//	glPointParameterf(GL_POINT_SIZE_MIN, kMinPointSize);
//	glPointParameterf(GL_POINT_SIZE_MAX, kMaxPointSize);
//	glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, attenuation);
}


void initAudio()
{
	int audio_rate = 44100;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;

	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) == -1) {
		fprintf(stderr,"Unable to open audio\n");
		exit(1);
	}
}

#ifdef __APPLE__
// import core foundation, but don't pollute current namespace with generic names like Rect() etc.
namespace Apple {
	#include <CoreFoundation/CoreFoundation.h>
};
using Apple::CFBundleRef;
using Apple::CFStringRef;
using Apple::CFURLRef;
using Apple::CFBundleGetMainBundle;
using Apple::CFStringCreateWithCString;
using Apple::kCFStringEncodingUTF8;
using Apple::kCFStringEncodingUTF8;
#endif

// gets full path to the resource.
// on win32 this is just the filename, on OSX  this is the
// absolute system path to resource inside app bundle.
void get_bundle_path(const char* const filename, char* newpath, size_t maxlen)
{
	#ifdef __APPLE__
		CFBundleRef mainBundle = CFBundleGetMainBundle();
		CFStringRef cfFileName = CFStringCreateWithCString(NULL, filename, kCFStringEncodingUTF8);
		CFURLRef cfFileURL = CFBundleCopyResourceURL( mainBundle, cfFileName, NULL, NULL );
		CFURLGetFileSystemRepresentation(cfFileURL,true,(Uint8*)newpath, maxlen);
	#else
		strncpy(newpath, filename, maxlen);
	#endif
}

Mix_Chunk* loadSound(const char* const filename)
{
	char actualPath[1024];
	get_bundle_path(filename, actualPath, 1024);
	Mix_Chunk* snd = Mix_LoadWAV(actualPath);
	if (!snd)
		fprintf(stderr, "Unable to load sound: %s.\n", actualPath);
	return snd;
}

void unloadSound(Mix_Chunk** sound)
{
	Mix_FreeChunk(*sound);
	*sound = NULL;
}

void playSound(Mix_Chunk* sound, int channel/*=-1*/)
{
	if (Mix_PlayChannel(channel, sound, 0) == -1)
		fprintf(stderr,"failed to play sound. Reason: %s\n", Mix_GetError());
}

// based on post by Luke Philpot: http://www.gamedev.net/community/forums/topic.asp?topic_id=184477
GLuint loadTexture(const char* filename, uint16_t* width, uint16_t* height)
{
	char actualPath[1024];
	get_bundle_path(filename, actualPath, 1024);
	
    SDL_Surface *tex = IMG_Load(actualPath);

	if (!tex)
		return GL_INVALID_VALUE;

	GLuint texture;
	
	glEnable(GL_TEXTURE_2D);
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	if (tex->format->Amask)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->w, tex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->pixels);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->w, tex->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->pixels);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(tex);
	
	glDisable(GL_TEXTURE_2D);
	
	// return width/height if ptrs were passed
	if (width)
		*width = tex->w;
	if (height)
		*height = tex->h;
	
    return texture;
}


void unloadTexture(GLuint* texture)
{
	glDeleteTextures(1, texture);
	*texture = 0;
}
