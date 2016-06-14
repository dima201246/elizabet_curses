/*Планы:
	- Меню истории результатов
*/

/*ОШИБКИ:
	- Зависает при вводе быстрых оппераций
*/

#ifdef WIN32
	/*Local headers*/
	#include "header\long_calc\long_calc.h"
	#include "header\conf_lang\conf_lang.h"
	#include "header\win_lib.h"
	#include "header\user_input.h"
	#include "header\calc_block.h"
	#include "header\terminal_op.h"
	#include "header\angry_debug.h"
	#include "header\long_calc.h"
	#include "header\extern_work.h"
	#include "header\sapper.h"
#else
	/*Local headers*/
	#include "header/conf_lang/conf_lang.h"
	#include "header/calc_block/calc_block.h"
	#include "header/user_input.h"
	#include "header/extern_work.h"
	#include "header/terminal_op.h"
	#include "header/angry_debug.h"
	#include "header/sapper.h"
#endif

using namespace std;

vector <string> lang_base;
vector <string> conf_base;

#ifdef linux
void star() {cout << "NULL" << endl;}
#endif

void system_pause() {
	printw("%s", lang("press_any_key", lang_base).c_str());
	getch();
}

void hello_logo() {
	erase();
	printw("%s %s\n%s", del_end(lang("hello_logo_lang", lang_base)).c_str(), _version_el, del_end(lang("hello_logo_lang1", lang_base)).c_str());
	system_pause();
}

void version() {
	erase();
	printw("Elizabet version: %s ", _version_el);

	#ifdef linux
		printw("(linux) x%s", arch_l);
	#endif

	#ifdef WIN32
		printw("(windows) x%s", arch_l);
	#endif

	printw(" | Long Arithmetic version: %s\n", long_calc("", "", 0, true).c_str());
	printw("Date build: %s %s\n\n", __TIME__, __DATE__);
	printw("+++++++++++++++++++++++++++++++++++++\n");
	printw("+ For contact: dima201246@gmail.com +\n");
	printw("+     Web site: elizabet-calc.tk    +\n");
	printw("+++++++++++++++++++++++++++++++++++++\n");
	printw("*************************************************\n");
	printw("*                Special thks to:               *\n");
	printw("*        Mariya Quesada (help with bugs)        *\n");
	printw("* Artem Kazakov, Georgiy Efimov (bugs searcher) *\n");
	printw("*         Andrey Arhipov (idea for code)        *\n");
	printw("*************************************************\n");
	printw("---------------------\n");
	printw("| :DV company 2016  |\n");
	printw("| Dmitriy Volchenko |\n");
	printw("---------------------\n");
	system_pause();
}

void help() {
	erase();
	cout << lang("hot_keys", lang_base) << endl;
	if (conf("recor_results", conf_base) == "1") cout << "\"&\" -" << lang("&_key_press", lang_base) << endl;
		 //<< lang("key_press") << "\"e\" -" << lang("e_key_press") << endl;
	//	cout << lang("key_press") << "\"h\" -" << lang("i_key_press") << endl
	cout << lang("hot_keys_znach", lang_base) << endl;
	system_pause();
}

void input_sample() {
	string im_end_r, enter;
	int us_en;
	do {
		enter = enter_us(0, 0, 0, lang_base, conf_base); // visov vvoda primera
		if (enter == "exit")
			return; // eslivozvrashen exit

		if ((conf("recor_results", conf_base) == "1") && (!enter.empty()))
			add_to_file(conf("out_res_file", conf_base), "primer = " + enter); // esli razreshena zapis - zapisat primer

		string end_r = calculate_ex(enter);

		erase();
		printw("%s %s\n", del_end(lang("end_result", lang_base)).c_str(), end_r.c_str()); // vivod resultata
		printw("%s\n", del_end(lang("any_action", lang_base)).c_str()); // vopros o eshe odnomdeystvii
		im_end_r = end_r;

		if ((conf("recor_results", conf_base) == "1") && (!im_end_r.empty())) {
			add_to_file(conf("out_res_file", conf_base), "result = " + im_end_r); // zapis resultata
		}

		printw("TAB - %s\n", del_end(lang("rounding", lang_base)).c_str()); // vopros o okruglenii

		us_en = getch();

		if (us_en == KEY_TAB) {
			string temp_array[1];
			unsigned int rounding_num;

			temp_array[0] = im_end_r;

			erase();

			do {
				printw("%s", del_end(lang("rounding_num", lang_base)).c_str());
				refresh();

				echo();
				scanw("%d", &rounding_num);
				noecho();

				if (1 > rounding_num) {
					erase();
					printw("%s\n", del_end(lang("rounding_num_m_null", lang_base)).c_str());
				}

				if (8 < rounding_num) {
					erase();
					printw("%s\n", del_end(lang("rounding_num_b_null", lang_base)).c_str());
				}

			} while ((1>rounding_num) || (8 < rounding_num));
				
			erase();
				
			rounding(temp_array, 1, rounding_num);
				
			printw("%s %s\n", del_end(lang("end_result", lang_base)).c_str(), temp_array[0].c_str()); // vivod resultata
			printw("%s\n", del_end(lang("any_action", lang_base)).c_str()); // vopros o eshe odnom deystvii
			us_en = getch();
		}
	} while (us_en == KEY_ENTER_LOCAL); // poka nado shto-to vipolnyat
}

void menu_new() {
	bool		cycle;

	char		selected,
				game;

	int			key_presed;
	
	cycle		= true;
	key_presed	= 0;
	selected	= 0;
	game		= 0;

	string		path_fm	= conf("system_fm", conf_base);
	
	if ((path_fm != "null") && (path_fm != "none"))
		path_fm +=  " " + conf("path_works_files", conf_base);
	
	/*LANG ZONE START*/
	string	menu_lg					= lang("menu", lang_base),
			case_enter_sample		= lang("case_enter_sample", lang_base),
			case_work_with_formul	= lang("case_work_with_formul", lang_base),
			help_lang				= lang("help", lang_base),
			version_prog			= lang("version_prog", lang_base),
			case_exit				= lang("case_exit", lang_base),
			work_with_formul		= lang("work_with_formul", lang_base),
			enter_formula			= lang("enter_formula", lang_base),
			open_work_fol			= lang("open_work_fol", lang_base),
			work_with_files			= lang("work_with_files", lang_base),
			back					= lang("back", lang_base);
	/*LANG ZONE END*/

	if (conf("game", conf_base) == "1")
		game	= 1;

	while (true) {
		cycle	= true;
		while (cycle) {
			erase();

			if ((conf("debug", conf_base) == "1") || (conf("angry_debug", conf_base) == "1"))
				mvprintw(1, 0, "***DEBUGING MODE***");
			
			mvprintw(0, 0, "%s", menu_lg.c_str());

			mvprintw(selected + 2, 0, ">");

			mvprintw(2, 2, "%s", case_enter_sample.c_str());
			mvprintw(3, 2, "%s", case_work_with_formul.c_str());
			mvprintw(4, 2, "%s", help_lang.c_str());
			mvprintw(5, 2, "%s", version_prog.c_str());
			mvprintw(6, 2, "Change language");
			mvprintw(7, 2, "%s", case_exit.c_str());
			
			if (game == 1) {
				mvprintw(2, 7, "Sapper");
			}

			key_presed = getch();

			switch (key_presed) {
				case KEY_ESC: erase();
							  printw("%s \n", lang("for_exit_press_enter", lang_base).c_str());
							  if (getch() == KEY_ENTER_LOCAL) {
								  erase();
								  printw("%s\n", lang("bye_bye", lang_base).c_str());
								  return;
							  }
							  break;

				case KEY_UP: if (selected != 0) selected--;
							 else selected = (5 + game);
							 break;

				case KEY_DOWN: if (selected != (5 + game)) selected++;
							 else selected = 0;
							 break;

				case KEY_ENTER_LOCAL: switch (selected) {
									case 0:	input_sample();
											break;

									case 1:	cycle = false;
											break;
								
									case 2:	help();
											break;

									case 3:	version();
											break;

									case 4:	if (!load_to_vector("./langs/eng.lang", lang_base)) {
												printw("Not found lang file!!!\n");
												exit(-2);
											} else {
												menu_new();
												return;
											}
											break;

									case 5:	erase();
											printw("%s\n", lang("bye_bye", lang_base).c_str());
											return;
											break;

									case 6:	sapper(lang_base);
											break;
								}
								break;

				case '1': selected = 0;
						input_sample();
						break;

				case '2': selected = 1;
						cycle = false;
						break;

				case '3': selected = 2;
						help();
						break;

				case '4': selected = 3;
						version();
						break;

				case '5': selected = 4;
						if (!load_to_vector("./langs/eng.lang", lang_base)) {
							printw("Not found lang file!!!\n");
							exit(-2);
						} else {
							menu_new();
							return;
						}
						break;

				case '6': erase();
						printw("%s\n", lang("bye_bye", lang_base).c_str());
						return;
						break;
			}
		}

		cycle = true;
		selected = 0;
		while (cycle) {
			erase();

			if ((conf("debug", conf_base) == "1") || (conf("angry_debug", conf_base) == "1"))
				mvprintw(1, 0, "***DEBUGING MODE***");
			
			mvprintw(0, 0, "%s", work_with_formul.c_str());

			mvprintw(selected + 2, 0, ">");

			mvprintw(2, 2, "%s", enter_formula.c_str());
			mvprintw(3, 2, "%s", open_work_fol.c_str());
			mvprintw(4, 2, "%s", work_with_files.c_str());
			mvprintw(5, 2, "%s", back.c_str());

			key_presed = getch();

			switch (key_presed) {
				case KEY_UP:		if (selected != 0) {
										selected--;
									} else {
										selected = 3;
									}
									break;

				case KEY_DOWN:		if (selected != 3) {
										selected++;
									} else {
										selected = 0;
									}
									break;

				case KEY_ENTER_LOCAL:	switch (selected) {
											case 0: manual_example(false, lang_base, conf_base);
													break;

											case 1: if ((path_fm == "null") || (path_fm == "none")) {
														erase();
														printw("%s\n", lang("not_fm", lang_base).c_str());
														system_pause();
													} else {
														system(path_fm.c_str());
													}
													break;

											case 2: if (extern_work(conf("path_works_files", conf_base), lang_base, conf_base) == -1) {
														printw("EERROORR\n");
														system_pause();
													}
													break;

											case 3: selected = 1;
													cycle = false;
													break;
										}
										break;

				case KEY_BACKSPACE_LOCAL:	selected = 1;
											cycle = false;
											break;

				case KEY_ESC:		selected = 1;
									cycle = false;
									break;

				case '1':			manual_example(false, lang_base, conf_base);
									break;

				case '2':			if ((path_fm == "null") || (path_fm == "none")) {
										erase();
										printw("%s\n", lang("not_fm", lang_base).c_str());
										system_pause();
									} else
										system(path_fm.c_str());
									break;
								
				case '3':			if (extern_work(conf("path_works_files", conf_base), lang_base, conf_base) == -1) {
										printw("EERROORR\n");
										system_pause();
									}
									break;

				case '4':			selected = 1;
									cycle = false;
									break;
			}
		}
	}
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "");

	if (!FileExists("temp_files"))
		if (!CreatFoldren("temp_files")) {
			printf("Can't creat temp_files directory!!!\n");
			printf("Press any key...\n");
			_getch();
			return -28;
		}

	if (!FileExists("work"))
		if (!CreatFoldren("work")) {
			printf("Can't creat work directory!!!\n");
			printf("Press any key...\n");
			_getch();
			return -27;
		}

	if (!load_to_vector(CONF_FILE, conf_base)) {
		printf("Not config file!!!\n");
		printf("Press any key...\n");
		_getch();
		return -1;
	}

	if (!load_to_vector("./langs/" + conf("language", conf_base) + ".lang", lang_base)) {
		printf("Not found lang file!!!\n");
		printf("Press any key...\n");
		_getch();
		return -2;
	}

	if (argc > 1) {
		printf("%s\n", del_end(lang("please_wait", lang_base)).c_str());
		string end_r = calculate_ex((const char*)argv[1]);
		// for (unsigned int i = 0; i < w_length(lang("please_wait", lang_base)); i++, printf("\b"));
		printf("%s = %s\n", argv[1], end_r.c_str());
		// if (w_length(lang("please_wait", lang_base)) > (strlen(argv[1]) + 3 + end_r.length()))
			// for (unsigned int i = 0; i < w_length(lang("please_wait", lang_base)) - (strlen(argv[1]) + 3 + end_r.length()); i++, printf(" "));
		// printf("\n");
		return 0;
	}

	#ifdef WIN32
		start_dir(lang_base);
	#else
		if (conf("system_fm", conf_base) == "0x1") {
			printf("Oooops... Not found system_fm in configuration file!\n");
			system_pause();
			return -7;
		}

		if ((conf("system_fm", conf_base) == "null") && (main_fm(lang_base, conf_base)))
			if (!load_to_vector(CONF_FILE, conf_base)) {
				printf("Not config file!!!\n");
				return -1;
			}

	#endif

	init_curses();

	if (conf("first_start", conf_base) == "1") {
		hello_logo();
		erase();
		configurator(CONF_FILE, "first_start", "0", true);
	}

	if (conf("recor_results", conf_base) == "1") {
		
		string linkTT = conf("out_res_file", conf_base);
		
		if ((fopen(linkTT.c_str(), "r")) == NULL) {
			ofstream t(linkTT.c_str());
			t.close();
		}
	}

	menu_new();
	endwin();
	
	return 0;
}