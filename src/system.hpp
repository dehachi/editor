#pragma once
#include "buffer.hpp"
#include "screen.hpp"
#include "cursor.hpp"
#include "input.hpp"
#include "command.hpp"
#include "config.hpp"

class System {
	Buffer buffer; // バッファ
	Screen screen; // スクリーン管理
	Cursor cursor; // カーソル
	Input input; // 入力
	Command command; // コマンド
	Config config; // 設定
public:
	void init(char* &filename); // 起動時の初期処理
	void end(); // 終了時の処理
	void main(); // メイン処理
};