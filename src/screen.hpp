#pragma once
#include "buffer.hpp"
#include "cursor.hpp"

class Cursor;

class Screen {
	int height, width; // ターミナルの縦幅、横幅
	int top, left; // 描画範囲の最上、最左のスクリーン座標
	bool need_write_again; // 再描画の必要性
	bool write_linenumber; // 行番号を描画するか
	int width_linenumber; // 行番号の幅
	int tabsize; // タブ幅
public:
	void set_height_and_width(); // ターミナルの縦横幅を取得
	void set_top_and_left(int top, int left); // 描画範囲の最上、最左のバッファ座標を設定
	void set_need_write_again(bool need); // 再描画の必要性を設定
	void set_write_linenumber(bool write_linenumber); // 行番号を描画するかを設定
	void set_width_linenumber(int width_linenumber); // 行番号の幅を設定
	void set_tabsize(int tabsize); // タブサイズを設定
	std::pair<int,int> get_top_and_left(); // 描画範囲の最上、最左のバッファ座標を返す
	std::pair<int,int> get_height_and_width(); // ターミナルの縦幅、横幅を返す
	bool get_need_write_again(); // 再描画の必要性を返す
	bool get_write_linenumber();  // 行番号を描画するかを返す
	int get_width_linenumber(); // 行番号の幅を返す
	int get_tabsize(); // タブサイズを返す
	void write_char(int y, int x, char ch, int color); // (y, x)に１文字描画
	void write_oneline(int buf_y, int y, Buffer &buffer); // バッファを１行描画する
	void write_allline(Buffer &buffer, Cursor &cursor); // 描画範囲のバッファを全て描画
	void erase_oneline(int y); // バッファの描画を1行消す
	void scroll_up(Buffer &buffer, Cursor &cursor); // スクリーンを上に1スクロール
	void scroll_down(Buffer &buffer, Cursor &cursor); // スクリーンを下に1スクロール
	void write_statusline(int y, Buffer &buffer, Cursor &cursor); // ステータスラインを描画
};