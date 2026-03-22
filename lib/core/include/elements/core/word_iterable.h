#ifndef ELEMENTS_WORD_ITERABLE_H
#define ELEMENTS_WORD_ITERABLE_H

#include <string>
#include <string_view>
#include <cstddef>

namespace elements {
	class word_iterable {
	public:
		class word_iterator {
			using difference_type = std::ptrdiff_t;
			using value_type = std::string_view;
			using reference = value_type&;
			using iterator_category = std::forward_iterator_tag;
		private:

			std::string word;
			std::string::const_iterator str_start;
			std::string::const_iterator str_next;
			std::string::const_iterator str_end;
		public:
			word_iterator(const std::string::const_iterator& begin, const std::string::const_iterator& end) :
				str_next(begin),
				str_start(begin),
				str_end(end) {
				if (begin != end) {
					++(*this);
				}
			}

			static bool is_whitespace(char c) {
				return c == ' ' || c == '\t';
			}
			static bool is_quote(char c) {
				return c == '"';
			}

			// pre
			word_iterator& operator++() {
				//setup next frame
				str_start = str_next;
				auto start = str_start;
				auto iter = str_start;
				auto next = str_start;

				//find end of word
				if (iter != str_end && is_quote(*iter)) {
					start++;
					iter++;
					for (;
						iter != str_end && !is_quote(*iter);
						++iter) {
						if (*iter == '\\') ++iter;
					}
					next = iter;
					if (next != str_end) next++;
				}
				else {
					for (;
						iter != str_end && !is_whitespace(*iter) && !is_quote(*iter);
						++iter) {
						if (*iter == '\\') ++iter;
					}
					next = iter;
				}

				//save word
				word = "";
				for (auto i = start; i != iter; i++) {
					if (*i == '\\') {
						i++;
						switch (*i) {
						case 't':
							word += '\t';
							break;
						case 'n':
							word += '\n';
							break;
						case 'r':
							word += '\r';
							break;
						default:
							word += *i;
						}
					}
					else {
						word += *i;
					}
				}

				//find next available word
				for (;
					next != str_end && is_whitespace(*next);
					++next);

				str_next = next;

				return *this;
			}
			// post
			word_iterator operator++(int) {
				word_iterator o = *this;
				++(*this);
				return o;
			}
			bool operator==(const word_iterator& other) const {
				return str_start == other.str_start;
			}
			const std::string& operator*() {
				return word;
			}
		};

		using iterator = word_iterator;
	private:
		const std::string* str;
	public:

		word_iterable(const std::string& str) : str(&str) {}

		iterator begin() const {
			return iterator(str->cbegin(), str->cend());
		}
		iterator end() const {
			return iterator(str->cend(), str->cend());
		}
	};
}

#endif // ELEMENTS_WORD_ITERABLE_H
