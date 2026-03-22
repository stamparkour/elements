#ifndef ELEMENTS_UI_IO_H
#define ELEMENTS_UI_IO_H

#include "commands.h"
#include <iostream>
#include <string>

namespace elements {
	class game_io {
		std::istream* cin;
		std::ostream* cout;
		command_handler cmd;
	public:
		game_io(std::istream* cin, std::ostream* cout, bool is_dev = false) : cin(cin), cout(cout), cmd(default_commands(is_dev)) {}

		//blocking function for std::cin
		bool cmd_in() {
			*cout << "$ ";
			std::string line;
			std::getline(*cin, line);
			if (line.empty()) return true;
			if (line == "reload") return false;

			int ret_code = cmd.invoke(line, *cin, *cout);

			if (ret_code != 0) {
				std::cout << "return code: " << std::hex << ret_code << std::endl;
			}

			return true;
		}
	};
}

#endif // ELEMENTS_UI_IO_H