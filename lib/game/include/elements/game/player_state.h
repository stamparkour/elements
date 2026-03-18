#ifndef ELEMENTS_GAME_PLAYER_STATE_H
#define ELEMENTS_GAME_PLAYER_STATE_H

#include "element.h"
#include "recipe.h"
#include <vector>
#include <unordered_map>
#include <cstddef>

namespace elements {
	class element_state {
	public:
		int produced = 0;
		int current = 0;
	};
	class player_state {
		element_registry* element_r = nullptr;
		recipe_registry* recipe_r = nullptr;
		std::unordered_map<element_token, element_state> states{};
		int craft_delta = 0; // dictates the change in 'current' for element_state when used to craft
	public:
		player_state(element_registry* er, recipe_registry* rr) : element_r(er), recipe_r(rr) {}

		bool try_get_element_state(element_token token, element_state** out) {
			auto f = states.find(token);
			if (f == states.end()) return false;
			*out = &std::get<1>(*f);
		}
		bool try_get_element_state(element_token token, element_state const** out) const {
			auto f = states.find(token);
			if (f == states.end()) return false;
			if(out != nullptr) *out = &std::get<1>(*f);
			return true;
		}
		element_state& get_element_state(element_token token) {
			return states[token];
		}
		const element_state& get_element_state(element_token token) const {
			return states[token];
		}

		void increment_element(element_token token) {
			element_state& s = get_element_state(token);
			s.produced++;
			s.current++;
		}

		bool craft(element_token a, element_token b) {
			element_state* as;
			element_state* bs;
			if (!try_get_element_state(a, &as) || as->current == 0) return false;
			if (!try_get_element_state(b, &bs) || bs->current == 0) return false;

			element_token out;
			if (!recipe_r->try_get_out(a, b, &out)) return false;

			as->current += craft_delta;
			bs->current += craft_delta;

			increment_element(out);
			return true;
		}

		auto& element_states() const {
			return states;
		}
	};
}

#endif // ELEMENTS_GAME_PLAYER_STATE_H