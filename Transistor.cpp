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

std::hash<str> hasher;

long hash(str s) {
	return hasher(s);
}

enum Buildings {
    cursor = -5238174239041902407,
    moss = 7569504340697116271,
    smallFAB = -2000947298636880309,
    mediumFAB = -1712167728928864321,
    largeFAB = -7940484003211731860
};

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
str version = "0.0.1 DevBuild 4.1_1";

number cursorPrice = 15, mossPrice = 100, smallFABPrice = 1'000, mediumFABprice = 11'000, largeFABPrice = 120'000, intelI860Price = 1'3;
number cursorYeild = 0.1, mossYeild = 1, smallFABYeild = 10, mediumFABYeild = 60, largeFABYeild = 260;
number expantionFactor = number(23, 20);

integer toInt(number x) {
	return integer(x);
}

str tolower(str s) {
	str ret = "";
	for(int i = 0; i < s.length(); i++) {
		ret += tolower(s[i]);
	}
	return ret;
}

number pow(number base, integer exponent) {
	number result = 1;
	for(integer i = 0; i < exponent; i++) {
		result *= base;
	}
	return result;
}

number expandPrice(number price, integer count) {
	return price * pow(expantionFactor, count);
}

void printTitileCard() {
	std::cout << BOLDBLUE << name << ' ' << BOLDGREEN << version << RESET << "\n\n\n";
}

void clear(std::vector<str>&) {
	int bung = system("clear");
	printTitileCard();
}

str TransitorsString(number transistors, str colA = BOLDGREEN, str colB = BOLDBLUE) {
	std::stringstream ss;
	str plural = transistors == 1 ? "transistor" : "transistors";
	ss << colA << transistors.get_num_mpz_t() << ' ' << colB << plural;
	return ss.str();
}

void increaseTransistors(number by) {
	save.transistorBalance += by;
	save.totalTransistors += by;
}

number calcCursorYeild() {
	return save.cusors * cursorYeild;
}

number calcTPS() {
	number TPS = 0;
	TPS += calcCursorYeild();

	return TPS;
}

void onTick(std::vector<str>&) {
	number TPS = calcTPS();
	increaseTransistors(TPS / 16);
}

void onExit(std::vector<str>&) {
	std::cout << BOLDRED << "Exiting Human Controll Console...\n";
	CMD::log("Terminating...");
}

void click() {
	number clickValue = 1;
	increaseTransistors(clickValue);
	save.clicks++;
	std::cout << BOLDWHITE << "Click! " << BOLDBLUE << "You made " << TransitorsString(clickValue) << RESET << '\n';
}

void balance(std::vector<str>& args) {
	CMD::log("Ran \"balance\" command");
	
	if(args.size() >= 3) {
		if(args[0] == "building") {
			integer count;
			number producing;
			number TPS = calcTPS();
			str buildingName;

			switch(hash(tolower(args[1]))) {
				case cursor:
					buildingName = "cursor";
				    count = save.cusors;
				    producing = calcCursorYeild();
				    break;
				default:
				    std::cout << BOLDRED << "Unknown building!\n";
				    return;
			}

		    integer thousandthsOfTPS = toInt((producing / TPS) * 1000);
			number percent = thousandthsOfTPS / 10;

			std::cout << BOLDBLUE << "You have " << buildingName << "s,\n";
			std::cout << "which poduce " << TransitorsString(producing) << " per second which is " << BOLDGREEN << percent << "%" << BOLDBLUE << " of your total TPS.\n";
		}
	}
	std::cout << BOLDBLUE << "You have " << TransitorsString(save.transistorBalance) << '\n';
	std::cout << BOLDBLUE << "You have made " << TransitorsString(save.transistorBalance) << " in total!\n";
	std::cout << BOLDBLUE << "You make" << TransitorsString(calcTPS()) << " per second!\n";

	CMD::log("They have" + TransitorsString(save.transistorBalance, "", "") + "!");
}

void buy(std::vector<str>& args) {
	if(args.size() < 1) {
		std::cout << BOLDRED << "You must specify somthing to buy!\n";
		return;
	}
	str building = args[0];

	str buildingName;
	number basePrice;
	integer* countPtr = nullptr;

	switch(hash(tolower(building))) {
		case cursor:
		    buildingName = "cursor";
		    basePrice = cursorPrice;
			countPtr = &save.cusors;
		    break;
		default:
		    std::cout << BOLDRED << "Unknown building!\n";
		    break;
	}

	integer& count = *countPtr;

	if(args.size() >= 2) {
		if(args[1] == "max") {
			integer totalBought = 0;
			number finalPrice = 0;

			while(save.transistorBalance >= finalPrice) {
				totalBought++;
				finalPrice += expandPrice(basePrice, count + totalBought);
			}

			totalBought--;
			finalPrice -= expandPrice(basePrice, count + totalBought);

			count += totalBought;
			save.transistorBalance -= finalPrice;

			if(count <= 0) {
				std::cout << BOLDRED << "You can't afford a " << buildingName << "!\n";
				return;
			}

			std::cout << BOLDBLUE << "You bought " << totalBought << " " << buildingName << "s for " << TransitorsString(finalPrice) << "!\n";

			return;
		}
		
		integer toPurchase(args[1]);
		number totalPrice = 0;

		for(integer i = 0; i < toPurchase; i++) {
			totalPrice += expandPrice(basePrice, i+count);
		}

		if(save.transistorBalance >= totalPrice) {
			save.transistorBalance -= totalPrice;
			count += toPurchase;

			std::cout << BOLDBLUE << "bought " << toPurchase << " " << buildingName << "s for " << TransitorsString(totalPrice) << "!\n";
		} else {
			std::cout << BOLDRED << "You don't have enough transistors to buy " << toPurchase << " for " << TransitorsString(totalPrice, BOLDRED, BOLDRED) << "!\n";
		}
	} else {
		if(save.transistorBalance >= expandPrice(basePrice, count)) {
			save.transistorBalance -= expandPrice(basePrice, count);
			count++;
		}

		std::cout << BOLDBLUE << "bought a " << buildingName << " for " << TransitorsString(expandPrice(basePrice, count - 1)) << "!\n";
	}
}

int main() {
	std::jthread gameThread = CMD::init(name, GREEN + str("@HCC") + BOLDBLUE + " ~/You" + RESET + "$ ");

	CMD::exit = onExit;

	CMD::addcommand("balance", balance);
	CMD::addcommand("clear",  clear);
	CMD::addcommand("buy", buy);

	CMD::log("Program iniitialized");

	CMD::runcomm("clear", click);

	CMD::command_loop(click);

	CMD::kill(gameThread);

	return 0;
}
