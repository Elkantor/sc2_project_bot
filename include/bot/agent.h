#pragma once

#include "bot/common.h"
#include "bot/scoring.h"

namespace sc2_bot {

	class Bot : public sc2::Agent {
	public:

		Bot(sc2_bot::Action& action_game_start)
			:_action_game_start(action_game_start)
		{};

		~Bot() {};

		sc2_bot::Action _action_game_start;

		virtual void OnGameStart() final {
			std::cout << "Hello, World!" << std::endl;
			std::cout << "score : " << _action_game_start.score << std::endl;
			_action_game_start.do_action(_action_game_start);
			std::cout << "score modified, after action is done: " << _action_game_start.score << std::endl;
			std::cin.get();
		}

		virtual void OnStep() final {
			/*std::cout << Observation()->GetGameLoop() << std::endl;*/
		}

		virtual void OnGameEnd() final {
			std::cout << "The game is over." << std::endl;
		}
	};
	


} // namespace sc2_bot