#pragma once
#include "render.h"

class Background {
public:
	enum class Type {
		Void,
		Sky,
		Vapour
	};

	Background();
	void Update();
	void Draw(RenderContext& ctx);
	void SwitchType(Type swit);
private:
	Type current;
};