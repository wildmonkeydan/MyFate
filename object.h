#pragma once
#include <psxgte.h>

struct ObjectData {
	SVECTOR position;
	unsigned char meshId;
};

class Object {
public:

private:
	ObjectData* data;
};