#include <fstream>
#include <iostream>
#include <string>
#include "config.hpp"

void Config::read(Screen &screen) {
	std::ifstream ifs("config.txt");
	if (ifs.fail()) return;
	std::cin.rdbuf(ifs.rdbuf());
	std::string type;
	int x;
	while (std::cin >> type >> x) {
		if (type == "tabsize") {
			screen.set_tabsize(x);
		} else if (type == "linenumber") {
			screen.set_write_linenumber(x);
		}
	}
}