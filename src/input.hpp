#pragma once
#include "buffer.hpp"
#include "screen.hpp"
#include "cursor.hpp"
#include "command.hpp"

class Input {
	int key = 0; // 最後に入力されたキー
public:
	void set_key(int key); // 最後に入力されたキーを設定する
	void operation(Buffer &buffer, Screen &screen, Cursor &cursor, Command &command); // 最後に入力されたキー、現在のモードにより従って操作を実行
};