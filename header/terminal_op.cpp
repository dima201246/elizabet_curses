#include "terminal_op.h"

#ifdef linux
	#include <iostream>
	#include "conf_lang/conf_lang.h"
	#include "extern_work.h"
#endif

/*
#ifdef linux
void get_term_size(unsigned int &maxX, unsigned int &maxY) {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	maxX = w.ws_col;
	maxY = w.ws_row;
}

#else

void get_term_size(unsigned int &maxX, unsigned int &maxY) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	maxX = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	maxY = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}
#endif
*/

#ifdef linux
int _getch() {
	struct termios oldt,
	newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

bool main_fm(std::vector <std::string> lang_base, std::vector <std::string> conf_base) {
	std::string fm_name = "0";
	do {
		erase();

		std::cout << lang("skip_fm", lang_base) << std::endl; 
		
		if (fm_name.empty())
			std::cout << lang("fm_not_found", lang_base) << std::endl;

		std::cout << del_end(lang("enter_fm", lang_base));
		std::cin >> fm_name;
		
		if (fm_name == "null") {
			getch();
			return false;
		}

		if (!FileExists("/usr/bin/" + fm_name))
			fm_name.clear();
		else
			configurator(CONF_FILE, "system_fm", fm_name, true);

	} while (fm_name.empty());
	getch();
	return true;
}
#endif


void init_curses() {
	initscr();
	start_color();
	keypad (stdscr, TRUE);
	noecho(); // Не выводить символы с клавиатуры
	curs_set(0);
	init_pair (TEXT_WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
	init_pair (TEXT_BLACK_WHITE, COLOR_BLACK, COLOR_WHITE);
	init_pair (TEXT_WHITE_YELLOW, COLOR_WHITE, COLOR_YELLOW);
	init_pair (TEXT_WHITE_BLUE, COLOR_WHITE, COLOR_BLUE);
	init_pair (TEXT_WHITE_GREEN, COLOR_WHITE, COLOR_GREEN);
	init_pair (TEXT_WHITE_RED, COLOR_WHITE, COLOR_RED);
	init_pair (TEXT_WHITE_MAGENTA, COLOR_WHITE, COLOR_MAGENTA);
	init_pair (TEXT_WHITE_CYAN, COLOR_WHITE, COLOR_CYAN);
	init_pair (TEXT_BLACK_YELLOW, COLOR_BLACK, COLOR_YELLOW);
	init_pair (TEXT_BLACK_BLUE, COLOR_BLACK, COLOR_BLUE);
	init_pair (TEXT_BLACK_GREEN, COLOR_BLACK, COLOR_GREEN);
	init_pair (TEXT_BLACK_RED, COLOR_BLACK, COLOR_RED);
	init_pair (TEXT_BLACK_MAGENTA, COLOR_BLACK, COLOR_MAGENTA);
	init_pair (TEXT_BLACK_CYAN, COLOR_BLACK, COLOR_CYAN);
	erase(); // Очистка экрана
	refresh();
}