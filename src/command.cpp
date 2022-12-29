#include <ncurses.h>
#include "command.hpp"

void Command::insert_char(int cmd_x, char ch) {
	Command::data.insert(Command::data.begin()+cmd_x, ch);
}

void Command::clear() {
	Command::data.clear();
}

void Command::erase_char(int cmd_x) {
	Command::data.erase(Command::data.begin()+cmd_x);
}

int Command::get_size() {
	return Command::data.size();
}

void Command::write_data(Screen &screen) {
	auto [height, width] = screen.get_height_and_width();
	screen.write_char(height-1, 0, '>', 0);
	int x = 2;
	for (int i=0; i<Command::data.size(); i++) {
		screen.write_char(height-1, x, Command::data.at(i), 0);
		x++;
	}
}

void Command::run(Buffer &buffer) {
	for (int i=0; i<Command::data.size(); i++) {
		char ch = Command::data.at(i);
		if (ch == 'w') {
			buffer.write(buffer.get_filename());
		} else if (ch == 'q') {
			endwin();
			exit(0);
		}
	}
}