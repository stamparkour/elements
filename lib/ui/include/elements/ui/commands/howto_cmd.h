#ifndef ELEMENTS_UI_COMMANDS_HOWTO_CMD_H
#define ELEMENTS_UI_COMMANDS_HOWTO_CMD_H

#include <string>
#include <iostream>
#include <elements/elements_config.h>
#include <cstring>

namespace elements {
	int howto_cmd(int argv, char** argc, std::istream& cin, std::ostream& cout) {
		bool is_help_invoke = false;
		bool is_version_invoke = false;
		for (int i = 1; i < argv; i++) {
			if (!std::strcmp("--help", argc[i]) || !std::strcmp("-h", argc[i])) {
				is_help_invoke = true;
				break;
			}
			else if (!std::strcmp("--version", argc[i]) || !std::strcmp("-v", argc[i])) {
				is_version_invoke = true;
				break;
			}
		}
		if (is_help_invoke) {
			cout
				<< "howto *[flags]" << std::endl
				<< "flags:" << std::endl
				<< "--version | -v : prints version info about howto command" << std::endl
				<< "--help | -h : prints help info about howto command" << std::endl;
			return 0;
		}
		if (is_version_invoke) {
			cout
				<< "howto 1.0 - elements game howto command" << std::endl;
			return 0;
		}

		cout
			<< "Elements " << ELEMENTS_VERSION << std::endl
			<< "In this game, you mix elements into new elements and try to unlock all elements." << std::endl
			<< std::endl
			<< "  - Type the command 'list' to see your elements." << std::endl
			<< "  - Type the command 'mix' to mix elements and enter mix mode; or" << std::endl
			<< "  - Type 'mix <element one> <element two> to do an inline mix." << std::endl
			<< std::endl;

		return 0;
	}
}

#endif // ELEMENTS_UI_COMMANDS_HOWTO_CMD_H