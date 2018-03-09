#pragma once

#include "bot/common.h"

namespace sc2_bot {

	struct Action {
		const char* name;
		uint32_t score = 0;
		std::function<bool(Action& current_action)> do_action;
	};

	Action CreateAction(
		const std::vector<Action>& actions_available, 
		const char* name, 
		const uint32_t& score,
		const std::function<bool(Action& current_action)>& function
	) {
		
		for (const Action& action : actions_available) {
			if (action.name == name) {
				return Action{};
			}
		}

		return Action{
			name,
			score,
			function
		};
	}

} // namespace sc2_bot