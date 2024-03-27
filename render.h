#pragma once
#include <cstddef>
#include <cstdint>
#include <cassert>
#include <psxgpu.h>

static constexpr size_t DEFAULT_OT_LENGTH = 4094;
static constexpr size_t DEFAULT_BUFFER_LENGTH = 8192;

static constexpr int SCREEN_XRES = 320;
static constexpr int SCREEN_YRES = 240;

class RenderBuffer {
private:
	DISPENV _disp_env;
	DRAWENV _draw_env;

	std::uint32_t* _ot;
	std::uint8_t* _buffer;
	std::size_t   _ot_length, _buffer_length;

public:
	RenderBuffer(std::size_t ot_length, std::size_t buffer_length);
	~RenderBuffer(void);
	void setup(int x, int y, int w, int h, int r, int g, int b);

	inline uint8_t* buffer_start(void) const {
		return _buffer;
	}
	inline uint8_t* buffer_end(void) const {
		return &_buffer[_buffer_length];
	}
	inline uint32_t* ot_entry(int z) const {
		//assert((z >= 0) && (z < _ot_length));
		return &_ot[z];
	}

	inline void clear_ot(void) {
		ClearOTagR(_ot, _ot_length);
	}
	inline void draw(void) {
		DrawOTagEnv(&_ot[_ot_length - 1], &_draw_env);
	}
	inline void display(void) const {
		PutDispEnv(&_disp_env);
	}
};

class RenderContext {
private:
	RenderBuffer _buffers[2];
	std::uint8_t* _next_packet;
	int          _active_buffer;

	// These functions are simply shorthands for _buffers[_active_buffer] and
	// _buffers[_active_buffer ^ 1] respectively. They are only used internally.
	inline RenderBuffer& _draw_buffer(void) {
		return _buffers[_active_buffer];
	}
	inline RenderBuffer& _disp_buffer(void) {
		return _buffers[_active_buffer ^ 1];
	}

public:
	RenderContext(
		std::size_t ot_length = DEFAULT_OT_LENGTH,
		std::size_t buffer_length = DEFAULT_BUFFER_LENGTH
	);
	void setup(int w, int h, int r, int g, int b);
	void flip(void);

	// This is a "factory function" that allocates a new primitive within the
	// currently active buffer. It is a template method, meaning T will get
	// replaced at compile time by the type of the primitive we are going to
	// allocate (and sizeof(T) will change accordingly!).
	template<typename T> inline T* new_primitive(int z = 0) {
		// Place the primitive after all previously allocated primitives, then
		// insert it into the OT and bump the allocation pointer.
		auto prim = reinterpret_cast<T*>(_next_packet);

		addPrim(_draw_buffer().ot_entry(z), prim);
		_next_packet += sizeof(T);

		// Make sure we haven't yet run out of space for future primitives.
		assert(_next_packet <= _draw_buffer().buffer_end());

		return prim;
	}

	// A simple helper for drawing text using PSn00bSDK's debug font API. Note
	// that FntSort() requires the debug font texture to be uploaded to VRAM
	// beforehand by calling FntLoad().
	inline void draw_text(int x, int y, int z, const char* text) {
		_next_packet = reinterpret_cast<uint8_t*>(
			FntSort(_draw_buffer().ot_entry(z), _next_packet, x, y, text)
			);

		assert(_next_packet <= _draw_buffer().buffer_end());
	}
};
