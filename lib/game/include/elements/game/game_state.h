#ifndef ELEMENTS_GAME_GAME_STATE_H
#define ELEMENTS_GAME_GAME_STATE_H

#include "element.h"
#include "recipe.h"
#include "element_inventory.h"
#include <vector>
#include <unordered_map>
#include <cstddef>
#include <stdexcept>

namespace elements {
	
	class game_state {
		static inline game_state* global_game_state_v = nullptr;
		elements::element_registry element_r;
		elements::recipe_registry recipe_r;
		elements::element_inventory inventory_v;
	public:
		game_state() :
			element_r(),
			recipe_r(),
			inventory_v(&element_r, &recipe_r)
		{
			if (global_game_state_v != nullptr) throw new std::runtime_error("global_game_state_v already exists");
			global_game_state_v = this;
		}
		game_state(const game_state&) = delete;
		game_state operator=(const game_state&) = delete;
		~game_state() {
			if(global_game_state_v == this) global_game_state_v = nullptr;
		}

		static game_state* global_game_state() {
			return global_game_state_v;
		}
		elements::element_registry* element_registry() {
			return &element_r;
		}
		elements::recipe_registry* recipe_registry() {
			return &recipe_r;
		}
		elements::element_inventory* element_inventory() {
			return &inventory_v;
		}
	};
}

#endif // ELEMENTS_GAME_GAME_STATE_H