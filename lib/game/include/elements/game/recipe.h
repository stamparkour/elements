#ifndef ELEMENTS_GAME_RECIPE_H
#define ELEMENTS_GAME_RECIPE_H


#include <unordered_map>
#include <cstddef>
#include <initializer_list>
#include "element.h"

namespace elements {
	class recipe_in {
		friend class recipe_registry;
		element_token a_v; // smaller
		element_token b_v; // larger

		recipe_in(element_token a, element_token b) : a_v(a), b_v(b) {
			if (b_v < a_v) std::swap(a_v, b_v);
		}
	public:
		element_token a() const {
			return a_v;
		}
		element_token b() const {
			return b_v;
		}

		friend bool operator ==(const recipe_in& a, const recipe_in& b) {
			return a.a() == b.a() && a.b() == b.b();
		}
		friend std::strong_ordering operator <=>(const recipe_in& a, const recipe_in& b) {
			auto ac = a.a() <=> b.a();
			if (ac != std::strong_ordering::equal) return ac;
			return a.b() <=> b.b();
		}
		std::size_t hash() const {
			std::hash<element_token> hash_f{};
			std::size_t ah = hash_f(a());
			std::size_t bh = hash_f(b());
			return (ah << 31) ^ bh;
		}
	};
}

template<>
struct std::hash<elements::recipe_in> {
	std::size_t operator()(const elements::recipe_in& v) const noexcept {
		return v.hash();
	}
};

namespace elements {
	class recipe_t {

	};
	class recipe_registry {
		std::unordered_map<recipe_in, element_token> recipe_in_to_output{};
	public:
		recipe_registry() {}

		void insert(const std::tuple<element_token, element_token, element_token>& v) {
			const auto& [a, b, out] = v;
			recipe_in_to_output.emplace(recipe_in{ a,b }, out);
		}
		void insert_list(std::initializer_list< std::tuple<element_token, element_token, element_token>> elm) {
			for (const auto& e : elm) {
				insert(e);
			}
		}
		bool try_get_out(element_token a, element_token b, element_token* out) const {
			auto f = recipe_in_to_output.find(recipe_in{ a,b });
			if (f == recipe_in_to_output.end()) return false;
			if (out != nullptr) *out = std::get<1>(*f);
			return true;
		}
	};
}

#endif // ELEMENTS_GAME_RECIPE_H