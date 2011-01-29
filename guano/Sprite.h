#ifndef _SPRITE_H_INCLUDED
#define _SPRITE_H_INCLUDED

#include "main.h"

class Sprite2d {
public:
	Sprite2d();
	virtual ~Sprite2d();
	bool load(const char* filename, uint16_t frameWidth, uint16_t frameHeight);
	void draw(uint8_t frame);
	void drawText(const char* str);

	GLuint texture;
	uint16_t textureWidth, textureHeight;
	uint16_t frameWidth, frameHeight;
	uint16_t framesPerRow, maxFrames;
};

#endif
