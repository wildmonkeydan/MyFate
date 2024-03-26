#pragma once
#include <psxcd.h>
#include <psxspu.h>

#define CD_SECTOR_SIZE  2048
#define BtoS(len) ( ( len + CD_SECTOR_SIZE - 1 ) / CD_SECTOR_SIZE )

class CD {
public:
	void Init();
	uint32_t* LoadFile(const char* filename);
private:

};