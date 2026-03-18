#ifndef ELEMENTS_GAME_ELEMENT_H
#define ELEMENTS_GAME_ELEMENT_H

#include <string>
#include <string_view>
#include <cstddef>
#include <stdexcept>
#include <unordered_map>
#include <compare>
#include <vector>

namespace elements {
	class element_desc {
		friend class element_registry;
		std::string name_v;
		element_desc(const std::string_view& str) {
			this->name_v = str;
		}
	public:
		const std::string& name() const {
			return name_v;
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
	public:
		element_registry() {
			token_to_desc.emplace_back(); // sets size to 1, next open index is 1
		}

		void emplace(const std::string_view& str, const element_desc& desc) {
			str_to_token.emplace(std::string{str}, next_token);
			token_to_desc.emplace_back(desc);
			next_token = next_token.next();
		}
		element_token to_token(const std::string& str) const {
			auto p = str_to_token.find(str);
			if (p != str_to_token.end()) {
				throw std::runtime_error("value of str not a valid element in registry");
			}
			const auto& [str_out, token] = *p;
			return token;
		}
		const element_desc& element_desc(element_token token) const {
			if (token.id() <= 0 || token.id() > token_to_desc.size()) {
				throw std::runtime_error("value of token not a valid element in registry");
			}
			return token_to_desc[token.id()];
		}
		std::size_t total_element_ids() {
			return next_token.id();
		}
	};
}

#endif // ELEMENTS_GAME_ELEMENT_H