
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


namespace CMD {

	using str = std::string;

	typedef void (*ScriptFunction)(std::vector<str>&);
	typedef void (*UpdateFunction)(void);
	using Onzero = UpdateFunction;

	typedef bool (*Condition)(std::vector<str>);
	typedef void (*Result)(std::vector<str>);

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

        constexpr auto update_interval = ::std::chrono::milliseconds{100};

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

	void testtriggers(std::vector<str>& toks, std::vector<Trigger>& triggers) {
		std::vector<int> erase = {};
                using size_t = decltype(erase)::size_type;
		for(size_t i = 0; i < triggers.size(); i++) {
			Trigger trigger = triggers[i];
			if(trigger.condition(toks)) {
				trigger.result(toks);
				if(trigger.once) {
					erase.insert(erase.begin(), i);
				}
			}
		}
		for(size_t i = 0; i < erase.size(); i++) {
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

	void exitt(std::vector<str> &args) {
		std::cout << "Exiting...\n";
		log(prgname + " terminated");
		Engine.detach();
		if(live) {
			Spawner.detach();
		}
	}

	void update() {
           ::std::vector<str> empty;
           testtriggers(empty, triggersanytime);
           updatee();
	}

	void bungle() {}

        void execute_command(
                ScriptFunction command,
                ::std::vector <str> args)
        {
           command(args);
        }

        void execute_update(UpdateFunction updater) {
           updater();
        }

        ::std::mutex command_mutex;
        ::std::condition_variable command_ready_condition;
        ScriptFunction command = nullptr;
        ::std::vector<str> arguments;

        void engine_loop(
                ::std::stop_token const stop,
                str const name,
                UpdateFunction updater
        ) {
           using clock = ::std::chrono::high_resolution_clock;
           auto last_update = clock::now();
           auto next_update = last_update + update_interval;
           // Common code.
           auto check_time = [&last_update, &next_update]() {
              auto current_time = clock::now();
              // Account for the clock possibly going backwards
              // (someone sets the time, for example).
              if (current_time < last_update) {
                 last_update = current_time;
                 next_update = current_time + update_interval;
              }
              return current_time;
           };
           while (!stop.stop_requested()) {
              ::std::unique_lock lk{command_mutex};
              auto current_time = check_time();
              auto time_to_wait = next_update - current_time;
              auto const time_zero = time_to_wait.zero();
              if (time_to_wait < time_zero) {
                 time_to_wait = time_zero;
              }
              command_ready_condition.wait_for(
                      lk, time_to_wait,
                      [&stop]{
                         return stop.stop_requested() || command != nullptr;
                      }
              );
              if (!stop.stop_requested()) {
                 if (command != nullptr) {
                    // Copy out command and arguments.
                    auto command_copy = command;
                    // So, if someone changes the type of arguments,
                    // this will still be right.
                    decltype(arguments) arg_copy;
                    // Using swap in this way here cleanly sets arguments to be
                    // again empty, while simultaneously copying its old value
                    // (the value before it was empty) into arg_copy.
                    arg_copy.swap(arguments);
                    // Now rest command to nullptr to indicate we have it and
                    // are ready for a new command.
                    command = nullptr;
                    // Release the lock since we're done messing with command
                    // and arguments now.
                    lk.release();
                    // Notify the command reading and parsing loop if it's
                    // waiting to set a command.
                    command_ready_condition.notify_all();
                    execute_command(command_copy, ::std::move(arg_copy));
                 } else {
                    lk.release();
                 }
                 if (!stop.stop_requested()) {
                    current_time = check_time();
                    if (current_time >= next_update) {
                       log("Running updater.");
                       execute_update(updater);
                       // Moving time forward in this way makes sure the update
                       // interval remains consistent even if commands or
                       // updates take some amount of time.
                       last_update = next_update;
                       next_update += update_interval;
                    }
                 }
              }
           }
           log("Stop of engine loop requested!");
        }

        ::std::jthread start_engine(str const &name, UpdateFunction updater) {
           // jthread has the stop semaphore stuff built into it.
           return ::std::jthread(engine_loop, name, updater);
        }

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
		addcommand("exit", exitt);
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
