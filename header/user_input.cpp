#include "conf_lang/conf_lang.h"
#include "calc_block/calc_block.h"

#include "user_input.h"
#include "extern_work.h"
#include "terminal_op.h"
#include "last_result.h"

using namespace std;

const int max_chars = 1000;

void system_pause_uin(vector <string> lang_base) {
	cout << lang("press_any_key", lang_base) << endl;
	getch();
}

bool test_op(string line, vector <string> lang_base) { // proverka operaciy
	string temp;
	bool read_stat = false;
	int l_l = line.length();
	for (int k = 0; k <= l_l; k++) {
		if (((int)line[k] >= 97) && ((int)line[k] <= 122)) {
			read_stat = true;
			temp += line[k];
		} else if (read_stat == true) {
			if ((!temp.empty()) && (replace_act(temp + "(") == "0")) {
				system(CLEAR_SCREEN);
				cout << lang("unknown_op", lang_base) << temp << "\"" << endl;
				system_pause_uin(lang_base);
				temp.clear();
				return false;
			}
			temp.clear();
		}
	}
	return true;
}

string replace_act(string banka) {
	if (banka == "cos(")
		return "ctg(";
	if (banka == "ctg(")
		return "cos(";
	if (banka == "ln(")
		return "log(";
	if (banka == "log(")
		return "ln(";
	if (banka == "sin(")
		return "sqrt(";
	if (banka == "sqrt(")
		return "sin(";
	if (banka == "tg(")
		return "tg(";
	if (banka == "exp(")
		return "exp(";
	if (banka == "pi(")
		return "pi(";
	if (banka == "arcctg(")
		return "arctg(";
	if (banka == "arctg(")
		return "arccos(";
	if (banka == "arccos(")
		return "arcsin(";
	if (banka == "arcsin(")
		return "arcsec(";
	if (banka == "arcsec(")
		return "arccosec(";
	if (banka == "arccosec(")
		return "arcctg";
	if (banka == "e(")
		return "e(";
	return "0";
}

void fast_op_left(string* fast_op, int* fast_op_int, string example_inter, int coord_op, int left_s) { // Ïåðåìåùåíèå ìàññèâîâ ñ êîîðäèíàòàìè è íàçâàíèåì îïåðàöèè âëåâî
	for (int i = coord_op; i <= del_bl(example_inter); i++) { // Öèêë ðàáîòàåò íà óâèëè÷åíèå, äàáû íå ïåðåìåñòèòü óæ ïåðåìåù¸ííûå ïåðåìåííûå
		if(fast_op_int[i] != 0) { // Åñëè íàéäåíà êîîðäèíàòà îïåðàöèè, òî å¸ ïåðåìåùàþò
			fast_op_int[i - left_s] = fast_op_int[i];
			fast_op[i + fast_op_int[i] - left_s].clear();
			fast_op[i + fast_op_int[i] - left_s] = fast_op[i + fast_op_int[i]];
			fast_op[i + fast_op_int[i]].clear();
			fast_op_int[i] = 0;
		}
	}
}

void fast_op_right(string* fast_op, int* fast_op_int, string example_inter, int coord_op, int right_s) { // Ïåðåìåùåíèå ìàññèâîâ ñ êîîðäèíàòàìè è íàçâàíèåì îïåðàöèè âïðàâî
	for (int i = del_bl(example_inter); i >= coord_op; i--) { // Öèêë ðàáîòàåò íà óìåíüøåíèå, äàáû íå ïåðåìåñòèòü óæ ïåðåìåù¸ííûå ïåðåìåííûå
		if(fast_op_int[i] != 0) { // Åñëè íàéäåíà êîîðäèíàòà îïåðàöèè, òî å¸ ïåðåìåùàþò
			fast_op_int[i + right_s] = fast_op_int[i];
			fast_op[i + fast_op_int[i] + right_s].clear();
			fast_op[i + fast_op_int[i] + right_s] = fast_op[i + fast_op_int[i]];
			fast_op[i + fast_op_int[i]].clear();
			fast_op_int[i] = 0;
		}
	}
}

string del_primer(string* fast_op, int* fast_op_int, string example_inter, int coord_op) { // Óäàëåíèå ñèìâîëà èëè îïåðàöèè èç ïåðåìåííîé
	if (fast_op[coord_op].empty()) { //
		fast_op_left(fast_op, fast_op_int, example_inter, coord_op, 1);
		example_inter.erase((coord_op - 1), 1);
	} else {
		int length_banka = fast_op[coord_op].length();
		example_inter.erase((coord_op - length_banka), fast_op[coord_op].length());
		fast_op_int[coord_op - length_banka] = 0;
		fast_op[coord_op].clear();
		fast_op_left(fast_op, fast_op_int, example_inter, coord_op, length_banka);
	}
	return example_inter;
}

/*INPUT BLOCK START*/
string fast_operations[] = {"cos", "ctg", "sin", "sqrt", "ln", "log", "tg", "exp", "pi", "arcctg", "arctg", "arccos", "arcsin", "arcsec", "arccosec", "e"};

struct free_space {
	unsigned int	max_len,
					start_pos,
					end_pos; 
};

string list_of_ops(char fast_key, unsigned int pos, unsigned int MaxX, unsigned int MaxY) { // Вывод окошка быстрых операций
	unsigned int	max_len;

	string			temp_line;

	vector <string>	temp_vec;

	max_len				= 0;

	temp_vec.clear();

	for (unsigned int i	= 0; !fast_operations[i].empty(); i++)
			if ((fast_operations[i][0] == fast_key) || (fast_key == '0')) {
				temp_vec.insert(temp_vec.end(), fast_operations[i]); // Заполнение вектора нужными элементами
				
				if (max_len < fast_operations[i].length()) // Поиск самой большой длины
					max_len = fast_operations[i].length();
			}

	if ((max_len + pos + 2) > MaxX) { // Если вылезло за экран
		pos = MaxX - max_len - 2; // Попытка вставить в экран
		if (pos > MaxX) // Если не вышло, то выход
			return "0";
	}

	bool			cycle;

	unsigned int	selected,
					top_shift, // Границы вывода списка
					bottom_shift;

	int				key;

	cycle			= true;
	key				= 0;
	selected		= 0;
	top_shift		= 0;
	bottom_shift 	= 0;

	if (MaxY < temp_vec.size()) {
		bottom_shift	= MaxY;
	} else {
		bottom_shift	= temp_vec.size();
	}

	while (cycle) {

		for (unsigned int i	= 0; i < bottom_shift; i++) {
			if (MaxY < temp_vec.size())
				for (unsigned int j	= 0; j < max_len; j++, mvprintw(MaxY - i, pos + j, "  "));
			mvprintw(MaxY - i, pos, "  %s", temp_vec[i + top_shift].c_str()); // Вывод элементов
		}

		mvprintw(MaxY - (selected - top_shift), pos, ">");

		if (MaxY < temp_vec.size()) {
			mvprintw(MaxY - (selected - top_shift), pos, ">");
		}

		key		= getch();

		switch (key) {

			case KEY_UP:			if (selected != (temp_vec.size() - 1)) {
										if ((MaxY < temp_vec.size()) && (((MaxY - (selected - top_shift)) - 1) == 0)) {
											top_shift++;
										}
										selected++;
									} else {
										top_shift	= 0;
										selected	= 0;
									}
									break;

			case KEY_DOWN:			if (selected != 0) {
										if ((MaxY < temp_vec.size()) && (selected == top_shift)) {
											top_shift--;
										}
										selected--;
									} else {
										selected	= temp_vec.size() - 1;
										top_shift	= temp_vec.size() - bottom_shift;
									}
									break;

			case KEY_ENTER_LOCAL:	if ((temp_vec[selected] == "pi") || (temp_vec[selected] == "e"))
										return temp_vec[selected];
									else
										return temp_vec[selected] + "(";
									break;

			default:				return "0";
									break;
		}
	}
	return "0";
}

bool fast_ops(char key) { // Проверка ли существует быстрая операция на заданную букву
	for (int i = 0; !fast_operations[i].empty(); i++)
		 if (fast_operations[i][0] == key)
		 	return true;
	return false;
}

string write_line(string line /*сама строка*/, unsigned int position /*позиция указателя*/, free_space line_space /*Количество свободных колон*/) {

	line.insert(position, "|");

	if ((line_space.max_len < line.length()) && (line_space.start_pos < position))
		line.erase(0, line_space.start_pos);

	if ((line_space.max_len < line.length()) && (line_space.start_pos >= position))
		line.erase(0, line_space.start_pos);
	
	if ((line.length() > (line_space.end_pos - line_space.start_pos + 1)) || ((line_space.start_pos > 0) && (position == line_space.start_pos))) {
		if (position == line_space.end_pos)
			line.erase(0, 1);
		line.erase(line.length() - (line.length() - line_space.max_len), line.length());
		line += "&";
	}

	if (line_space.start_pos > 0) {
		if (position == line_space.start_pos) {
			line.erase(line.length() - 2, 1);
		} else {
			line.erase(0, 1);
		}
		line.insert(0, "&");

	}
	
	return line;
}
string enter_us(int param, unsigned int count, unsigned int vals, vector <string> lang_base, vector <string> conf_base) {

	string			line,
					key_str,
					search_line, // Для поиска места указателя на экране
					fo, // Для вставки быстрой операции
					end_r, // Переменная для результата выражения
					last_input_temp; // Переменная для сравнения результатов выражения (чтобы не пересчитывать несколько раз)

	line.clear();

	int				key = 0,
					stdrt = 1; // Стандартный сдвиг указателя

	free_space		line_space;

	unsigned int	position = 0, // Позиция указателя
					first_word = 8, // Длина первого слова, исправить, когда будет подключен lang_out модуль
					MaxX,
					MaxY,
					pos_down = 0; // Позиция окошка с быстрыми операциями

	bool			cycle;

	getmaxyx(stdscr, MaxY, MaxX);

	line_space.max_len		= MaxX - first_word - 1; // Максимально возможное место под вывод ввода
	line_space.start_pos	= 0; //Начало обрезки
	line_space.end_pos		= line_space.max_len; // Конец обрезки

	/*LANG ZONE START*/
	string	input_example	= del_end(lang("primer", lang_base)),
			result_lng		= del_end(lang("result", lang_base));
	/*LANG ZONE END*/

	cycle		= true;
	position	= 0;
	first_word	= w_length(input_example);
	pos_down	= 0;
	end_r.clear();
	last_input_temp.clear();

	while (cycle) {
		
		erase();

		if (last_input_temp != line) {
			end_r	= calculate_ex(line);
			last_input_temp	= line;
		}
		mvprintw(0, 0, "Key: %d", (int)key);
		mvprintw(MaxY - 2, 0, "%s%s", result_lng.c_str(), end_r.c_str());
		mvprintw(MaxY - 1, 0, "%s%s", input_example.c_str(), write_line(line, position, line_space).c_str());

		key = getch();

		switch (key) {

			case KEY_RIGHT: if (position != line.length()) {

								if ((line[position] >= 'a') && (line[position] <= 'z')) {
									unsigned int j = position, count = 0;
									bool resech = true;

									while ((j < line.length()) && (resech)) {
										if ((line[j] < 'a') || (line[j] > 'z')) {
											position = j;
											if (line[j] == '(')
												position++;
											resech = false;
										}
										count++;
										j++;
									}

									if (resech)
										position = line.length();
									
									if ((!resech) && (position > line_space.end_pos)) {
										line_space.start_pos += count;
										line_space.end_pos += count;
									}

									if ((resech) && (position > line_space.end_pos)) {
										line_space.start_pos += line.length() - line_space.max_len;
										line_space.end_pos += line.length();
									}

									break;
								}

								if (position >= line_space.end_pos) {
									line_space.start_pos++;
									line_space.end_pos++;
								}
								position++;
							}
							break;

			case KEY_LEFT: if (position != 0) {
								
								if (((position > 2) && (line[position-1] == '(') && ((line[position-2] >= 'a') && (line[position - 2] <= 'z'))) || ((line[position - 1] >= 'a') && (line[position - 1] <= 'z'))) { // Проскок быстрой подстановки
									unsigned int j, count = 0;
									bool resech = true;

									if ((line[position - 1] >= 'a') && (line[position - 1] <= 'z')) // Авось там быстрая операция из одной буквы!
										j = position - 1;
									else
										j = (position - 2);

									while ((j != 0) && (resech)) {
										if ((line[j] < 'a') || (line[j] > 'z')) { // Поиск начала быстрой операции
											position = j + 1;
											resech = false;
										}
										count++;
										j--;
									}
									
									if (resech)
										position = 0;
									
									if ((!resech) && (position < line_space.start_pos)) {
										line_space.start_pos -= line_space.start_pos - position;
										line_space.end_pos -= count; // Магическая подгонка под экран
									}

									if ((resech) && (position < line_space.start_pos)) {
										line_space.start_pos = 0;
										line_space.end_pos = line_space.max_len;
									}

									break;
								}



								if (position <= line_space.start_pos) {
									line_space.start_pos--;
									line_space.end_pos--;
								}
								position--;
							}
							break;

			case KEY_DOWN: search_line = write_line(line, position, line_space);

							for (pos_down = 0; pos_down < search_line.length(); pos_down++)
								if(search_line[pos_down] == '|') // Поиск указателя, чтобы под ним вывести список
									break;

							fo = list_of_ops('0', pos_down + first_word, MaxX, MaxY - 2);
							if (fo != "0") {
								if ((position != 0) && (((line[position - 1] >= '0') && ('9' >= line[position - 1])) || (line[position - 1] == ')'))) { // При каких условиях подставлять умножение
									line.insert(position, "*");
									position++;
								}
								
								line.insert(position, fo);
								position += fo.length();

								if ((line[position - 1] != '(') && (position != line.length())) { // Добавление умножения, если быстрая операция не содержала открытой скобки
									line.insert(position, "*");
									position++;
								}

								if (position > line_space.end_pos) {
									line_space.start_pos += position - line_space.end_pos;
									line_space.end_pos += position - line_space.end_pos;
								}

							}
							break;

			case KEY_CTRL_LEFT: if (position != 0) {
									
									bool resech = true;
									
									if ((position > 2) && (line[position - 1] == '(') && ((line[position - 2] >= 'a') && (line[position - 2] <= 'z'))) {
										if (position <= line_space.start_pos) {
											line_space.start_pos--;
											line_space.end_pos--;
										}
										position--;
									}

									if (((line[position - 1] >= '0') && (line[position - 1] <= '9')) || ((line[position - 1] >= 'a') && (line[position - 1] <= 'z'))) { // Прыжки через скопление цифр
										position--;
										while ((position > 0) && (resech)) { // Поиск не цифры или не буквы
											if (((line[position] >= '0') && (line[position] <= '9')) || ((line[position] >= 'a') && (line[position] <= 'z'))) {
												if (position <= line_space.start_pos) {
													line_space.start_pos--;
													line_space.end_pos--;
												}
												position--;
											} else {
												position++; // Шаг вперед, так как нашло не цифру
												resech = false; // Остановка цикла
											}
										}
										if (!resech)
											break;
									}
									
									if (position < line_space.start_pos) { // Если не через чего перескакивать, то пропуск одного элемента
										line_space.start_pos--;
										line_space.end_pos--;
									}
									if (position != 0)
										position--;
								}
								break;

			case KEY_CTRL_RIGHT: if (position < line.length()) {
									
									bool resech = true;

									if (((line[position] >= '0') && (line[position] <= '9')) || ((line[position] >= 'a') && (line[position] <= 'z'))) { // Прыжки через скопление цифр

										while ((position < line.length()) && (resech)) { // Поиск не цифры или не буквы
											
											if (((line[position] >= '0') && (line[position] <= '9')) || ((line[position] >= 'a') && (line[position] <= 'z'))) {
												if (position >= line_space.end_pos) {
													line_space.start_pos++;
													line_space.end_pos++;
												}
												position++;
											} else {
												resech = false; // Остановка цикла
											}
										}
										break;
									}

									if (position >= line_space.end_pos) {
										line_space.start_pos++;
										line_space.end_pos++;
									}
									position++;
								}
								break;

			case KEY_BACKSPACE_LOCAL: if (position == 0)
										break;
									
									if (((position > 2) && (line[position - 2] >= 'a') && (line[position - 2] <= 'z')) && ((line[position] >= 'a') && (line[position] <= 'z'))) {
										erase();
										printw("I'm sorry, but I can not delete the operation if there are no other operations");
										getch();
										break;
									}

									if (((position > 2) && (line[position-1] == '(') && ((line[position-2] >= 'a') && (line[position - 2] <= 'z'))) || ((line[position - 1] >= 'a') && (line[position - 1] <= 'z'))) { // Проскок быстрой подстановки
										unsigned int j, count = 0;
										bool resech = true;

										if ((line[position - 1] >= 'a') && (line[position - 1] <= 'z')) // Авось там быстрая операция из одной буквы!
											j = position;
										else {
											line.erase(position - 1, 1);
											j = position - 1;
										}

										while ((j != 0) && (resech)) {
											j--;
											if ((line[j] < 'a') || (line[j] > 'z')) { // Поиск начала быстрой операции
												position = j + 1;
												resech = false;
											} else
												line.erase(j, 1);
											count++;
										}

										if (resech)
											position = 0;
									
										if ((!resech) && (position < line_space.start_pos)) {
											line_space.start_pos -= line_space.start_pos - position;
											line_space.end_pos -= count; // Магическая подгонка под экран
										}
										if ((resech) && (position < line_space.start_pos)) {
											line_space.start_pos = 0;
											line_space.end_pos = line_space.max_len;
										}

										break;
									}

									if (line_space.end_pos > line_space.max_len) {
										line_space.end_pos--;
										line_space.start_pos--;
									}
									line.erase(position - 1, 1);
									position--;
									break;

			case KEY_DELETE: if (position == line.length()) 
								break;
							if (position != line.length()) {
								if ((line[position] >= 'a') && (line[position] <= 'z')) {

									while (((line[position] >= 'a') && (line[position] <= 'z')) || (line[position] == '(')) {
										line.erase(position, 1);

										if (line_space.end_pos > line_space.max_len) {
											line_space.end_pos--;
											line_space.start_pos--;
										}
									}
									break;
								}
							}
							if (line_space.end_pos > line_space.max_len) {
								line_space.end_pos--;
								line_space.start_pos--;
							}
							line.erase(position, 1);
							break;

			case KEY_ESC:	cycle = false;
							return "exit";
							break;

			case KEY_ENTER_LOCAL:	cycle	= false;
									break;

			default:if (((key >= '0') && (key <= '9')) || ((key == '+') || (key == '(') || (key == ')') || (key == '-') || (key == '/') || (key == '*') || (key == '^') || (key == '!') || (key == ',') || (key == '.') || (key == '|'))) { // Какие знаки можно вводить
						stdrt	= 1;
						key_str	= (char)key;
						line.insert(position, key_str);

						if ((position != 0) && (line[position - 1] >= 'a') && (line[position - 1] <= 'z') && (((key >= '0') && (key <= '9')) || (key == '('))) { // Добавление умножения, если перед вводимым знаком буква
							line.insert(position, "*");
							stdrt	= 2;
						}

						if ((position != 0) && (line[position - 1] == ')') && (((key >= '0') && (key <= '9')) || (key == '('))) { // Добавление умножения, если перед цифрой или открытой скобкой закрытая скобка
							line.insert(position, "*");
							stdrt	= 2;
						}

						if ((position != 0) && (line[position - 1] >= '0') && (line[position - 1] <= '9') && (key == '(')) { // Добавление умножения, если перед открытой скобкой цифра
							line.insert(position, "*");
							stdrt	= 2;
						}

						position += stdrt;
						if (line.length() > line_space.max_len) {
							line_space.start_pos += stdrt;
							line_space.end_pos += stdrt;
						}
						break;
					}

					if (fast_ops(key)) { // Быстрая подстановка
						search_line = write_line(line, position, line_space);

						for (pos_down = 0; pos_down < search_line.length(); pos_down++)
							if(search_line[pos_down] == '|') // Поиск указателя, чтобы под ним вывести список
								break;

						fo = list_of_ops((char)key, pos_down + first_word, MaxX, MaxY - 2);
						if (fo != "0") {
							if ((position != 0) && (((line[position - 1] >= '0') && ('9' >= line[position - 1])) || (line[position - 1] == ')') || ((line[position - 1] >= 'a') && (line[position - 1] <= 'z')))) { // При каких условиях подставлять умножение
								line.insert(position, "*");
								position++;
							}
								
							line.insert(position, fo);
							position += fo.length();

							if ((line[position - 1] != '(') && (position != line.length())) { // Добавление умножения, если быстрая операция не содержала открытой скобки
								line.insert(position, "*");
								position++;
							}

							if (position > line_space.end_pos) {
								line_space.start_pos += position - line_space.end_pos;
								line_space.end_pos += position - line_space.end_pos;
							}
						}
					}
					break;
		}
	}
	return line;
}
/*INPUT BLOCK END*/

void manual_example(bool add_file/*если запущено с обязательным сохранением файла*/, vector <string> lang_base, vector <string> conf_base) { // ruchnoi vvod formuli ПЕРЕДЕЛАТЬ!
	unsigned int vals;
	string main_sample_work, full_path, temp;
	bool created_new_file = false;
	
	do {
		main_sample_work = enter_us(1, 0, 0, lang_base, conf_base); // vvod formuli
		if (main_sample_work == "exit") return;
		vals = count_val(main_sample_work, lang_base, conf_base); // skolko peremennih
		if (main_sample_work.empty()) { // esli vozvrat pustoy
			system(CLEAR_SCREEN);
			cout << lang("formula_can_not_be_empty", lang_base) << endl;
			system_pause_uin(lang_base);
		}
	} while ((vals == 0) || (test_op(main_sample_work, lang_base) == false) || (main_sample_work.empty())); // poka net peremennih, provalen test na izvestie operacii, vvod pust - povtoryat
	
	if (conf("recor_results", conf_base) == "1")
		add_to_file(conf("out_res_file", conf_base), "formula = " + main_sample_work); // zapis formuli
	
	string start_path; // Переменная для всего
	char key;
	
	if (!add_file) {
		system(CLEAR_SCREEN);
		cout << lang("write_in_formul_prof", lang_base) << endl;
		if (getch() == KEY_ENTER)
			add_file = true;
	}

	if (add_file) {
		start_path = conf("path_works_files", conf_base);
		if (start_path == "0x1") {
			cout << lang("crit_er_par", lang_base) << endl
				 << "Err_0x1_path_works_files Modul_user_input" << endl;
			system_pause_uin(lang_base);
			return;
		}
		do {
			system(CLEAR_SCREEN);
			cout << del_end(lang("please_input_file_name", lang_base));
			cin >> temp;
			
			#ifdef linux
			getch();
			#endif
			
			if (fopen((start_path + temp + ".eli").c_str(), "r") != NULL) {
				system(CLEAR_SCREEN);
				cout << lang("warning_found_file", lang_base) << endl;
				key = getch();
				if (key == KEY_ESC)
					return;
				else if (key == KEY_TAB) {
						if (remove((start_path + temp + ".eli").c_str()) == -1) {
							cout << "ERROR!" << endl << "Can't delete file..." << endl;
							system_pause_uin(lang_base);
							return;
						}
						break;
					}
			} else
				break;
		} while (true);
		full_path = start_path + temp + ".eli";

		add_to_file(full_path, "primer = " + main_sample_work);

		system(CLEAR_SCREEN);
		cout << lang("creat_comment", lang_base) << endl;
			
		if (getch() == KEY_ENTER) {
			system(CLEAR_SCREEN);
			cout << del_end(lang("enter_comment", lang_base));
			cin >> temp;
			add_to_file(full_path, "comment = " + temp);
		#ifdef linux
			getch();
		#endif
		}
		created_new_file = true;
		system(CLEAR_SCREEN);
	}
	
	vector <string> values_vec;
	values_vec.clear();

	bool cycle = true;
	unsigned int count = 0;

	do {
		
		temp = enter_us(1, count + 1, vals, lang_base, conf_base);	
		
		if (temp == "exit")
			return;
		
		if (created_new_file)
			add_to_file(full_path, "values = " + temp);
		
		if (temp[temp.length() - 1] == 'p') {
			temp.erase(temp.length() - 2, 2);
		} else
			cycle = false;

		count++;
		values_vec.insert(values_vec.end(), temp);

	} while (cycle);

	string *vals_mass = new string [values_vec.size()];
	for (unsigned int i = 0; i < values_vec.size(); i++)
		vals_mass[i] = values_vec[i];

	string pi_num = conf("pi_number", conf_base);
	if (add_file) {
		system(CLEAR_SCREEN);
		cout << lang("calc_formula", lang_base) << endl;
		if (getch() == KEY_ENTER) {
			string* results = calculation_formula(main_sample_work, vals_mass, count, pi_num);
			menu_results(results, count, main_sample_work, vals_mass, lang_base, conf_base);
			delete [] results;
		}
	} else {
		string* results = calculation_formula(main_sample_work, vals_mass, count, pi_num);
		menu_results(results, count, main_sample_work, vals_mass, lang_base, conf_base);
		delete [] results;
	}
}