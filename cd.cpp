#include "cd.h"
#include <stdio.h>
#include <stdlib.h>

void CD::Init() {
	CdInit();
	SpuInit();

	// Master volume should be in range 0x0000 - 0x3fff
	SpuSetCommonMasterVolume(0x3fff, 0x3fff);
	// Cd volume should be in range 0x0000 - 0x7fff
	SpuSetCommonCDVolume(0x2fff, 0x2fff);
	// Set transfer mode 
	SpuSetTransferMode(SPU_TRANSFER_BY_DMA);
}

uint32_t* CD::LoadFile(const char* filename) {
	CdlFILE	file;
	uint32_t* buffer;

	printf("\nReading file %s... ", filename);

	// Search for the file
	if (!CdSearchFile(&file, filename))
	{
		// Return value is NULL, file is not found
		printf("Not found!\n");
		buffer = NULL;
		return buffer;
	}

	// Allocate a buffer for the file
	buffer = (uint32_t*)malloc(2048 * ((file.size + 2047) / 2048));

	// Set seek target (seek actually happens on CdRead())
	CdControl(CdlSetloc, (unsigned char*)&file.pos, 0);

	// Read sectors
	CdRead((int)BtoS(file.size), (uint32_t*)buffer, CdlModeSpeed);

	// Wait until read has completed
	CdReadSync(0, 0);

	printf("Done.\n");

	return buffer;
}