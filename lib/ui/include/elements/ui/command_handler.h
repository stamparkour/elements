#ifndef ELEMENTS_UI_COMMAND_HANDLER_H
#define ELEMENTS_UI_COMMAND_HANDLER_H

#include <unordered_map>
#include <string>
#include <string_view>
#include "word_iterable.h"
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

namespace elements {
	using command_type = std::function<int(int, char**, std::istream&, std::ostream&)>;

	class command_handler {
		std::unordered_map<std::string, command_type> commands;

		int help_cmd(int argc, char** argv, std::istream& cin, std::ostream& cout) {
			if (argc == 1) {
				cout 
					<< "for more information:" << std::endl
					<< "<command name> --help" << std::endl << std::endl;

				std::vector<std::string_view> lines{};
				for (auto& t : commands) {
					auto& [str, cmd] = t;
					lines.push_back(str);
				}
				std::ranges::sort(lines.begin(), lines.end());
				for (auto& l : lines) {
					cout << l << std::endl;
				}
				cout << std::endl;
				return 0;
			}
			return 1;
		}
		static int clear_cmd(int argc, char** argv, std::istream& cin, std::ostream& cout) {
			cout << "\x1B[2J\x1B[H";
			return 0;
		}
		static int echo_cmd(int argc, char** argv, std::istream& cin, std::ostream& cout) {
			for (int i = 1; i < argc; ++i) {
				cout << argv[i] << " ";
			}
			cout << std::endl;
			return 0;
		}
	public:
		command_handler() {
			using namespace std::placeholders;
			emplace("help", std::bind(&command_handler::help_cmd, this, _1, _2, _3, _4));
			emplace("clear", std::bind(&command_handler::clear_cmd, _1, _2, _3, _4));
			emplace("echo", std::bind(&command_handler::echo_cmd, _1, _2, _3, _4));
		}
		void emplace(const std::string& name, command_type cmd) {
			commands.emplace(name, cmd);
		}
		int simple_invoke(int argc, const char** argv, std::istream& cin, std::ostream& cout) {

			if (argc < 1) return 0x7E00;
			auto f = commands.find(argv[0]);
			if (f == commands.end()) return 0x7E01;
			auto& [str_out, cmd] = *f;

			return cmd(argc, const_cast<char**>(argv), cin, cout);
		}
		int invoke(const std::string& line, std::istream& cin = std::cin, std::ostream& cout = std::cout) {
			std::vector<std::string> words{};
			std::vector<const char*> cwords{};
			for (std::string_view w : elements::word_iterable(line)) {
				words.push_back(std::string{ w });
			}
			for (auto& w : words) {
				cwords.push_back(w.c_str());
			}
			return simple_invoke(words.size(), cwords.data(), cin, cout);
		}
	};
}

#endif // ELEMENTS_UI_COMMAND_HANDLER_H