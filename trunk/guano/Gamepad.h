#ifndef _GAMEPAD_H_INCLUDED
#define _GAMEPAD_H_INCLUDED

#include "main.h"

#define GAMEPAD_UP		(1<<0)
#define GAMEPAD_DOWN	(1<<1)
#define GAMEPAD_LEFT	(1<<2)
#define GAMEPAD_RIGHT	(1<<3)
#define GAMEPAD_A		(1<<4)
#define GAMEPAD_B		(1<<5)
#define GAMEPAD_X		(1<<6)
#define GAMEPAD_Y		(1<<7)

// encapsulates user input. Works like a generic 4 button, 4 direction digital gamepad.
class Gamepad {
public:
	Gamepad();
	virtual ~Gamepad();

	void poll();

	int didPress(uint8_t button);
	int isHeld(uint8_t button);
	int didRelease(uint8_t button);

	int getX1() { return x1; }
	int getY1() { return y1; }
	int getX2() { return x2; }
	int getY2() { return y2; }
	
	// Semi-protected methods.
	// Dont use em unless you know full well what your doing :P
	void _setButton(uint8_t button, bool on=true);
	void _setState(uint8_t newState);
	uint8_t _getState();

private:
	int x1,y1, x2, y2;
	uint8_t state;
	uint8_t change;
	Uint8* key;
	SDL_Joystick *joy;
};


#endif
