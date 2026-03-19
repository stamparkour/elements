#ifndef ELEMENTS_GAME_ELEMENT_H
#define ELEMENTS_GAME_ELEMENT_H

#include <string>
#include <string_view>
#include <cstddef>
#include <stdexcept>
#include <unordered_map>
#include <compare>
#include <vector>
#include <initializer_list>
#include <concepts>

namespace elements {
	class element_desc {
		friend class element_registry;
		std::string name_v;
		int initial_count_v = 0;
		element_desc(const std::string_view& str, int initial_count = 0) {
			this->name_v = str;
			this->initial_count_v = initial_count;
		}
	public:
		element_desc() {}
		const std::string& name() const {
			return name_v;
		}
		int initial_count() const {
			return initial_count_v;
		}
	};
	class element_token {
		friend class element_registry;
		std::size_t id_v;
		element_token(std::size_t id) : id_v(id) {}
		element_token next() const {
			return element_token{ id_v + 1};
		}
	public:
		element_token() : id_v(0) {}
		std::size_t id() const {
			return id_v;
		}
		friend bool operator ==(const element_token& a, nullptr_t) {
			return a.id() == 0;
		}
		friend bool operator ==(const element_token& a, const element_token& b) {
			return a.id_v == b.id_v;
		}
		friend std::strong_ordering operator <=>(const element_token& a, const element_token& b) {
			return a.id_v <=> b.id_v;
		}
		std::size_t hash() const {
			std::hash<std::size_t> hash_f{};
			return hash_f(id_v);
		}
	};
}

template<>
struct std::hash<elements::element_token> {
	std::size_t operator()(const elements::element_token& v) const noexcept {
		return v.hash();
	}
};

namespace elements {

	class element_registry {
		std::vector<element_desc> token_to_desc{};
		std::unordered_map<std::string, element_token> str_to_token{};
		element_token next_token{ 1 };
		std::vector<element_token> init_elements{};
	public:
		element_registry() {
			token_to_desc.emplace_back(); // sets size to 1, next open index is 1
		}

		element_token insert(const element_desc& desc) {
			if (str_to_token.count(desc.name())) {
				return element_token{};
			}
			auto tok = next_token;
			str_to_token.emplace(desc.name(), tok);
			token_to_desc.push_back(desc);
			next_token = next_token.next();

			if (desc.initial_count() != 0) {
				init_elements.push_back(tok);
			}

			return tok;
		}
		void insert_list(std::initializer_list<element_desc> elm) {
			for (const auto& e : elm) {
				insert(e);
			}
		}
		element_token get_token(const std::string& str) const {
			auto p = str_to_token.find(str);
			if (p == str_to_token.end()) {
				throw std::runtime_error("value of str not a valid element in registry");
			}
			const auto& [str_out, token] = *p;
			return token;
		}
		bool try_get_token(const std::string& str, element_token* out) const {
			auto p = str_to_token.find(str);
			if (p == str_to_token.end()) return false;
			const auto& [str_out, token] = *p;
			if(out != nullptr) *out = token;
			return true;
		}
		const element_desc& get_element_desc(element_token token) const {
			if (token.id() <= 0 || token.id() > token_to_desc.size()) {
				throw std::runtime_error("value of token not a valid element in registry");
			}
			return token_to_desc[token.id()];
		}
		std::size_t total_element_ids() {
			return next_token.id();
		}

		const std::vector<element_token>& initial_elements() const {
			return init_elements;
		}
	};
}

#endif // ELEMENTS_GAME_ELEMENT_H