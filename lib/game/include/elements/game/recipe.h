#ifndef ELEMENTS_GAME_RECIPE_H
#define ELEMENTS_GAME_RECIPE_H

#include <string>
#include <string_view>
#include <cstddef>
#include "element_desc.h"

namespace elements {
	class recipe_token {
		element_token a_v; // smaller
		element_token b_v; // larger
		std::size_t hash;

		recipe_token(element_token a, element_token b) : a_v(a), b_v(b) {
			if (b_v < a_v) std::swap(a_v, b_v);
			std::hash<element_token> hash_f{};
			std::size_t ah = hash_f(a);
			std::size_t bh = hash_f(b);
			hash = (ah << 31) ^ bh;
		}
	public:
		element_token a() const {
			return a_v;
		}
		element_token b() const {
			return b_v;
		}

		friend std::strong_ordering operator <=>(const recipe_token& a, const recipe_token& b) {
			auto ac = a.a() <=> b.a();
			if (ac != std::strong_ordering::equal) return ac;
			return a.b() <=> b.b();
		}
	};
	class recipe_desc {
		recipe_token recipe_v;
		element_token out_v;
	public:
	};
}

#endif // ELEMENTS_GAME_RECIPE_H