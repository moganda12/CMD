
#include <iostream>
#include <ao/ao.h>
#include <functional>

#include "./CMD.hpp"

str name = "test";

str ver = "0.0.0";

void dummy(std::vector<str> args) {
	std::cout << "dummy\n";
}

int abalabada = 0;

bool adisAbaba(std::vector<str> args) {
	return abalabada == 1234;
}

int main() {
	CMD::init(name, "Command? ");
	CMD::log("Booting " + name + ".\n ver " + ver);
	CMD::addcommand("dummy", dummy);
	CMD::addtrigger(adisAbaba, dummy, true, 45);
	CMD::log("Program booted");
	CMD::run();
	sleep(6);
	abalabada = 1234;
	CMD::Engine.join();
	return 0;
}
