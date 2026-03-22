#ifndef ELEMENTS_SERIALIZE_ELEMENTS_CONFIG_H
#define ELEMENTS_SERIALIZE_ELEMENTS_CONFIG_H

#include <elements/game/element.h>
#include <elements/game/recipe.h>
#include <elements/game/game_state.h>
#include <elements/core/word_iterable.h>
#include <iostream>
#include <string>
#include <vector>

namespace elements {
	void parse_elements_config(std::istream& cin) {
		game_state* state = game_state::global_game_state();
		auto& recipe_reg = *state->recipe_registry();
		auto& element_reg = *state->element_registry();
		auto& inventory = *state->element_inventory();

		while (cin) {
			std::string line;
			std::getline(cin, line);
			if (line.empty() || line[0] == '#') continue;

			std::vector<std::string> words{};
			for (const auto& w : word_iterable(line)) {
				words.push_back(std::string{ w });
			}

			if (words.size() == 1) {
				if(!element_reg.try_get_token(words[0], nullptr)) element_reg.insert({words[0], 1});
			}
			else if (words.size() == 3) {
				element_token out;
				if (!element_reg.try_get_token(words[2], &out)) out = element_reg.insert({ words[2] });

				element_token a = element_reg.get_token(words[0]);
				element_token b = element_reg.get_token(words[1]);

				recipe_reg.insert(a, b, out);
			}
		}
	}
}

#endif // ELEMENTS_SERIALIZE_ELEMENTS_CONFIG_H