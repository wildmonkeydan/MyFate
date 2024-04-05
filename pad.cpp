#include "pad.h"

#include <psxapi.h>
#include <stdio.h>

Pad::Pad() {
	// Init BIOS pad driver and set pad buffers (buffers are updated
	// automatically on every V-Blank)
	InitPAD((uint8_t*)&pad_buff[0][0], 34, (uint8_t*)&pad_buff[1][0], 34);

	// Start pad
	StartPAD();

	// Don't make pad driver acknowledge V-Blank IRQ (recommended)
	ChangeClearPAD(0);
}

bool Pad::IsButtonDown(PadButton btn) {
	PADTYPE* pad;
	pad = (PADTYPE*)&pad_buff[0][0];

	if (pad->stat == 0) {
		if ((pad->type == PadTypeID::PAD_ID_DIGITAL) || (pad->type == PadTypeID::PAD_ID_ANALOG_STICK) || (pad->type == PadTypeID::PAD_ID_ANALOG)) {
			return !(pad->btn & btn);
		}
	}

	return false;
}

DVECTOR Pad::GetStick(int stick) {
	PADTYPE* pad;
	pad = (PADTYPE*)&pad_buff[0][0];

	DVECTOR out = { 0 };

	if (pad->stat == 0) {
		if (pad->type == PadTypeID::PAD_ID_ANALOG || pad->type == PadTypeID::PAD_ID_ANALOG_STICK) {
			if (stick == LEFT_STICK) {
				if ((pad->ls_x - 128) > 16 || (pad->ls_x - 128) < -16) {
					out.vx = pad->ls_x - 128;
				}

				if ((pad->ls_y - 128) > 16 || (pad->ls_y - 128) < -16) {
					out.vy = pad->ls_y - 128;
				}
			}
			else {
				if ((pad->rs_x - 128) > 16 || (pad->rs_x - 128) < -16) {
					out.vx = pad->rs_x - 128;
				}
				if ((pad->rs_y - 128) > 16 || (pad->rs_y - 128) < -16) {
					out.vy = pad->rs_y - 128;
				}
			}
		}
	}

	return out;
}

bool Pad::IsFaceButtonDown() {
	return IsButtonDown(PAD_CIRCLE) || IsButtonDown(PAD_CROSS) || IsButtonDown(PAD_SQUARE) || IsButtonDown(PAD_TRIANGLE);
}