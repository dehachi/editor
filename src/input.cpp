#include <tuple>
#include "input.hpp"

void Input::set_key(int key) {
	Input::key = key;
}

void Input::operation(Buffer &buffer, Screen &screen, Cursor &cursor, Command &command) {
	int mode = cursor.get_mode();
	if (mode == 0) {
		if (Input::key == 27) { // esc
			cursor.change_mode(1, screen, command);
		} else if (Input::key==9 || (32<=Input::key && Input::key<=126)) { // asciicode:9,32-126(水平tabとspaceから~まで)
			auto [buf_y, buf_x] = cursor.get_buf_yx();
			auto [y, _x] = cursor.get_yx();
			buffer.insert_char(buf_y, buf_x, static_cast<char>(Input::key));
			screen.erase_oneline(y);
			screen.write_oneline(buf_y, y, buffer);
			cursor.move_right(buffer, screen, cursor);
		} else if (Input::key == 263 || Input::key == 127) { // backspace
			auto [buf_y, buf_x] = cursor.get_buf_yx();
			if (buf_x == 0) {
				if (buf_y > 0) {
					cursor.move_left(buffer, screen, cursor);
					std::tie(buf_y, buf_x) = cursor.get_buf_yx();
					buffer.pop_back_char(buf_y);
					for (int i=0; i<buffer.get_size(buf_y+1); i++) {
						buffer.emplace_back_char(buf_y, buffer.get_char(buf_y+1, i));
					}
					buffer.erase_oneline(buf_y+1);
					screen.set_need_write_again(true);
				}
				return;
			}
			auto [y, x] = cursor.get_yx();
			buffer.erase_char(buf_y, buf_x-1);
			screen.erase_oneline(y);
			screen.write_oneline(buf_y, y, buffer);
			cursor.move_left(buffer, screen, cursor);
		} else if (Input::key == 330) { // delete
			auto [buf_y, buf_x] = cursor.get_buf_yx();
			if (buf_x == buffer.get_size(buf_y)-1) {
				if (buf_y < buffer.get_number_of_line()-1) {
					cursor.move_left(buffer, screen, cursor);
					buffer.pop_back_char(buf_y);
					for (int i=0; i<buffer.get_size(buf_y+1); i++) {
						buffer.emplace_back_char(buf_y, buffer.get_char(buf_y+1, i));
					}
					buffer.erase_oneline(buf_y+1);
					screen.set_need_write_again(true);
					cursor.move_right(buffer, screen, cursor);
				}
				return;
			}
			auto [y, x] = cursor.get_yx();
			buffer.erase_char(buf_y, buf_x);
			screen.erase_oneline(y);
			screen.write_oneline(buf_y, y, buffer);
		} else if (Input::key == 261) { // right
			cursor.move_right(buffer, screen, cursor);
		} else if (Input::key == 260) { // left
			cursor.move_left(buffer, screen, cursor);
		} else if (Input::key == 259) { // up
			cursor.move_up(buffer, screen, cursor);
		} else if (Input::key == 258) { // down
			cursor.move_down(buffer, screen, cursor);
		} else if (Input::key == 262) { // home
			cursor.move_begin();
		} else if (Input::key == 360) { // end
			cursor.move_end(buffer, screen, cursor);
		} else if (Input::key == 10) { // enter
			auto [buf_y, buf_x] = cursor.get_buf_yx();
			buffer.insert_oneline(buf_y+1);
			buffer.pop_back_char(buf_y+1);
			int sz_buf_y = buffer.get_size(buf_y);
			for (int i=buf_x; i<sz_buf_y; i++) {
				buffer.emplace_back_char(buf_y+1, buffer.get_char(buf_y, i));
			}
			for (int i=buf_x; i<sz_buf_y; i++) {
				buffer.pop_back_char(buf_y);
			}
			buffer.emplace_back_char(buf_y, '\n');
			cursor.move_down(buffer, screen, cursor);
			cursor.move_begin();
			screen.set_need_write_again(true);
		}
	} else if (mode == 1) {
		if (Input::key == 27) { // esc
			cursor.change_mode(0, screen, command);
		} else if (Input::key==9 || (32<=Input::key && Input::key<=126)) { // asciicode:9,32-126(水平tabとspaceから~まで)
			command.insert_char(cursor.get_cmd_x(), static_cast<char>(Input::key));
			cursor.move_right_cmd(command);
		} else if (Input::key == 263 || Input::key == 127) { // backspace
			if (cursor.get_cmd_x() > 0) {
				command.erase_char(cursor.get_cmd_x()-1);
				cursor.move_left_cmd();
			}
		} else if (Input::key == 330) { // delete
			command.erase_char(cursor.get_cmd_x());
		} else if (Input::key == 10) { // enter
			command.run(buffer);
			cursor.change_mode(0, screen, command);
		} else if (Input::key == 261) { // right
			cursor.move_right_cmd(command);
		} else if (Input::key == 260) { // left
			cursor.move_left_cmd();
		}
	}
}