#include <ncurses.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <algorithm>
#include "screen.hpp"

void Screen::set_height_and_width() {
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    Screen::height = ws.ws_row;
    Screen::width = ws.ws_col;
}

void Screen::set_top_and_left(int top, int left) {
    Screen::top = top;
    Screen::left = left;
}

void Screen::set_need_write_again(bool need) {
	Screen::need_write_again = need;
}

void Screen::set_write_linenumber(bool write_linenumber) {
	Screen::write_linenumber = write_linenumber;
}

void Screen::set_width_linenumber(int width_linenumber) {
	Screen::width_linenumber = width_linenumber;
}

void Screen::set_tabsize(int tabsize) {
	Screen::tabsize = tabsize;
}

std::pair<int,int> Screen::get_top_and_left() {
	return std::make_pair(Screen::top, Screen::left);
}

std::pair<int,int> Screen::get_height_and_width() {
	return std::make_pair(Screen::height, Screen::width);
}

bool Screen::get_need_write_again() {
	return need_write_again;
}

bool Screen::get_write_linenumber() {
	return Screen::write_linenumber;
}

int Screen::get_width_linenumber() {
	return Screen::width_linenumber;
}

int Screen::get_tabsize() {
	return Screen::tabsize;
}

void Screen::write_char(int y, int x, char ch, int color) {
	attron(COLOR_PAIR(color));
	mvaddch(y, x, ch);
	attroff(COLOR_PAIR(color));
}

void Screen::write_oneline(int buf_y, int y, Buffer &buffer) {
	int cnt = 0, idx = 0, x = 0;
	if (Screen::write_linenumber) {
		x += Screen::width_linenumber+1;
		move(y, 0);
		printw("%*d", Screen::width_linenumber, buf_y+1);
	}
	for (idx=0; idx<buffer.get_size(buf_y); idx++) {
		int sz = (buffer.get_char(buf_y, idx)=='\t' ? Screen::tabsize : 1);
    	if (cnt >= Screen::left) break;
    	cnt += sz;
	}
	if (Screen::left > cnt) {
		return;
	}
	x += cnt-Screen::left;
	for (int i=idx; i<buffer.get_size(buf_y); i++) {
		char c = buffer.get_char(buf_y, i);
		if (c == '\t') x += Screen::tabsize;
		else Screen::write_char(y, x++, c, 0);
		if (x >= Screen::width) break;
	}
}

void Screen::write_allline(Buffer &buffer, Cursor &cursor) {
	int y = 0, mode = cursor.get_mode();
	int bottom = std::min(Screen::top+Screen::height-(mode==1 ? 2 : 1), buffer.get_number_of_line());
	for (int line=Screen::top; line<bottom; line++) {
		Screen::write_oneline(line, y, buffer);
		y++;
	}
}

void Screen::erase_oneline(int y) {
	move(y, 0);
	deleteln();
	insertln();
}

void Screen::scroll_up(Buffer &buffer, Cursor &cursor) {
	wscrl(stdscr, 1);
	Screen::top++;
	int next_bottom = Screen::top+Screen::height-1;
	if (next_bottom < buffer.get_number_of_line()) {
		int mode = cursor.get_mode();
		Screen::erase_oneline(Screen::height-(mode==1 ? 3 : 2));
		Screen::write_oneline(next_bottom, Screen::height-(mode==1 ? 3 : 2), buffer);
	}
}

void Screen::scroll_down(Buffer &buffer, Cursor &cursor) {
	wscrl(stdscr, -1);
	int next_top = Screen::top-1;
	if (next_top >= 0) {
		Screen::write_oneline(next_top, 0, buffer);
		Screen::top = next_top;
	}
	int mode = cursor.get_mode();
	Screen::erase_oneline(Screen::height-(mode==1 ? 2 : 1));
}

void Screen::write_statusline(int y, Buffer &buffer, Cursor &cursor) {
	std::string filename = buffer.get_filename();
	auto [row, col] = cursor.get_buf_yx();
	std::string row_col = "["+std::to_string(row+1)+","+std::to_string(col+1)+"]";
	int space = Screen::width-filename.size()-row_col.size();
	std::string statusline = filename+std::string(space, ' ')+row_col;
	Screen::erase_oneline(y);
	move(y-1, 0);
	attron(A_BOLD);
	attron(COLOR_PAIR(1));
	printw("%s", statusline.c_str());
	attroff(COLOR_PAIR(1));
	attroff(A_BOLD);
	move(y-1, 0);
	insertln();
	if (Screen::top+y-1 < buffer.get_number_of_line()) {
		Screen::write_oneline(Screen::top+y-1, y-1, buffer);
	}
}