#pragma once
#include <vector>
#include "buffer.hpp"
#include "screen.hpp"

class Screen;

class Command {
	std::vector<char> data;
public:
	void insert_char(int cmd_x, char ch); // コマンド列に1文字追加
	void clear(); // コマンド列を消去
	void erase_char(int cmd_x); // コマンド列から1文字削除
	int get_size(); // コマンド列の長さを返す
	void write_data(Screen &screen); // コマンド列を描画
	void run(Buffer &buffer); // コマンド列を実行
};