#include "conf_lang\conf_lang.h"
#include "win_lib.h"
#include "terminal_op.h"

void star() {
    Beep(392, 750); Beep(392, 750); Beep(392, 750); Beep(311, 563); Beep(466, 188);
    Beep(392, 750); Beep(311, 653); Beep(466, 188); Beep(392, 1500);
	Beep(587, 750); Beep(587, 750); Beep(587, 750); Beep(622, 653); Beep(466, 188);
    Beep(369, 750); Beep(311, 653); Beep(466, 188); Beep(391, 1500);
}

void start_dir(std::vector <std::string> lang_base) {
	TCHAR buffer[MAX_PATH];
	GetCurrentDirectory(sizeof(buffer),buffer);
	std::string buffer_str = buffer;
	buffer_str += "\\";
	std::string banka; banka.clear();
	int l_buffer_str = buffer_str.length();
	bool admin_stat = false;
	for (int i = 0; i <= l_buffer_str; i++) {
		if ((buffer_str[i] == '\\') || (buffer_str[i] == ':')) {
			if (banka == "Program Files") {admin_stat = true; break;}
			else {banka.clear(); continue;}
		}
		banka += buffer_str[i];
	}
	if (admin_stat == true){
		system(CLEAR_SCREEN);
		std::cout << lang("alarm_dir", lang_base) << std::endl;
		if (_getch() != KEY_ENTER) {
			system(CLEAR_SCREEN);
			std::cout << lang("bye_bye", lang_base) << std::endl;
			std::cout << lang("press_any_key", lang_base) << std::endl;
			_getch();
			exit(0);
		}
	}
}