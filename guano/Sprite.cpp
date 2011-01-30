#include "Sprite.h"

Sprite2d::Sprite2d()
{
	texture = 0;
	textureWidth = textureHeight = 0;
	frameWidth = frameHeight = 0;
	framesPerRow = maxFrames = 0;
}

Sprite2d::~Sprite2d()
{
	if (texture)
		unloadTexture(&texture);
}

bool Sprite2d::load(const char* filename, uint16_t _frameWidth, uint16_t _frameHeight)
{
	texture = loadTexture(filename, &textureWidth, &textureHeight);
	if (!texture)
		return false;

	assert(_frameWidth > 0 &&
		   _frameHeight > 0 &&
		   _frameWidth <= textureWidth &&
		   _frameHeight <= textureHeight);

	frameWidth = _frameWidth;
	frameHeight = _frameHeight;
	framesPerRow = textureWidth/_frameWidth;
	maxFrames = framesPerRow * (textureHeight/frameHeight);

	return true;
}


void Sprite2d::draw(uint8_t frame)
{
	// texture not loaded
	if (!texture) {
		assert(false);
		return;
	}

	// invalid frame
	if (frame >= maxFrames) {
		assert(false);
		return;
	}

	const GLfloat width = frameWidth/(float)textureWidth;
	const GLfloat height = frameHeight/(float)textureHeight;
	const uint16_t col = frame % framesPerRow;
	const uint16_t row = frame / framesPerRow;

	const GLshort spriteVertices[] = {
		0, 0,
		frameWidth, 0,
		0, frameHeight,
		frameWidth, frameHeight,
	};

	const GLfloat textureCoords[] = {
		width*col, height*row,
		width*(col+1), height*row,
		width*col, height*(row+1),
		width*(col+1), height*(row+1),
	};

//	glColor3ub(0xFF, 0xFF, 0xFF);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glVertexPointer(2, GL_SHORT, 0, spriteVertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, textureCoords);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
}


void Sprite2d::drawText(const char* str)
{
	glPushMatrix();

	const char* c = str;

	while (*c != '\0')
	{
		if (*c >= 'a' && *c <= 'z')
			draw(*c - 'a');
		else if (*c >= 'A' && *c <= 'Z')
			draw(*c - 'A');
		else if (*c >= '0' && *c <= '9')
			draw(26 + *c-'0');
		else if (*c == '.')
			draw(39);
		else if (*c == '!')
			draw(41);
		else if (*c == ':')
			draw(46);
		else if (*c == '*')
			draw(47);

		glTranslatef(frameWidth, 0, 0);
		c++;
	}

	glPopMatrix();
}
