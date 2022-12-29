#include <fstream>
#include "buffer.hpp"

void Buffer::read(char* &filename) {
	Buffer::data.emplace_back(std::vector<char>());
	Buffer::filename = filename;
	std::ifstream ifs(filename);
	if (ifs.fail()) return;
	int buf_y = 0;
	char ch;
	while (ifs.get(ch)) {
		Buffer::data.at(buf_y).emplace_back(ch);
		if (ch == '\n') {
			Buffer::data.resize(++buf_y+1);
		}
	}
	if (Buffer::data.back().empty()) {
		Buffer::data.resize(buf_y);
	}
}

void Buffer::write(std::string filename) {
	std::ofstream file;
	file.open(filename, std::ios::out);
	for (std::vector<char> &oneline: Buffer::data) {
		for (char &ch: oneline) {
			file << ch;
		}
	}
	file.close();
}

std::string Buffer::get_filename() {
	return Buffer::filename;
}

char Buffer::get_char(int buf_y, int buf_x) {
	return Buffer::data.at(buf_y).at(buf_x);
}

int Buffer::get_size(int buf_y) {
	return Buffer::data.at(buf_y).size();
}

int Buffer::get_number_of_line() {
	return Buffer::data.size();
}

void Buffer::insert_char(int buf_y, int buf_x, char ch) {
	Buffer::data.at(buf_y).insert(Buffer::data.at(buf_y).begin()+buf_x, ch);
}

void Buffer::emplace_back_char(int buf_y, char ch) {
	Buffer::data.at(buf_y).emplace_back(ch);
}

void Buffer::erase_char(int buf_y, int buf_x) {
	Buffer::data.at(buf_y).erase(Buffer::data.at(buf_y).begin()+buf_x);
}

void Buffer::pop_back_char(int buf_y) {
	Buffer::data.at(buf_y).pop_back();
}

void Buffer::insert_oneline(int buf_y) {
	Buffer::data.insert(Buffer::data.begin()+buf_y, std::vector<char>({'\n'}));
}

void Buffer::erase_oneline(int buf_y) {
	Buffer::data.erase(Buffer::data.begin()+buf_y);
}

bool Buffer::empty_oneline(int buf_y) {
	return Buffer::data.at(buf_y).empty();
}