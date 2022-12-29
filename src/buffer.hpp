#pragma once
#include <vector>
#include <string>

class Buffer {
	std::vector<std::vector<char>> data; // バッファのデータ
	std::string filename; // ファイル名
public:
	void read(char* &filename); // 入力ファイルをバッファに読み込む
	void write(std::string filename); // バッファをファイルに書き込む
	std::string get_filename(); // ファイル名を返す
	char get_char(int buf_y, int buf_x); // バッファのbuf_y行目のbuf_x文字目を返す
	int get_size(int buf_y); // バッファのbuf_y行目の長さを返す
	int get_number_of_line(); // バッファの行数を返す
	void insert_char(int buf_y, int buf_x, char ch); // バッファに1文字書き込む
	void emplace_back_char(int buf_y, char ch); // バッファのbuf_y行目の末尾に1文字書き込む
	void erase_char(int buf_y, int buf_x); // バッファから1文字削除する
	void pop_back_char(int buf_y); // バッファのbuf_y行目の末尾から1文字削除する
	void insert_oneline(int buf_y); // バッファに1行追加する
	void erase_oneline(int buf_y); // バッファから1行削除する
	bool empty_oneline(int buf_y); // バッファのある行が空かを返す
};