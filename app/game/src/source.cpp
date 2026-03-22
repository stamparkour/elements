#include <iostream>
#include <fstream>
#include <elements/game/game_state.h>
#include <elements/ui/io.h>
#include <elements/serialize/elements_config.h>
#include <filesystem>
#include <elements/elements_config.h>

using namespace elements;

int main(int argc, char** argv) {
	while (true) {
		std::cout
			<< "Elements " << ELEMENTS_VERSION << std::endl
			<< "  - Type 'howto' to learn how to play." << std::endl
			<< "  - Type 'help' for a list of commands." << std::endl;


		game_state state{};
		game_io io{&std::cin, &std::cout, true}; // is_dev = true

		if (!std::filesystem::exists("elements_config.txt")) {
			std::cout << "elements_config.txt does not exist. terminating...";
			return 1;
		}

		std::ifstream elements_config_file{"elements_config.txt"};
		parse_elements_config(elements_config_file);

		state.element_inventory()->reset();

		while (io.cmd_in());
	}

	return 0;
}