#include <ncurses.h>
#include "cursor.hpp"

void Cursor::set_yx(int y, int x) {
	Cursor::y = y;
	Cursor::x = x;
}

void Cursor::set_buf_yx(int buf_y, int buf_x) {
	Cursor::buf_y = buf_y;
	Cursor::buf_x = buf_x;
}

void Cursor::set_cmd_x(int cmd_x) {
	Cursor::cmd_x = cmd_x;
}

void Cursor::set_shaft(int shaft) {
	Cursor::shaft = shaft;
}

void Cursor::set_updown(bool updown) {
	Cursor::updown = updown;
}

void Cursor::set_mode(int mode) {
	Cursor::mode = mode;
}

void Cursor::set_contact_statusline(bool contact_statusline) {
	Cursor::contact_statusline = contact_statusline;
}

void Cursor::set_x_from_bufx(Buffer &buffer, Screen &screen) {
	int cnt = 0;
	for (int i=0; i<Cursor::buf_x; i++) {
		cnt += (buffer.get_char(Cursor::buf_y, i)=='\t' ? screen.get_tabsize() : 1);
	}
	Cursor::x = cnt;
	return;
	if (buffer.get_char(Cursor::buf_y, buffer.get_size(Cursor::buf_y)-1)!='\n') {
		Cursor::x++;
	}
}  

bool Cursor::set_left_from_bufx(Buffer &buffer, Screen &screen) {
	int cnt = 0;
	auto [_, width] = screen.get_height_and_width();
	if (screen.get_write_linenumber()) {
		width -= screen.get_width_linenumber()+1;
	}
	for (int i=0; i<Cursor::buf_x; i++) {
		cnt += (buffer.get_char(Cursor::buf_y, i)=='\t' ? screen.get_tabsize() : 1);
	}
	int next_left = std::max(0, cnt-width+1);
	auto [top, left] = screen.get_top_and_left();
	screen.set_top_and_left(top, next_left);
	return (left != next_left);
}

void Cursor::set_bufx_from_shaft(Buffer &buffer, Screen &screen) {
	int cnt = 0;
	for (int i=0; i<buffer.get_size(Cursor::buf_y); i++) {
		if (cnt >= Cursor::shaft) {
			if (i == 0) {
				Cursor::buf_x = i;
			} else {
				int pre = cnt-(buffer.get_char(Cursor::buf_y, i-1)=='\t' ? screen.get_tabsize() : 1);
				if (Cursor::shaft-pre >= cnt-Cursor::shaft) Cursor::buf_x = i;
				else Cursor::buf_x = i-1;
			}
			return;
		}
		cnt += (buffer.get_char(Cursor::buf_y, i)=='\t' ? screen.get_tabsize() : 1);
	}
	Cursor::buf_x = buffer.get_size(Cursor::buf_y)-1;
}

std::pair<int,int> Cursor::get_yx() {
	return std::make_pair(Cursor::y, Cursor::x);
}

std::pair<int,int> Cursor::get_buf_yx() {
	return std::make_pair(Cursor::buf_y, Cursor::buf_x);
}

int Cursor::get_cmd_x() {
	return Cursor::cmd_x;
}

int Cursor::get_shaft() {
	return Cursor::shaft;
}

bool Cursor::get_updown() {
	return Cursor::updown;
}

int Cursor::get_mode() {
	return Cursor::mode;
}

bool Cursor::get_contact_statusline() {
	return Cursor::contact_statusline;
}

void Cursor::write(Screen &screen) {
	auto [height, width] = screen.get_height_and_width();
	if (Cursor::mode == 1) {
		move(height-1, Cursor::cmd_x+2);
		return;
	}
	int x = std::min(Cursor::x+(screen.get_write_linenumber() ? screen.get_width_linenumber()+1 : 0), width-1);
	move(Cursor::y, x);
}

void Cursor::move_right(Buffer &buffer, Screen &screen, Cursor &cursor) {
	Cursor::updown = false;
	bool onemore = (!buffer.empty_oneline(Cursor::buf_y))&&(buffer.get_char(Cursor::buf_y, buffer.get_size(Cursor::buf_y)-1)!='\n');
	if (Cursor::buf_x+1 < buffer.get_size(Cursor::buf_y)+onemore) {
		Cursor::buf_x++;
		Cursor::set_x_from_bufx(buffer, screen);
	} else if (Cursor::buf_y+1 < buffer.get_number_of_line()) {
		Cursor::move_down(buffer, screen, cursor);
		Cursor::move_begin();
	}
}

void Cursor::move_left(Buffer &buffer, Screen &screen, Cursor &cursor) {
	Cursor::updown = false;
	if (Cursor::buf_x > 0) {
		Cursor::buf_x--;
		Cursor::set_x_from_bufx(buffer, screen);
	} else if (Cursor::buf_y > 0) {
		Cursor::move_up(buffer, screen, cursor);
		Cursor::move_end(buffer, screen, cursor);
	}
}

void Cursor::move_up(Buffer &buffer, Screen &screen, Cursor &cursor) {
	Cursor::updown = true;
	if (Cursor::buf_y > 0) {
		Cursor::buf_y--;
		Cursor::set_bufx_from_shaft(buffer, screen);
		Cursor::set_x_from_bufx(buffer, screen);
		if (Cursor::y > 0) Cursor::y--;
		else screen.scroll_down(buffer, cursor);
	}
}

void Cursor::move_down(Buffer &buffer, Screen &screen, Cursor &cursor) {
	Cursor::updown = true;
	if (Cursor::buf_y+1 < buffer.get_number_of_line()) {
		Cursor::buf_y++;
		Cursor::set_bufx_from_shaft(buffer, screen);
		Cursor::set_x_from_bufx(buffer, screen);
		auto [height, _] = screen.get_height_and_width();
		if (Cursor::y+1 < height-(Cursor::mode==1 ? 2: 1)) Cursor::y++;
		else screen.scroll_up(buffer, cursor);
	}
}

void Cursor::move_begin() {
	Cursor::updown = false;
	Cursor::buf_x = 0;
	Cursor::x = 0;
}

void Cursor::move_end(Buffer &buffer, Screen &screen, Cursor &cursor) {
	Cursor::updown = false;
	Cursor::buf_x = buffer.get_size(Cursor::buf_y)-1;
	Cursor::set_x_from_bufx(buffer, screen);
	bool onemore = (!buffer.empty_oneline(Cursor::buf_y))&&(buffer.get_char(Cursor::buf_y, buffer.get_size(Cursor::buf_y)-1)!='\n');
	if (onemore) Cursor::move_right(buffer, screen, cursor);
}

void Cursor::change_mode(int mode, Screen &screen, Command &command) {
	if ((Cursor::mode==1) == (mode==1)) return;
	auto [top, left] = screen.get_top_and_left();
	if (Cursor::contact_statusline) {
		screen.set_top_and_left(top+(Cursor::mode==1 ? -1 : 1), left);
		Cursor::y += (Cursor::mode==1 ? 1 : -1);
	}
	if (Cursor::mode==1 || mode==1) {
		command.clear();
		Cursor::cmd_x = 0;
	}
	Cursor::set_mode(mode);
	screen.set_need_write_again(true);
}

void Cursor::move_right_cmd(Command &command) {
	if (Cursor::cmd_x+1 <= command.get_size()) {
		Cursor::cmd_x++;
	}
}

void Cursor::move_left_cmd() {
	if (Cursor::cmd_x > 0) {
		Cursor::cmd_x--;
	}
}