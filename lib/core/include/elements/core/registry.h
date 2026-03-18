#ifndef ELEMENTS_CORE_REGISTRY_H
#define ELEMENTS_CORE_REGISTRY_H

#include <unordered_map>
#include <string>
#include <string_view>
#include <stdexcept>

namespace elements {
	template<typename T>
	class basic_registry {
		std::unordered_map<std::size_t, T> to_value_v{};
		std::unordered_map<T, std::size_t> from_value_v{};
		std::size_t next_key = 1;
	public:
		basic_registry() {}

		std::size_t emplace(const T& value) {
			std::size_t key = next_key++;
			to_value_v.emplace(value, key);
			from_value_v.emplace(key, value);
			return key;
		}

		T& to_value(std::size_t key) {
			auto k = to_value_v.find(key);
			if (k == to_value_v.end()) throw std::runtime_error("value of str does not exists in registry.");
			return *k;
		}

		const T& to_value(std::size_t key) const {
			auto k = to_value_v.find(key);
			if (k == to_value_v.end()) throw std::runtime_error("value of str does not exists in registry.");
			return *k;
		}
		std::size_t from_value(const T& value) {
			auto k = from_value_v.find(key);
			if (k == to_value_v.end()) throw std::runtime_error("value of str does not exists in registry.");
			return *k;
		}
	};
}

#endif // ELEMENTS_CORE_REGISTRY_H