#ifndef ELEMENTS_GAME_ELEMENT_INVENTORY_H
#define ELEMENTS_GAME_ELEMENT_INVENTORY_H

#include "element.h"
#include "recipe.h"
#include <unordered_map>
#include <initializer_list>
#include <stdexcept>


namespace elements {
	class element_state {
	public:
		bool is_unlimited_v = false;
		int produced = 0;
		int current = 0;

		bool is_unlimited() const {
			return is_unlimited_v;
		}
		bool is_unlocked() const {
			return is_unlimited() || produced > 0;
		}
		bool can_use() const {
			return current > 0;
		}
	};
	class element_inventory {
		std::unordered_map<element_token, element_state> states{};
		recipe_registry* recipe_r = nullptr;
		element_registry* element_r = nullptr;
		int craft_delta = 0; // dictates the change in 'current' for element_state when used to craft
	public:
		element_inventory(element_registry* element_r, recipe_registry* recipe_r) : element_r(element_r), recipe_r(recipe_r) {}

		bool try_get_element_state(element_token token, element_state** out) {
			auto f = states.find(token);
			if (f == states.end()) return false;
			else {
				*out = &std::get<1>(*f);
				return true;
			}
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
			if (!try_get_element_state(a, &as) || !as->can_use()) return false;
			if (!try_get_element_state(b, &bs) || !bs->can_use()) return false;

			element_token temp;
			element_token* out = out_v != nullptr ? out_v : &temp;
			if (!recipe_r->try_get_out(a, b, out)) return false;

			if (!as->is_unlimited()) as->current += craft_delta;
			if (!bs->is_unlimited()) bs->current += craft_delta;

			increment_element(*out);
			return true;
		}

		auto& element_states() const {
			return states;
		}
		auto& element_states() {
			return states;
		}

		void reset() {
			states.clear();
			for (auto e : element_r->initial_elements()) {
				auto& desc = element_r->get_element_desc(e);
				element_state state{};
				state.is_unlimited_v = true;
				state.current = desc.initial_count();
				states.emplace(e, state);
			}
		}
	};
}

#endif // ELEMENTS_GAME_ELEMENT_INVENTORY_H