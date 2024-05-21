
#include <iostream>
#include <functional>

#include "./CMD.hpp"

using CMD::str;
str name = "test";

str ver = "0.0.0";

void dummy(std::vector<str> args) {
	std::cout << "dummy\n";
}

void dummy(std::vector<str>& args) {
	std::cout << "dummy\n";
}

int abalabada = 0;

bool adisAbaba(std::vector<str> args) {
	return abalabada == 1234;
}

int main() {
	auto engine_thread = CMD::start_engine(name, CMD::bungle);
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
