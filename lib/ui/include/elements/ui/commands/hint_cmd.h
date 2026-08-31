#ifndef ELEMENTS_UI_COMMANDS_HINT_CMD_H
#define ELEMENTS_UI_COMMANDS_HINT_CMD_H

#include <elements/game.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <cstring>

namespace elements {
	int hint_cmd(int argv, char** argc, std::istream& cin, std::ostream& cout) {
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
				<< "hint *[flags]" << std::endl
				<< "flags:" << std::endl
				<< "--version | -v : prints version info about hint command" << std::endl
				<< "--help | -h : prints help info about hint command" << std::endl;
			return 0;
		}
		if (is_version_invoke) {
			cout
				<< "hint 1.0 - elements game hint command" << std::endl;
			return 0;
		}

		game_state* state = game_state::global_game_state();
		auto inventory = state->element_inventory();
		auto element_registry = state->element_registry();
		auto recipe_registry = state->recipe_registry();


		for (auto& e : inventory->element_states()) {
			auto& [k, v] = e;
			if (!v.can_use()) continue;

			const std::vector<element_token>* complements;
			if (!recipe_registry->try_get_complements(k, &complements)) continue;

			for (auto t : *complements) {
				element_state* state;
				if (!inventory->try_get_element_state(t, &state) || !state->can_use()) continue;

				element_token out_tok;
				recipe_registry->try_get_out(k, t, &out_tok); // guaranteed success

				element_state* state_o;
				if (inventory->try_get_element_state(out_tok, &state_o) && state_o->is_unlocked()) continue;

				// k + t -> out_tok is valid and out_tok is new

				element_desc a_desc = element_registry->get_element_desc(k);
				element_desc b_desc = element_registry->get_element_desc(t);

				cout << "mix " << a_desc.name() << " " << b_desc.name() << std::endl;
				goto loop_break_had_hint; //break out of nested loop
			}
		}

		cout << "there are no unknown elements!" << std::endl;

		return 0;

		loop_break_had_hint:
		return 0;
	}
}

#endif // ELEMENTS_UI_COMMANDS_HINT_CMD_H