
#include <iostream>
#include <functional>

#include "./CMD.hpp"

using CMD::str;
str name = "test";

str ver = "0.0.0";

void dummy(std::vector<str>& args) {
	std::cout << "dummy\n";
}

void my_updater() {
	::std::cout << "Updating game state.\n";
}

int abalabada = 0;

bool adisAbaba(std::vector<str> args) {
	return abalabada == 1234;
}

int main() {
	auto engine_thread = CMD::init(name, "Command? ");
	CMD::log("Booting " + name + ".\n ver " + ver);
	CMD::addcommand("dummy", dummy);
	CMD::log("Program booted");
	CMD::command_loop(CMD::errzero);
        CMD::log("Requesting stop.");
	engine_thread.request_stop();
	abalabada = 1234;
	engine_thread.join();
	return 0;
}
