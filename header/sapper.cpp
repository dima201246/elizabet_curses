#include "sapper.h"
#include "terminal_op.h"
#include "conf_lang/conf_lang.h"

#include <iostream>

// # include <sstream> // Для str()
// # include <ctime>
#ifdef WIN32
#include <sys/time.h>
#endif

using namespace std;

const string VERSION_GAME = "0.1 BeTa";

unsigned int MYC, MXC;

struct cell{
	bool mine, open, flag;
	int count_mines;
};

void view_map(cell **ground,
				unsigned int x_max,
				unsigned int y_max,
				unsigned int x_sel,
				unsigned int y_sel,
				unsigned int bombs,
				bool win,
				bool boom) {
	
	system(CLEAR_SCREEN);
	
	cout << bombs;

	if (boom) 
		cout << " X| (Press \"Backspace\" for restart)" << endl; // Esli user vzorvalsya
	else
		if (!win) {
			for (unsigned int i = 0; i < x_max/2-1; i++, printf(" "));
			printf(":)\n");
		}

	if (win) {
		system(CLEAR_SCREEN);
		cout << "8D (Press \"Backspace\" for restart)" << endl; // Esli user vzorvalsya
	}

	for (unsigned int y = 0; y < y_max; y++) {
		for (unsigned int x = 0; x < x_max; x++) {
			if ((x_sel == x) && (y_sel == y) && (!boom)) { // Ukazatel
				if (!ground[x][y].open) {
					if (ground[x][y].flag)
						printf("\033[31mX\033[0m");
					else
						printf("\033[34mX\033[0m");
				} else {
					if (ground[x][y].count_mines > 0)
						printf("\033[32mX\033[0m");
					else
						printf("\033[33mX\033[0m");
				}
				continue;
			}
			if (ground[x][y].open) {
				if (ground[x][y].count_mines > 0)
					printf("%d", ground[x][y].count_mines);
				else
					printf(" ");
			} else {
				if (boom) {
					if ((ground[x][y].mine) && (!ground[x][y].flag))
						printf("\033[31m*\033[0m");
					else {
						if ((ground[x][y].mine) && (ground[x][y].flag))
							printf("\033[32m*\033[0m");
						else
							printf("-");
					}
				} else {
					if (ground[x][y].flag)
						printf("\033[31m!\033[0m");
					else
						printf("#");
				}
			}
		}
		printf("\n");
	}
	return;
}

void signing_ground(cell **ground, unsigned int x_max, unsigned int y_max) {
	for (unsigned int y = 0; y < y_max; y++)
		for (unsigned int x = 0; x < x_max; x++) {
			ground[x][y].count_mines = 0;
			if (ground[x][y].mine) continue;
			for (unsigned int i = 1; i <= 8; i++) {
				switch (i) {
					case 1: if ((x > 0) && (y > 0) && (ground[x - 1][y - 1].mine)) ground[x][y].count_mines++; break;
					case 2: if ((x > 0) && (ground[x - 1][y].mine)) ground[x][y].count_mines++; break;
					case 3: if ((x > 0) && (y < y_max - 1) && (ground[x - 1][y + 1].mine)) ground[x][y].count_mines++; break;
					case 4: if ((y < y_max - 1) && (ground[x][y + 1].mine)) ground[x][y].count_mines++; break;
					case 5: if ((x < x_max - 1) && (y < y_max - 1) && (ground[x + 1][y + 1].mine)) ground[x][y].count_mines++; break;
					case 6: if ((x < x_max - 1) && (ground[x + 1][y].mine)) ground[x][y].count_mines++; break;
					case 7: if ((x < x_max - 1) && (y > 0) && (ground[x + 1][y - 1].mine)) ground[x][y].count_mines++; break;
					case 8: if ((y > 0) && (ground[x][y - 1].mine)) ground[x][y].count_mines++; break;
				}
			}
		}
	return;
}

bool open_cells(cell **ground, unsigned int x_max, unsigned int y_max, int& opened_cells) {
	bool reseach_non_compleate = false;
	for (unsigned int y = 0; y < y_max; y++) {
		for (unsigned int x = 0; x < x_max; x++)
			if ((ground[x][y].open) && (ground[x][y].count_mines == 0)) {
				if ((x != 0) && (!ground[x - 1][y].flag) && (!ground[x - 1][y].open) && (!ground[x - 1][y].mine)) {reseach_non_compleate = true; ground[x - 1][y].open = true; opened_cells++;}
				if ((x != 0) && (y != (y_max - 1)) && (!ground[x - 1][y + 1].flag) && (!ground[x - 1][y + 1].open) && (!ground[x - 1][y + 1].mine)) {reseach_non_compleate = true; ground[x - 1][y + 1].open = true; opened_cells++;}
				if ((y != (y_max - 1)) && (!ground[x][y + 1].flag) && (!ground[x][y + 1].open) && (!ground[x][y + 1].mine)) {reseach_non_compleate = true; ground[x][y + 1].open = true; opened_cells++;}
				if ((x != (x_max - 1)) && (y != (y_max - 1)) && (!ground[x + 1][y + 1].flag) && (!ground[x + 1][y + 1].open) && (!ground[x + 1][y + 1].mine)) {reseach_non_compleate = true; ground[x + 1][y + 1].open = true; opened_cells++;}
				if ((x != (x_max - 1)) && (!ground[x + 1][y].flag) && (!ground[x + 1][y].open) && (!ground[x + 1][y].mine)) {reseach_non_compleate = true; ground[x + 1][y].open = true; opened_cells++;}
				if ((x != (x_max - 1)) && (y != 0) && (!ground[x + 1][y - 1].flag) && (!ground[x + 1][y - 1].open) && (!ground[x + 1][y - 1].mine)) {reseach_non_compleate = true; ground[x + 1][y - 1].open = true; opened_cells++;}
				if ((y != 0) && (!ground[x][y - 1].flag) && (!ground[x][y - 1].open) && (!ground[x][y - 1].mine)) {reseach_non_compleate = true; ground[x][y - 1].open = true; opened_cells++;}
				if ((x != 0) && (y != 0) && (!ground[x - 1][y - 1].flag) && (!ground[x - 1][y - 1].open) && (!ground[x - 1][y - 1].mine)) {reseach_non_compleate = true; ground[x - 1][y - 1].open = true; opened_cells++;}
			}
	}
	if (reseach_non_compleate) return true;
	else return false;
}

bool open_surround(cell **ground, unsigned int x_max, unsigned int y_max, unsigned int x_sel, unsigned int y_sel, int& opened_cells) {
	if (!ground[x_sel][y_sel].open) return false;
	int mc = 0;
	bool boom = false;
	if ((x_sel != 0) && (ground[x_sel - 1][y_sel].flag) && (!ground[x_sel - 1][y_sel].open)) mc++;
	if ((x_sel != 0) && (y_sel != (y_max - 1)) && (ground[x_sel - 1][y_sel + 1].flag) && (!ground[x_sel - 1][y_sel + 1].open)) mc++;
	if ((y_sel != (y_max - 1)) && (ground[x_sel][y_sel + 1].flag) && (!ground[x_sel][y_sel + 1].open)) mc++;
	if ((x_sel != (x_max - 1)) && (y_sel != (y_max - 1)) && (ground[x_sel + 1][y_sel + 1].flag) && (!ground[x_sel + 1][y_sel + 1].open)) mc++;
	if ((x_sel != (x_max - 1)) && (ground[x_sel + 1][y_sel].flag) && (!ground[x_sel + 1][y_sel].open)) mc++;
	if ((x_sel != (x_max - 1)) && (y_sel != 0) && (ground[x_sel + 1][y_sel - 1].flag) && (!ground[x_sel + 1][y_sel - 1].open)) mc++;
	if ((y_sel != 0) && (ground[x_sel][y_sel - 1].flag) && (!ground[x_sel][y_sel - 1].open)) mc++;
	if ((x_sel != 0) && (y_sel != 0) && (ground[x_sel - 1][y_sel - 1].flag) && (!ground[x_sel - 1][y_sel - 1].open)) mc++;
	if (mc == ground[x_sel][y_sel].count_mines) {
		if ((x_sel != 0) && (!ground[x_sel - 1][y_sel].flag) && (!ground[x_sel - 1][y_sel].open)) {
			if (ground[x_sel - 1][y_sel].mine) boom = true; else {ground[x_sel - 1][y_sel].open = true; opened_cells++;}
		}
		if ((x_sel != 0) && (y_sel != (y_max - 1)) && (!ground[x_sel - 1][y_sel + 1].flag) && (!ground[x_sel - 1][y_sel + 1].open)) {
			if (ground[x_sel - 1][y_sel + 1].mine) boom = true; else {ground[x_sel - 1][y_sel + 1].open = true; opened_cells++;}
		}
		if ((y_sel != (y_max - 1)) && (!ground[x_sel][y_sel + 1].flag) && (!ground[x_sel][y_sel + 1].open)) {
			if (ground[x_sel][y_sel + 1].mine) boom = true; else {ground[x_sel][y_sel + 1].open = true; opened_cells++;}
		}
		if ((x_sel != (x_max - 1)) && (y_sel != (y_max - 1)) && (!ground[x_sel + 1][y_sel + 1].flag) && (!ground[x_sel + 1][y_sel + 1].open)) {
			if (ground[x_sel + 1][y_sel + 1].mine) boom = true; else {ground[x_sel + 1][y_sel + 1].open = true; opened_cells++;}
		}
		if ((x_sel != (x_max - 1)) && (!ground[x_sel + 1][y_sel].flag) && (!ground[x_sel + 1][y_sel].open)) {
			if (ground[x_sel + 1][y_sel].mine) boom = true; else {ground[x_sel + 1][y_sel].open = true; opened_cells++;}
		}
		if ((x_sel != (x_max - 1)) && (y_sel != 0) && (!ground[x_sel + 1][y_sel - 1].flag) && (!ground[x_sel + 1][y_sel - 1].open)) {
			if (ground[x_sel + 1][y_sel - 1].mine) boom = true; else {ground[x_sel + 1][y_sel - 1].open = true; opened_cells++;}
		}
		if ((y_sel != 0) && (!ground[x_sel][y_sel - 1].flag) && (!ground[x_sel][y_sel - 1].open)) {
			if (ground[x_sel][y_sel - 1].mine) boom = true; else {ground[x_sel][y_sel - 1].open = true; opened_cells++;}
		}
		if ((x_sel != 0) && (y_sel != 0) && (!ground[x_sel - 1][y_sel - 1].flag) && (!ground[x_sel - 1][y_sel - 1].open)) {
			if (ground[x_sel - 1][y_sel - 1].mine) boom = true; else {ground[x_sel - 1][y_sel - 1].open = true; opened_cells++;}
		}
		while (open_cells(ground, x_max, y_max, opened_cells));
	}
	if (boom) return true;
	else return false;
}

bool boom_state(cell **ground, unsigned int x_max, unsigned int y_max, unsigned int x_sel, unsigned int y_sel, int& opened_cells) {
	if (ground[x_sel][y_sel].open) return false;
	if (ground[x_sel][y_sel].mine) return true;
	else if (!ground[x_sel][y_sel].flag) {
		ground[x_sel][y_sel].open = true;
		opened_cells++;
		while (open_cells(ground, x_max, y_max, opened_cells));
	}
	return false;
}

bool game(unsigned int x_max, unsigned int y_max, unsigned int m_max) {
	system(CLEAR_SCREEN);
	cell **ground = new cell* [x_max];
	for (unsigned int x = 0; x < x_max; x++)
		ground[x] = new cell [y_max];
	unsigned int rand_x = x_max, rand_y = y_max, all_cells = x_max * y_max;
	for (unsigned int y = 0; y < y_max; y++)
		for (unsigned int x = 0; x < x_max; x++) {
			ground[x][y].mine = false;
			ground[x][y].flag = false;
			ground[x][y].open = false;
		}
	srand((unsigned)time(NULL));
	for (unsigned int m = 0; m < m_max; m++) {
		while (((rand_x == x_max) && (rand_y == y_max)) || (ground[rand_x][rand_y].mine)) {
			rand_x = rand() % x_max;
			rand_y = rand() % y_max;
		}
		ground[rand_x][rand_y].mine = true;
	}
	signing_ground(ground, x_max, y_max);
	int key_press = 0,
		count_flags = m_max,
		opened_cells = 0;

	unsigned int y_sel = 0,
				x_sel = 0,
				count_bomb = 0;

	bool work = true, boom = false, win = false;

	while (work) {
		if ((all_cells == m_max) || ((count_flags == 0) && (count_bomb == m_max))) win = true;
		view_map(ground, x_max, y_max, x_sel, y_sel, count_flags, win, boom);
		//mvprintw(0, 10, "flags: %i bombs: %i cells: %i", count_flags, count_bomb, all_cells);
		key_press = getch();
		opened_cells = 0;
		switch (key_press) {
			case KEY_UP: if ((boom) || (win)) break; if (y_sel > 0) y_sel--; break;
			case KEY_DOWN: if ((boom) || (win)) break; if (y_sel != y_max - 1) y_sel++; break;
			case KEY_LEFT: if ((boom) || (win)) break; if (x_sel > 0) x_sel--; break;
			case KEY_RIGHT: if ((boom) || (win)) break; if (x_sel != x_max - 1) x_sel++; break;
			case '\n': if ((boom) || (win)) break;
						boom = boom_state(ground, x_max, y_max, x_sel, y_sel, opened_cells);
						if (!boom) all_cells -= opened_cells;
						break;
			case 9: if ((boom) || (win)) break; // Tab
					if (!ground[x_sel][y_sel].open) {
						if (ground[x_sel][y_sel].flag) {
							ground[x_sel][y_sel].flag = false;
							count_flags++;
							if (ground[x_sel][y_sel].mine) count_bomb--;
						} else {
							ground[x_sel][y_sel].flag = true;
							count_flags--;
							if (ground[x_sel][y_sel].mine) count_bomb++;
						}
					} break;
			# ifdef WIN32
			case 8: if ((boom) || (win)) return true; /*backspace*/
			# else
			case 127: if ((boom) || (win)) return true; /*backspace*/
						boom = open_surround(ground, x_max, y_max, x_sel, y_sel, opened_cells);
						if (!boom) all_cells -= opened_cells;
						break;
			case 263: if ((boom) || (win)) return true; /*backspace*/
			# endif
						boom = open_surround(ground, x_max, y_max, x_sel, y_sel, opened_cells);
						if (!boom) all_cells -= opened_cells;
						break;
			// case 27: work = false; break;
		}
	}
	for (unsigned int x = 0; x < x_max; x++)
		delete [] ground[x];
	delete [] ground;
	return false;
}

void user_game(vector <string> lang_base) {
	int selected = 0,
		press_key,
		bombs = 5;
		
	unsigned int lines_game = 9,
				columns_game = 9;
	
	/*LANG ZONE START*/
	string my_settings = del_end(lang("my_settings", lang_base)),
			bombs_lang = del_end(lang("bombs", lang_base)),
			back = del_end(lang("back", lang_base)),
			lines_lang = del_end(lang("lines_lang", lang_base)),
			columns_lang = del_end(lang("columns_lang", lang_base)),
			start_game = del_end(lang("start_game", lang_base));
	/*LANG ZONE END*/

	while (true) {
		system(CLEAR_SCREEN);
		cout << my_settings << endl;
		if (selected == 0) cout << "> "; else cout << "  ";
		cout << bombs_lang << ": <"<< bombs << ">" << endl;
		if (selected == 1) cout << "> "; else cout << "  ";
		cout << lines_lang << ": <"<< lines_game << ">" << endl;
		if (selected == 2) cout << "> "; else cout << "  ";
		cout << columns_lang << ": <"<< columns_game << ">" << endl;
		if (selected == 3) cout << "> "; else cout << "  ";
		cout << start_game << endl;
		if (selected == 4) cout << "> "; else cout << "  ";
		cout << back << endl;
		press_key = getch();
		switch (press_key) {
			case KEY_UP: if (selected == 0) selected = 4;
						else selected--;
						break;
			case KEY_DOWN: if (selected == 4) selected = 0;
						else selected++;
						break;
			case KEY_RIGHT: switch (selected) {
								case 0: if (500 > bombs) bombs++; break;
								case 1: if ((MYC - 1) > lines_game) lines_game++; break;
								case 2: if (MXC > columns_game) columns_game++; break;
							} break;
			case KEY_LEFT: switch (selected) {
								case 0: if (1 < bombs) bombs--; break;
								case 1: if (9 < lines_game) lines_game--; break;
								case 2: if (9 < columns_game) columns_game--; break;
							} break;
			case '\n': switch (selected) {
							case 3: while (game(columns_game, lines_game, bombs)); break;
							case 4: return; break;
						} break;
			case KEY_BACKSPACE: return; break;
		}
	}
	return;
}

void init_game(vector <string> lang_base) {
	int selected = 0,
		key_press = 0;
	
	bool exit_stat = false;

	/*LANG ZONE START*/
	string my_settings = del_end(lang("my_settings", lang_base)),
			expert_game = del_end(lang("expert_game", lang_base)),
			medium_game = del_end(lang("medium_game", lang_base)),
			easy_game = del_end(lang("easy_game", lang_base)),
			select_level = del_end(lang("select_level", lang_base)),
			back = del_end(lang("back", lang_base));
	/*LANG ZONE END*/

	while (!exit_stat) {
		system(CLEAR_SCREEN);
		cout << select_level << endl;
		if (selected == 0) cout << "> "; else cout << "  ";
		cout << easy_game << endl;
		if (selected == 1) cout << "> "; else cout << "  ";
		cout << medium_game << endl;
		if (selected == 2) cout << "> "; else cout << "  ";
		cout << expert_game << endl;
		if (selected == 3) cout << "> "; else cout << "  ";
		cout << my_settings << endl;
		if (selected == 4) cout << "> "; else cout << "  ";
		cout << back << endl;
		key_press = getch();
		switch (key_press) {
			case KEY_UP: if (selected != 0) selected--;
						 else selected = 4;
						 break;
			case KEY_DOWN: if (selected != 4) selected++;
						 else selected = 0;
						 break;
			case '\n': switch (selected) {
							case 0: while (game(9, 9, 10)); break;
							case 1: while (game(16, 16, 40)); break;
							case 2: while (game(24, 20, 99)); break;
							// case 2: while (game(16, 30, 99)); break;
							case 3: user_game(lang_base); break;
							case 4: exit_stat = true; break;
						}
						break;
			case KEY_BACKSPACE: return;
								break;
		}
	}
	return;
}

void about_game(vector <string> lang_base) {
	system(CLEAR_SCREEN);
	cout << "Information" << endl << endl
		 << "Controls:" << endl
		 << "Arrows" << endl
		 << "Backspace - open a free cells" << endl
		 << "Tab - Put a mark on the cell" << endl
		 << "Enter - Select an item or open cells" << endl
		 << "ESC - Return to previous menu" << endl << endl
		 << "About game:" << endl
		 << "Sapper v."<< VERSION_GAME << endl
		 << ":DV company(c) 2016" << endl;
	cout << lang("press_any_key", lang_base) << endl;
	getch();
}

void sapper(vector <string> lang_base) {
	getmaxyx(stdscr, MYC, MXC);

	int selected = 0,
		key_press = 0;
	
	bool exit_stat = false;

	/*LANG ZONE START*/
	string start_game = del_end(lang("start_game", lang_base)),
			about_game_lang = del_end(lang("about_game", lang_base)),
			exit_lang = del_end(lang("case_exit", lang_base));
	/*LANG ZONE END*/

	while (!exit_stat) {
		system(CLEAR_SCREEN);
		// mvprintw(6, 0, "The current time is: %2d:%02d:%02d.%02d\n", t.ti_hour, t.ti_min, t.ti_sec, t.ti_hund);
		cout << "Sapper v." << VERSION_GAME << endl;
		if (selected == 0) cout << "> "; else cout << "  ";
		cout << start_game << endl;
		if (selected == 1) cout << "> "; else cout << "  ";
		cout << about_game_lang << endl;
		if (selected == 2) cout << "> "; else cout << "  ";
		cout << exit_lang << endl;
		key_press = getch();
		switch (key_press) {
			case KEY_UP: if (selected != 0) selected--;
						 else selected = 2;
						 break;
			case KEY_DOWN: if (selected != 2) selected++;
						 else selected = 0;
						 break;
			case '\n': switch (selected) {
							case 0: init_game(lang_base); break;
							case 1: about_game(lang_base); break;
							case 2: exit_stat = true; break;
						}
						break;
		}
	}
	return;
}