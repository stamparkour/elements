#include <iostream>
#include <elements/game/game_state.h>
#include <elements/ui/io.h>

using namespace elements;

int main(int argc, char** argv) {
	std::cout << "Hello World!" << std::endl;

	game_state state{};
	game_io io{&std::cin, &std::cout};

	state.element_registry().insert_list({
		"fire",
		"earth",
		"water",
		"air",
		"rain"
	});
	auto tok = [&](auto t) { return state.element_registry().get_token(t); };
	state.recipe_registry().insert_list({
		{ tok("water"), tok("air"), tok("rain")}
	});
	state.element_inventory().increment_element_list({
		tok("water"),
		tok("air")
	});

	while (io.cmd_in());

	return 0;
}