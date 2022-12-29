#pragma once
#include "buffer.hpp"
#include "screen.hpp"
#include "command.hpp"

class Screen;
class Command;

class Cursor {
	int y, x; // スクリーン座標
	int buf_y, buf_x; // バッファ座標(n行目、n文字目)
	int cmd_x; // commandmode時の座標
	int shaft; // 行移動の際、スクリーン座標をなるべくぶらさないようにするための軸座標
	bool updown; // 行移動の途中か
	int mode; // 0:insert, 1:command
	bool contact_statusline; // 非command時にカーソルがステータスラインと接触しているか
public:
	void set_yx(int y, int x); // スクリーン座標を設定
	void set_buf_yx(int buf_y, int buf_x); // バッファ座標を設定
	void set_cmd_x(int cmd_x); // commandmode時の座標を設定
	void set_shaft(int shaft); // スクリーン座標をぶらさないための軸座標を設定
	void set_updown(bool updown); // 行移動の途中かを設定
	void set_mode(int mode); // モードを設定
	void set_contact_statusline(bool contact_statusline); // 非command時にカーソルがステータスラインと接触しているかを設定
	void set_x_from_bufx(Buffer &buffer, Screen &screen); // バッファ座標からターミナル座標を設定
	bool set_left_from_bufx(Buffer &buffer, Screen &screen); // バッファ座標から描画範囲の最左のスクリーン座標を設定
	void set_bufx_from_shaft(Buffer &buffer, Screen &screen); // ターミナル座標から軸座標になるべく近くなるようにバッファ座標を設定
	std::pair<int,int> get_yx(); // スクリーン座標を返す
	std::pair<int,int> get_buf_yx(); // バッファ座標を返す
	int get_cmd_x(); // commandmode時の座標を返す
	int get_shaft(); // スクリーン座標をぶらさないための軸座標を返す
	bool get_updown(); // 行移動の途中かを返す
	int get_mode(); // モードを返す
	bool get_contact_statusline(); // 非command時にカーソルがステータスラインと接触しているかを返す
	void write(Screen &screen); // カーソルを描画
	void move_right(Buffer &buffer, Screen &screen, Cursor &cursor); // カーソルを右に1動かす
	void move_left(Buffer &buffer, Screen &screen, Cursor &cursor); // カーソルを左に1動かす
	void move_up(Buffer &buffer, Screen &screen, Cursor &cursor); // カーソルを上に1動かす
	void move_down(Buffer &buffer, Screen &screen, Cursor &cursor); // カーソルを下に1動かす
	void move_begin(); // カーソルを行の先頭に動かす
	void move_end(Buffer &buffer, Screen &screen, Cursor &cursor); // カーソルを行の末尾に動かす
	void change_mode(int mode, Screen &screen, Command &command); // モードを変更する
	void move_right_cmd(Command &command); // commandmode時にカーソルを右に1動かす
	void move_left_cmd(); // commandmode時にカーソルを右に1動かす
};