#include "system.hpp"

int main(int argc, char* argv[]) {
	System system;
	system.init(argv[1]);
	system.main();
	system.end();
	return 0;
}