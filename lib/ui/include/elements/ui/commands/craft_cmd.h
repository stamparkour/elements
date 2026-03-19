#ifndef ELEMENTS_UI_COMMANDS_CRAFT_CMD_H
#define ELEMENTS_UI_COMMANDS_CRAFT_CMD_H

#include <elements/game.h>
#include <string>
#include <iostream>

namespace elements {
	int craft_cmd(int argv, char** argc) {
		game_state* state = game_state::global_game_state();
		auto recipes_reg = state->recipe_registry();
		auto element_reg = state->element_registry();
		auto inventory = state->element_inventory();
		
		//two modes: list mode ; cmd line mode
		
		if (argv != 3) return 1;

		element_token a, b;
		if (!element_reg->try_get_token(argc[1], &a)) return 2;
		if (!element_reg->try_get_token(argc[2], &b)) return 3;
		element_token out;
		if (inventory->craft(a, b, &out)) {
			auto& desc = element_reg->get_element_desc(out);
			std::cout << "crafted " << desc.name() << std::endl;
		}
		else {
			std::cout << "fizzled..." << std::endl;
		}

		return 0;
	}
}

#endif // ELEMENTS_UI_COMMANDS_CRAFT_CMD_H