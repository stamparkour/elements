#ifndef ELEMENTS_GAME_RECIPE_H
#define ELEMENTS_GAME_RECIPE_H


#include <unordered_map>
#include <cstddef>
#include <initializer_list>
#include "element.h"

namespace elements {
	class recipe_in {
		friend class recipe_registry;
		friend class recipe_t;

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
	struct recipe_t {
		element_token a;
		element_token b;
		element_token out;

		recipe_in get_in() const {
			return {a,b};
		}
	};
	class recipe_registry {
		std::unordered_map<recipe_in, element_token> recipe_in_to_output{};
		std::unordered_map<element_token, std::vector<recipe_in>> output_to_recipe_ins{};
		std::unordered_map<element_token, std::vector<element_token>> token_to_recipe_complements{};
	public:
		recipe_registry() {}

		bool insert(element_token a, element_token b, element_token out) {
			recipe_in re{ a,b };
			// is unique recipe
			if (recipe_in_to_output.count(re)) return false;
			// now is guaranteed that (a,b) and (b,a) are not in set
			recipe_in_to_output.emplace(re, out);

			token_to_recipe_complements[a].push_back(b);
			token_to_recipe_complements[b].push_back(a);
			output_to_recipe_ins[out].push_back(re);

			return true;
		}
		void insert_list(std::initializer_list< std::tuple<element_token, element_token, element_token>> elm) {
			for (const auto& e : elm) {
				auto& [a, b, out] = e;
				insert(a,b,out);
			}
		}
		bool try_get_out(element_token a, element_token b, element_token* out) const {
			auto f = recipe_in_to_output.find(recipe_in{ a,b });
			if (f == recipe_in_to_output.end()) return false;
			if (out != nullptr) *out = std::get<1>(*f);
			return true;
		}
		bool try_get_in(element_token out, const std::vector<recipe_in>** out_vector) {
			auto f = output_to_recipe_ins.find(out);
			if (f == output_to_recipe_ins.end()) return false;
			if (out_vector != nullptr) *out_vector = &std::get<1>(*f);
			return true;
		}
		bool try_get_complements(element_token a, const std::vector<element_token>** out_vector) {
			auto f = token_to_recipe_complements.find(a);
			if (f == token_to_recipe_complements.end()) return false;
			if (out_vector != nullptr) *out_vector = &std::get<1>(*f);
			return true;
		}
	};
}

#endif // ELEMENTS_GAME_RECIPE_H