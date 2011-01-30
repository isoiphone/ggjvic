#include "Gamepad.h"

Gamepad::Gamepad() : state(0), change(0)
{
	SDL_ShowCursor(false);
	
	key = SDL_GetKeyState(NULL);
	joy = NULL;

	// find and use the first joystick with enough buttons / axis'
	for (int i=0; i<SDL_NumJoysticks(); ++i) {
		joy = SDL_JoystickOpen(i);

		if (joy) {
			if (SDL_JoystickNumAxes(joy) >= 2 && SDL_JoystickNumButtons(joy) >= 4) {
				printf("Using Joystick for input.\n");
				printf("Name: %s\n", SDL_JoystickName(i));
				printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
				printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
				printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
				
				if (SDL_JoystickNumAxes(joy) == 2) {
					break;
				} else {
					SDL_JoystickClose(joy);
					joy = NULL;
				}
			} else {
				SDL_JoystickClose(joy);
				joy = NULL;
			}
		}
	}
}

Gamepad::~Gamepad() 
{
	SDL_ShowCursor(true);

	if (joy)
		SDL_JoystickClose(joy);
}

void Gamepad::poll()
{
	// use joystick input when possible
	if (joy) {
		static const int deadzone = 5000;
		static const int buttmap[4][2] ={	{0, GAMEPAD_A},
											{1, GAMEPAD_B},
											{5, GAMEPAD_X},
											{3, GAMEPAD_Y}	};
		
		x1 = SDL_JoystickGetAxis(joy, 0);
		y1 = SDL_JoystickGetAxis(joy, 1);
		
		x2 = SDL_JoystickGetAxis(joy, 2);
		y2 = SDL_JoystickGetAxis(joy, 3);


		Sint16 axis_x = SDL_JoystickGetAxis(joy, 0);
		Sint16 axis_y = SDL_JoystickGetAxis(joy, 1);

		if (axis_y < -deadzone) {
			_setButton(GAMEPAD_UP);
			_setButton(GAMEPAD_DOWN, false);
		} else if (axis_y < deadzone) {
			_setButton(GAMEPAD_UP, false);
			_setButton(GAMEPAD_DOWN, false);
		} else {
			_setButton(GAMEPAD_UP, false);
			_setButton(GAMEPAD_DOWN, true);
		}

		if (axis_x < -deadzone) {
			_setButton(GAMEPAD_LEFT);
			_setButton(GAMEPAD_RIGHT, false);
		} else if (axis_x < deadzone) {
			_setButton(GAMEPAD_LEFT, false);
			_setButton(GAMEPAD_RIGHT, false);
		} else {
			_setButton(GAMEPAD_LEFT, false);
			_setButton(GAMEPAD_RIGHT, true);
		}

		for (int i=0; i<4; ++i) {
			if (SDL_JoystickGetButton(joy, buttmap[i][0]))
				_setButton(buttmap[i][1]);
			else
				_setButton(buttmap[i][1], false);
		}
	// fall back onto keyboard input
	} else {
//		static const int map[8][2] = {	{SDLK_UP,		GAMEPAD_UP},
//										{SDLK_DOWN,		GAMEPAD_DOWN}, 
//										{SDLK_LEFT,		GAMEPAD_LEFT},
//										{SDLK_RIGHT,	GAMEPAD_RIGHT},
//										{SDLK_LCTRL,	GAMEPAD_A},
//										{SDLK_SPACE,	GAMEPAD_B},
//										{SDLK_LSHIFT,	GAMEPAD_X},
//										{SDLK_LALT,		GAMEPAD_Y}	};
		
	// left analog stick == arrow keys
		if (key[SDLK_LEFT] || key[SDLK_a]) {
			x1 = -32000;
		} else if (key[SDLK_RIGHT] || key[SDLK_d]) {
			x1 = +32000;
		} else {
			x1 = 0;
		}
		
		if (key[SDLK_UP] || key[SDLK_w]) {
			y1 = -32000;
		} else if (key[SDLK_DOWN] || key[SDLK_s]) {
			y1 = +32000;
		} else {
			y1 = 0;
		}
		
	// right analog stick == mouse
		int mx=0,my=0;
		uint8_t mb = SDL_GetMouseState(&mx, &my);
		int dx = (mx - kScreenWidth*0.5);
		int dy = (my - kScreenHeight*0.5);
		
		x2 = dx*200;
		y2 = dy*200;
		
				
	// space or left mouse == X
		if (key[SDLK_SPACE] || (mb&SDL_BUTTON_LEFT)) {
			_setButton(GAMEPAD_X);
		} else {
			_setButton(GAMEPAD_X, false);
		}
		
		
		
//		// We assume that SDL_PumpEvents() is called elsewhere.
//		// This is required to update the key array returned by SDL_GetKeyState()
//		for (int i=0; i<8; ++i) {
//			if (key[map[i][0]])
//				_setButton(map[i][1]);
//			else
//				_setButton(map[i][1], false);
//		}
	}
}

int Gamepad::isHeld(uint8_t button)
{
	return (state & button) != 0;
}

int Gamepad::didPress(uint8_t button)
{
	return ((state & button) != 0 && (change & button) != 0);
}

int Gamepad::didRelease(uint8_t button)
{
	return ((state & button) == 0 && (change & button) != 0);
}

void Gamepad::_setButton(uint8_t button, bool on/*=true*/)
{
	// If we are supposed to set the button on, and its not on...
	if (on && !(state & button)) {
		state |= button;
		change |= button;
	}
	// If we are supposed to set the button off, and it is on...
	else if (!on && (state & button)) {
		state &= ~button;
		change |= button;
	}
	// Button has not changed, make sure the 'changed' bit is off
	else
		change &= ~button;
}

void Gamepad::_setState(uint8_t newState)
{
	change = newState ^ state;
	state = newState;
}

uint8_t Gamepad::_getState()
{
	return state;
}
