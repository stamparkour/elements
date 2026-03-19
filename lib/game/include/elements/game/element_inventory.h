#ifndef ELEMENTS_GAME_ELEMENT_INVENTORY_H
#define ELEMENTS_GAME_ELEMENT_INVENTORY_H

#include "element.h"
#include "recipe.h"
#include <unordered_map>
#include <initializer_list>


namespace elements {
	class element_state {
	public:
		int produced = 0;
		int current = 0;

		bool is_unlocked() const {
			return produced > 0;
		}
	};
	class element_inventory {
		std::unordered_map<element_token, element_state> states{};
		recipe_registry* recipe_r = nullptr;
		int craft_delta = 0; // dictates the change in 'current' for element_state when used to craft
	public:
		element_inventory(recipe_registry* recipe_r) : recipe_r(recipe_r) {}

		bool try_get_element_state(element_token token, element_state** out) {
			auto f = states.find(token);
			if (f == states.end()) return false;
			*out = &std::get<1>(*f);
		}
		element_state& get_element_state(element_token token) {
			return states[token];
		}

		void increment_element(element_token token) {
			element_state& s = get_element_state(token);
			s.produced++;
			s.current++;
		}
		void increment_element_list(std::initializer_list<element_token> elm) {
			for (const auto& e : elm) {
				increment_element(e);
			}
		}

		bool mix(element_token a, element_token b, element_token* out_v) {
			element_state* as;
			element_state* bs;
			if (!try_get_element_state(a, &as) || as->current == 0) return false;
			if (!try_get_element_state(b, &bs) || bs->current == 0) return false;

			element_token temp;
			element_token* out = out_v != nullptr ? out_v : &temp;
			if (!recipe_r->try_get_out(a, b, out)) return false;

			as->current += craft_delta;
			bs->current += craft_delta;

			increment_element(*out);
			return true;
		}

		auto& element_states() const {
			return states;
		}
		auto& element_states() {
			return states;
		}
	};
}

#endif // ELEMENTS_GAME_ELEMENT_INVENTORY_H