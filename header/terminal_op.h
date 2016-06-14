#ifndef TERMINAL_OP_H
#define TERMINAL_OP_H

	#include <stdio.h>
	#include <curses.h>
	
	/*Colors*/
	#define TEXT_WHITE_BLACK	0
	#define TEXT_WHITE_YELLOW	1
	#define TEXT_WHITE_BLUE		2
	#define TEXT_WHITE_GREEN	3
	#define TEXT_WHITE_RED		4
	#define TEXT_WHITE_MAGENTA	5
	#define TEXT_WHITE_CYAN		6

	#define TEXT_BLACK_WHITE	7
	#define TEXT_BLACK_YELLOW	8
	#define TEXT_BLACK_BLUE		9
	#define TEXT_BLACK_GREEN	10
	#define TEXT_BLACK_RED		11
	#define TEXT_BLACK_MAGENTA	12
	#define TEXT_BLACK_CYAN		13

	/*Keys*/
	#define KEY_ESC 27
	#define KEY_SPACE 32
	#define KEY_DELETE 330
	#define KEY_TAB 9
	#define KEY_ESC 27
	#define KEY_ENTER_LOCAL '\n'
	#define KEY_BACKSPACE_LOCAL 127
	#define KEY_CTRL_LEFT 547
	#define KEY_CTRL_RIGHT 562
	
	#ifdef WIN32
		#include <windows.h>
		#include <conio.h>
		
		#define CLEAR_SCREEN "cls"
		#define CONF_FILE "./configs/PinkiePie.conf"

	#else
		// #include <sys/ioctl.h>
		#include <unistd.h>
		#include <termios.h>

		#define CLEAR_SCREEN "clear"
		#define CONF_FILE "./configs/PinkiePie_linux.conf"

	#endif

	// void get_term_size(unsigned int &maxX, unsigned int &maxY);
	void init_curses();
	#ifdef linux
		#include <vector>
		#include <string>
		bool main_fm(std::vector <std::string>, std::vector <std::string>);
		int _getch();
	#endif
#endif