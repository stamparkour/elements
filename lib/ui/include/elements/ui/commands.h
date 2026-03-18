#ifndef ELEMENTS_UI_COMMANDS_H
#define ELEMENTS_UI_COMMANDS_H

#include <unordered_map>
#include <string>
#include "word_iterable.h"
#include <vector>

namespace elements {
	using command_type = int(*)(int, char**);

	class command_handler {
		std::unordered_map<std::string, command_type> commands;
	public:
		command_handler() {}
		void emplace(const std::string& name, command_type cmd) {
			commands.emplace(name, cmd);
		}
		int simple_invoke(const std::string& line) {
			std::vector<std::string> words{};
			std::vector<const char*> cwords{};
			for (auto w : word_iterable{ line }) {
				std::string& k = *words.emplace(w);
				cwords.emplace(k.c_str());
			}

			if (words.size() < 1) return 0x7E00;
			auto f = commands.find(words[0]);
			if (f == commands.end()) return 0x7E00;
			auto& [str_out, cmd] = *f;

			return cmd(cwords.size(), static_cast<char**>(cwords.data()));
		}
	};

	class default_commands {
		command_handler handler{};
	public:
		default_commands() {

		}

		command_handler& command_handler() {
			return handler;
		}
		const command_handler& command_handler() const {
			return handler;
		}
	};
}

#endif // ELEMENTS_UI_COMMANDS_H