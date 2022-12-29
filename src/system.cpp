#include <ncurses.h>
#include "system.hpp"

void System::init(char* &filename) {
	System::buffer.read(filename);
	System::screen.set_height_and_width();
	System::screen.set_top_and_left(0, 0);
	System::screen.set_need_write_again(true);
	System::screen.set_write_linenumber(true);
	System::screen.set_tabsize(4);
	System::cursor.set_yx(0, 0);
	System::cursor.set_buf_yx(0, 0);
	System::cursor.set_cmd_x(0);
	System::cursor.set_shaft(0);
	System::cursor.set_updown(false);
	System::cursor.set_mode(0);
	System::cursor.set_contact_statusline(false);
	System::input.set_key(0);
	System::config.read(System::screen);
	initscr();
	start_color();
	use_default_colors();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	noecho();
	cbreak();
	set_escdelay(0);
	keypad(stdscr, true);
	scrollok(stdscr, true);
}

void System::end() {
	endwin();
}

void System::main() {
	while (true) {
		if (System::screen.get_write_linenumber()) {
			int next_width_linenumber = 0, pow10 = 10;
			for (int i=1; i<9; i++) {
				if (System::buffer.get_number_of_line() < pow10) {
					next_width_linenumber = i;
					break;
				}
				pow10 *= 10;
			}
			if (next_width_linenumber != System::screen.get_width_linenumber()) {
				System::screen.set_need_write_again(true);
			}
			System::screen.set_width_linenumber(next_width_linenumber);
		}
		if (System::cursor.set_left_from_bufx(System::buffer, System::screen)) {
			System::screen.set_need_write_again(true);
		}
		if (System::screen.get_need_write_again()) {
			clear();
			System::screen.write_allline(System::buffer, System::cursor);
			System::screen.set_need_write_again(false);
		}
		auto [y, x] = System::cursor.get_yx();
		if (!System::cursor.get_updown()) {
			System::cursor.set_shaft(x);
		}
		auto [height, width] = System::screen.get_height_and_width();
		int mode = System::cursor.get_mode();
		int statusline_y = height-(mode==1 ? 2 : 1);
		if (mode != 1) {
			System::cursor.set_contact_statusline(y+1 == statusline_y);
		}
		System::screen.write_statusline(statusline_y, System::buffer, System::cursor);
		if (mode == 1) {
			System::command.write_data(System::screen);
		}
		System::cursor.write(System::screen);
		refresh();
		System::input.set_key(getch());
		System::input.operation(System::buffer, System::screen, System::cursor, System::command);
	}
}