#ifndef ELEMENTS_GAME_ELEMENTS_DESC_H
#define ELEMENTS_GAME_ELEMENTS_DESC_H

#include <string>
#include <string_view>
#include <cstddef>
#include <stdexcept>
#include <unordered_map>
#include <compare>

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
		friend class std::hash<element_token>;
		std::size_t id;
		element_token(std::size_t id) : id(id) {}
		element_token next() const {
			return element_token{id + 1};
		}
	public:
		friend std::strong_ordering operator <=>(const element_token& a, const element_token& b) {
			return a.id <=> b.id;
		}
	};
}

template<>
struct std::hash<elements::element_token> {
	std::size_t operator()(const elements::element_token& v) const noexcept {
		std::hash<std::size_t> hash_f{};
		return hash_f(v.id);
	}
};

namespace elements {
	class element_registry {
		std::unordered_map<element_token, element_desc> token_to_desc{};
		std::unordered_map<std::string, element_token> str_to_token{};
		element_token next_token{1};
	public:
		element_registry() {}

		void emplace(const std::string_view& str, const element_desc& desc) {
			str_to_token.emplace(std::string{str}, next_token);
			token_to_desc.emplace(next_token, desc);
			next_token = next_token.next();
		}
		element_token get_token(const std::string& str) const {
			auto p = str_to_token.find(str);
			if (p != str_to_token.end()) {
				throw std::runtime_error("value of str not a valid element in registry");
			}
			const auto& [str_out, token] = *p;
			return token;
		}
		const element_desc& get_desc(element_token token) const {
			auto p = token_to_desc.find(token);
			if (p != token_to_desc.end()) {
				throw std::runtime_error("value of str not a valid element in registry");
			}
			const auto& [token_out, desc] = *p;
			return desc;
		}
	};
}

#endif // ELEMENTS_GAME_ELEMENTS_DESC_H