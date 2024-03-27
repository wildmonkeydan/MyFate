#include "pad.h"

#include <psxapi.h>

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