#pragma once

#include "bot/common.h"
#include "bot/sc2_bot_scoring.h"

namespace sc2_bot {

	class Bot : public sc2::Agent {
	public:

		Bot(std::map<sc2_bot::ActionName, sc2_bot::Action>& actions_available)
			:actions_available_(actions_available)
		{};

		~Bot() {};

		// Properties
		int max_worker_count_ = 70;
		sc2::Point3D staging_location_;
		std::vector<sc2::Point3D> expansions_;
		std::map<sc2_bot::ActionName, sc2_bot::Action> actions_available_;
		std::vector<const sc2::Unit*> worker_building_structure_;
		int count_supply_depot_ = 0;


		// Methods
		virtual void OnGameStart() final {
			std::cout << "[SUCCESS] Game started !" << std::endl;
			Action* action = &actions_available_.find(sc2_bot::ActionName::GAME_START)->second;
			action->do_action(*action, actions_available_, *this);
			
		}

		virtual void OnStep() final {
			sc2_bot::StartNextAction(actions_available_, *this);
			std::cout << Observation()->GetMinerals() << std::endl;
		}

		virtual void OnGameEnd() final {
			std::cout << "The game is over." << std::endl;
			Action action = actions_available_.find(sc2_bot::ActionName::GAME_END)->second;
			action.do_action(action, actions_available_, *this);
		}
	};
	


} // namespace sc2_bot