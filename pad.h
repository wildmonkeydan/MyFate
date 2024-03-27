#pragma once
#include <psxpad.h>

class Pad {
public:
	Pad();
	bool IsButtonDown(PadButton btn);
private:
	char pad_buff[2][34];
};