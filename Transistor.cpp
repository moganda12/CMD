#include <gmpxx.h>
#include <math.h>
#include <sstream>

#include "./src/CMD.hpp"

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

using str = std::string;

using integer = mpz_class;
using flat = mpf_class;
using number = mpq_class;

struct TclickerSave {
	number transistorBalance;
	number totalTransistors;
	/*number ascentionLevels;
	number heavenlyMicrochips;*/
	integer clicks; 
    integer cusors, moss, smallFABs, mediumFABs, largeFABs; 
	//integer i860s, startups;
};

TclickerSave save = {0, 0, 0, 0, 0, 0, 0, 0};

str name = "Transistor Clicker";
str version = "0.0.1 DevBuild 1.0";

number cursorPrice = 15, mossPrice = 100, smallFABPrice = 1'000, mediumFABprice = 11'000, largeFABPrice = 120'000;
number cursorYeild = 0.1, mossYeild = 1, smallFABYeild = 10, mediumFABYeild = 60, largeFABYeild = 260;

str TransitorsString(number transistors, str colA = BOLDGREEN, str colB = BOLDBLUE) {
	std::stringstream ss;
	str plural = transistors == 1 ? "transistor" : "transistors";
	ss << colA << transistors << ' ' << colB << plural;
	return ss.str();
}

void increaseTransistors(number by) {
	save.transistorBalance += by;
	save.totalTransistors += by;
}

void onTick(std::vector<str>&) {
}

void onExit(std::vector<str>&) {
	std::cout << BOLDRED << "Exiting Human Controll Console...";
	CMD::log("Terminating...");
}

void click() {
	integer clickValue = 1;
	increaseTransistors(clickValue);
	save.clicks++;
	std::cout << BOLDWHITE << "Click! " << BOLDBLUE << "You made " << BOLDGREEN << clickValue << BOLDBLUE << " Transistors!" << RESET << '\n';
}

int main() {
	CMD::log("Program loaded");

	CMD::exit = onExit;

	CMD::log("Program iniitialized");
	std::jthread gameThread = CMD::init("Transistor Clicker", GREEN + str("@HCC") + BOLDBLUE + " ~/You" + RESET + "$ ");

	CMD::command_loop(click);

	CMD::kill(gameThread);

	return 0;
}
