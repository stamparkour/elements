#ifndef ELEMENTS_UI_COMMANDS_H
#define ELEMENTS_UI_COMMANDS_H

#include "command_handler.h"
#include "commands/mix_cmd.h"
#include "commands/list_cmd.h"
#include "commands/listall_cmd.h"
#include "commands/howto_cmd.h"
#include "commands/hint_cmd.h"
#include "commands/combo_cmd.h"

namespace elements {

	command_handler default_commands(bool is_dev = false) {
		command_handler handler_v{};
		handler_v.emplace("list", list_cmd);
		handler_v.emplace("listall", listall_cmd);
		handler_v.emplace("mix", mix_cmd);
		handler_v.emplace("howto", howto_cmd);
		handler_v.emplace("hint", hint_cmd);
		if (is_dev) {
			handler_v.emplace("combo", combo_cmd);
		}
		return handler_v;
	}
}

#endif // ELEMENTS_UI_COMMANDS_H