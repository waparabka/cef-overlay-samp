#ifndef SAMP_HPP
#define SAMP_HPP

#include <RakHook/samp.hpp>
#include <RakHook/detail.hpp>


class c_chat {

public:
	c_chat*& reference() {
		return *(c_chat**)rakhook::samp_addr(0x21A0E4);
	}
	
	void add_message(long color, const char* text) {
		((void(__thiscall*)(c_chat*, long, const char*))rakhook::samp_addr(0x645A0))(this, color, text);
	}
};


class c_game {

public:
	c_game*& reference() {
		return *(c_game**)rakhook::samp_addr(0x21A10C);
	}

	void set_cursor_mode(int mode, BOOL hide) {
		((void(__thiscall*)(c_game*, int, BOOL))rakhook::samp_addr(0x9BD30))(this, mode, hide);
	}
};


#endif // SAMP_HPP