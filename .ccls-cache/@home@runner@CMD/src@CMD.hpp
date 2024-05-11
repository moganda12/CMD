
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <vector>
#include <unordered_map> 
#include <functional>
#include <thread>

#include "./skylibs/split.hpp"
#include "./skylibs/eraseat.hpp"

using str = std::string;

typedef void (*ScriptFunction)(std::vector<str>);
typedef void (*UpdateFunction)(void);
using Onzero = UpdateFunction;

typedef bool (*Condition)(std::vector<str>);
typedef void (*Result)(std::vector<str>);

namespace CMD {

	struct Trigger {
		Condition condition;
		Result result;
		bool once;
	};

	bool on = false;
	
	str name = "CMD:\\";

	str ver = "0.1.0";

	str prgname;

	str prompt;
	
	std::ofstream logfile;

	std::unordered_map<str, ScriptFunction> commands;

	std::thread Engine;

	std::thread Update;
	std::thread Spawner;

	std::vector<Trigger> triggersbefore;
	std::vector<Trigger> triggersafter;
	std::vector<Trigger> triggersanytime;

	bool live;

	UpdateFunction updatee;

	int interval;

	Onzero onzero;

	void addtrigger(Condition condition, Result result, bool once, int time = 0) {
		switch(time) {
			case 0:
				triggersbefore.push_back({condition, result, once});
				break;
			case 1:
				triggersafter.push_back({condition, result, once});
				break;
			case 2:
				triggersanytime.push_back({condition, result, once});
				break;
			default:
				logfile << "YU DUN MESSED UP SILLY: INVALID TIME FO LITTE TRIGGGY PIGGY\n";
		}
	}

	void testtriggers(std::vector<str> toks, std::vector<Trigger>& triggers) {
		std::vector<int> erase = {};
		for(int i = 0; i < triggers.size(); i++) {
			Trigger trigger = triggers[i];
			if(trigger.condition(toks)) {
				trigger.result(toks);
				if(trigger.once) {
					erase.insert(erase.begin(), i);
				}
			}
		}
		for(int i = 0; i < erase.size(); i++) {
			eraseAt(triggers, erase[i]);
		}
	}

	inline str gettime() {
		std::time_t time = std::time(nullptr);
		str temp = std::asctime(std::localtime(&time));
		temp.pop_back();
		return temp;
	}

	void log(str message, str user = "game") {
		logfile << "[" << gettime() << "] <" << user << ">: " << message << '\n';
	}

	void addcommand(str name, ScriptFunction func) {
		commands[name] = func;
	}

	void remcommand(str name) {
		if(commands.find(name) != commands.end()) {
			commands.erase(name);
		}
	}

	void runcomm(str commandfull) {
		std::vector<str> args = sky::split(commandfull, " ");
		testtriggers(args, triggersbefore);
		if(args.size() > 0) {
		str command = args[0];
		args.erase(args.begin());
		if (commands.find(command) != commands.end()) {
			ScriptFunction callback = commands[command];
			callback(args);
		} else {
			std::cout << "Command " + command + " not found.\n";
		}
		} else {onzero();}
		testtriggers(args, triggersafter);
	}

	inline void terminate() {
		logfile << "[" << gettime() << "] : " << name << " terminated\n";
	}

	void exitt(std::vector<str> args) {
		std::cout << "Exiting...\n";
		log(prgname + " terminated");
		Engine.detach();
		if(live) {
			Spawner.detach();
		}
		exit(0);
	}

	void update() {
		testtriggers({}, triggersanytime);
		updatee();
	}

	void bungle() {}

	void errzero() {
		std::cout << "BAD BOY: ENETER A REAL COMMAND\n";
	}

	void spawner() {
		while(on) {
			Update = std::thread(updatee);
			std::this_thread::sleep_for(std::chrono::microseconds(interval));
		}
	}

	inline void init(str tename, str promptstyle, Onzero onzero = errzero, bool livee = false, UpdateFunction upadateee = bungle, int intervalus = 62500) {
		logfile.open("log.txt", std::ios::app);
		prgname = tename;
		prompt = promptstyle;
		if(livee) {
			updatee = upadateee;
			live = true;
		}
		commands["exit"] = exitt;
		logfile << "\n\n\n[" << gettime() << "] : " << name << " initialized\n" << name << " ver " << ver << '\n';
	}

	void threddy() {
		while(on) {
			str command;
			std::cout << prompt;
			std::getline(std::cin, command);

			runcomm(command);
		}
		return;
	}

	void run() {
		on = true;
		Engine = std::thread(threddy);
		if(live) {
			Spawner = std::thread(spawner);
		}
	}
};
