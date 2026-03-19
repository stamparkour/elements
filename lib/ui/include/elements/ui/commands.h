#ifndef ELEMENTS_UI_COMMANDS_H
#define ELEMENTS_UI_COMMANDS_H

#include "command_handler.h"
#include "commands/mix_cmd.h"
#include "commands/list_cmd.h"

namespace elements {

	command_handler default_commands() {
		command_handler handler_v{};
		handler_v.emplace("list", list_cmd);
		handler_v.emplace("mix", mix_cmd);
		return handler_v;
	}
}

#endif // ELEMENTS_UI_COMMANDS_H