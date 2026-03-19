#ifndef ELEMENTS_UI_COMMANDS_LIST_CMD_H
#define ELEMENTS_UI_COMMANDS_LIST_CMD_H

#include <elements/game.h>
#include <string>
#include <iostream>
#include <algorithm>

namespace elements {
	int list_cmd(int argv, char** argc, std::istream& cin, std::ostream& cout) {
		bool is_help_invoke = false;
		bool is_version_invoke = false;
		for (int i = 1; i < argv; i++) {
			if (!std::strcmp("--help", argc[i]) || !std::strcmp("-h", argc[i])) {
				is_help_invoke = true;
				break;
			}
			else if (!std::strcmp("--version", argc[i]) || !std::strcmp("-v", argc[i])) {
				is_version_invoke = true;
				break;
			}
		}
		if (is_help_invoke) {
			cout
				<< "list *[flags]" << std::endl
				<< "flags:" << std::endl
				<< "--version | -v : prints version info about list command" << std::endl
				<< "--help | -h : prints help info about list command" << std::endl
				<< "--regex | -r : prints all elements that contain a match for the regex expression" << std::endl;
			return 0;
		}
		if (is_version_invoke) {
			cout
				<< "list 1.0 - elements game list command" << std::endl;
			return 0;
		}

		game_state* state = game_state::global_game_state();
		auto& elements = state->element_inventory().element_states();
		std::vector<std::pair<element_token, std::string_view>> arr{};

		// loop through all elements and adds to arr
		for (const auto& e : elements) {
			auto& [token, ev] = e;
			auto& element_desc = state->element_registry().get_element_desc(token);
			if (!ev.is_unlocked()) continue;
			arr.push_back({ token, element_desc.name() });
		}

		std::ranges::sort(arr.begin(), arr.end(), [](auto a, auto b) -> bool {
			return a.first < b.first;
		});


		//cout stuff

		cout << "current inventory: " << std::endl;

		for (auto e : arr) {
			auto& [token, name] = e;

			cout << "  - " << name << std::endl;
		}

		return 0;
	}
}

#endif // ELEMENTS_UI_COMMANDS_LIST_CMD_H