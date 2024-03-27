#pragma once
#include <psxpad.h>
#include <psxgte.h>

#define LEFT_STICK 0
#define RIGHT_STICK 1

class Pad {
public:
	Pad();
	bool IsButtonDown(PadButton btn);
	DVECTOR GetStick(int stick);
private:
	char pad_buff[2][34];
};