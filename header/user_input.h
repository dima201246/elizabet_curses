#ifndef USER_INPUT_H
#define USER_INPUT_H

	#include <string>
	#include <vector>
	#include <curses.h>
	
	std::string enter_us(int, unsigned int, unsigned int, std::vector <std::string>/*lang_base*/, std::vector <std::string>/*conf_base*/);
	std::string replace_act(std::string);
	void manual_example(bool, std::vector <std::string>/*lang_base*/, std::vector <std::string>/*conf_base*/);
#endif