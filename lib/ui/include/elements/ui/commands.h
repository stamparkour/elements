#ifndef ELEMENTS_UI_COMMANDS_H
#define ELEMENTS_UI_COMMANDS_H

#include "command_handler.h"
#include "commands/craft_cmd.h"
#include "commands/list_cmd.h"

namespace elements {

	class default_commands {
		command_handler handler_v{};
	public:
		default_commands() {
			handler_v.emplace("list", list_cmd);
			handler_v.emplace("craft", craft_cmd);
		}

		command_handler& handler() {
			return handler_v;
		}
	};
}

#endif // ELEMENTS_UI_COMMANDS_H