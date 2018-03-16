#pragma once

#include "bot/common.h"

namespace sc2_bot {
	
	enum ActionName {
		GAME_START,
		GAME_END,
		BUILD_SCV,
		BUILD_BARRACKS,
		MINE
	};

	struct Action {
		int score = -1;
		int score_modificator = -1;
		std::function<bool(
			Action& current_action, 
			std::map<sc2_bot::ActionName, sc2_bot::Action>& actions_available, 
			class Bot& bot
		)> do_action;
	};

	bool CheckActionExists(std::map<sc2_bot::ActionName, sc2_bot::Action>& actions_available, ActionName name) {
		bool action_exists = false;
		std::for_each(actions_available.begin(), actions_available.end(), [&name, &action_exists](const auto &pair) {
			if (name == pair.first) {
				action_exists = true;
			}
		});
		return action_exists;
	}
	

	bool StartNextAction(std::map<sc2_bot::ActionName, sc2_bot::Action>& actions_available, class Bot& bot) {
		int best_score = 0;
		ActionName action_to_launch;

		std::for_each(actions_available.begin(), actions_available.end(), [&](const auto &pair) {
			if (pair.second.score >= best_score) {
				best_score = pair.second.score;
				action_to_launch = pair.first;
			}
		});
		Action* action_selected = &actions_available.at(action_to_launch);
		action_selected->do_action(*action_selected, actions_available, bot);
		return true;
	}

} // namespace sc2_bot