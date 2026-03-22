#ifndef ELEMENTS_UI_COMMANDS_MIX_CMD_H
#define ELEMENTS_UI_COMMANDS_MIX_CMD_H

#include <elements/game.h>
#include <string>
#include <iostream>

namespace elements {
	int inline_mix(const char* ac, const char* bc, std::ostream& cout) {
		game_state* state = game_state::global_game_state();
		auto recipes_reg = state->recipe_registry();
		auto element_reg = state->element_registry();
		auto inventory = state->element_inventory();

		element_token a, b;
		element_state *a_state, *b_state;
		if (!element_reg->try_get_token(ac, &a) || !inventory->try_get_element_state(a, &a_state) || !a_state->can_use()) {
			cout << ac << " is an invalid element" << std::endl;
			return  0;
		}
		if (!element_reg->try_get_token(bc, &b) || !inventory->try_get_element_state(b, &b_state) || !b_state->can_use()) {
			cout << bc << " is an invalid element" << std::endl;
			return 0;
		}
		element_token out;
		if (inventory->mix(a, b, &out)) {
			auto& desc = element_reg->get_element_desc(out);
			cout << "mixed " << desc.name() << std::endl;
		}
		else {
			cout << "fizzled..." << std::endl;
		}

		return 0;
	}

	int mix_cmd(int argv, char** argc, std::istream& cin, std::ostream& cout) {
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
				<< "mix *[flags] <element one> <element two> - inline mix" << std::endl
				<< "mix *[flags] - standard in mix" << std::endl
				<< "flags:" << std::endl
				<< "--version | -v : prints version info about mix command" << std::endl
				<< "--help | -h : prints help info about mix command" << std::endl;
			return 0;
		}
		if (is_version_invoke) {
			cout
				<< "mix 1.0 - elements game mix command" << std::endl;
			return 0;
		}

		
		//two modes: list mode ; cmd line mode
		
		//inline mode
		if (argv > last_flag + 3) {
			return 10;
		}
		else if (argv == last_flag + 3) {
			return inline_mix(argc[last_flag+1], argc[last_flag + 2], cout);
		}
		else if (argv == last_flag + 1) {
			cout
				<< "mixing mode!" << std::endl 
				<< std::endl
				<< "to leave, type an empty line" << std::endl
				<< "to mix, type '<element one> <element two>'" << std::endl << std::endl;

			while (true) {
				std::string line;
				std::getline(cin, line);
				if (line.empty()) break;
				std::string a{};
				std::string b{};
				int i = 0;

				//get first word
				for (i = 0; i < line.size() && line[i] != ' '; i++);
				a = line.substr(0, i);

				//get start of second word
				for (; i < line.size() && line[i] == ' '; i++);
				int s = i;
				b = line.substr(s);

				int ret = inline_mix(a.c_str(), b.c_str(), cout);
			}
			return 0;
		}
		else {
			cout << "invalid invokation. Type 'mix --help' for more info." << std::endl;
			return 0;
		}
	}
}

#endif // ELEMENTS_UI_COMMANDS_MIX_CMD_H