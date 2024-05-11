
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

using str = std::string;

fstream log("log.txt", ios::out);

inline str gettime() {
	return std::ctime(std::chrono::system_clock::now());
}

void log(str message, str process = "game") {
	log << "[" << gettime() << "] " << process << ": " << message << '\n';
}
