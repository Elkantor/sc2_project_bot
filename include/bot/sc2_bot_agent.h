#pragma once

#include "bot/common.h"
#include "bot/sc2_bot_scoring.h"

namespace sc2_bot {

	class Bot : public sc2::Agent {
	public:

		Bot(std::map<sc2_bot::ActionName, sc2_bot::Action>& actions_available)
			:_actions_available(actions_available)
		{};

		~Bot() {};

		// Properties
		int max_worker_count_ = 70;
		std::map<sc2_bot::ActionName, sc2_bot::Action> _actions_available;

		// Methods
		virtual void OnGameStart() final {
			std::cout << "[SUCCESS] Game started !" << std::endl;
			Action* action = &_actions_available.find(sc2_bot::ActionName::GAME_START)->second;
			std::map<ActionName, int>* actions_impacted = &action->actions_impacted;
			action->do_action(*action, *actions_impacted, _actions_available, *this);
			
		}

		virtual void OnStep() final {
			sc2_bot::StartNextAction(_actions_available, *this);
		}

		virtual void OnGameEnd() final {
			std::cout << "The game is over." << std::endl;
			Action action = _actions_available.find(sc2_bot::ActionName::GAME_END)->second;
			action.do_action(action, action.actions_impacted, _actions_available, *this);
		}
	};
	


} // namespace sc2_bot