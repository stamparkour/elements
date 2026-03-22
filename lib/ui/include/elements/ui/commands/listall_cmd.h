#ifndef ELEMENTS_UI_COMMANDS_LISTALL_CMD_H
#define ELEMENTS_UI_COMMANDS_LISTALL_CMD_H

#include <elements/game.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <random>

namespace elements {
	int listall_cmd(int argv, char** argc, std::istream& cin, std::ostream& cout) {
		bool is_help_invoke = false;
		bool is_version_invoke = false;
		int last_flag = 0;
		for (int i = 1; i < argv; i++) {
			if (!std::strcmp("--help", argc[i]) || !std::strcmp("-h", argc[i])) {
				is_help_invoke = true;
				last_flag = i;
				break;
			}
			else if (!std::strcmp("--version", argc[i]) || !std::strcmp("-v", argc[i])) {
				is_version_invoke = true;
				last_flag = i;
				break;
			}
		}
		if (is_help_invoke) {
			cout
				<< "prints out unused recipe of two element pair." << std::endl
				<< "combo *[flags] [count]" << std::endl
				<< "flags:" << std::endl
				<< "--version | -v : prints version info about combo command" << std::endl
				<< "--help | -h : prints help info about combo command" << std::endl;
			return 0;
		}
		if (is_version_invoke) {
			cout
				<< "combo 1.0 - elements game combo command" << std::endl;
			return 0;
		}

		game_state* state = game_state::global_game_state();
		auto inventory = state->element_inventory();
		auto element_registry = state->element_registry();
		auto recipe_registry = state->recipe_registry();
		std::vector<std::string> arr{};

		int total_combos = 1;

		if (argv == last_flag + 2) {
			total_combos = std::stoi(argc[last_flag + 1]);
		}

		auto& list = element_registry->name_token_map();
		for (auto i = list.begin(); i != list.end(); ++i) {
			const auto& tuple1 = *i;
			const auto& [a_str, a_token] = tuple1;

			arr.push_back(a_str);
		}

		std::ranges::sort(arr);

		for (auto& s : arr) {
			cout << s << std::endl;
		}

		return 0;
	}
}

#endif // ELEMENTS_UI_COMMANDS_LISTALL_CMD_H