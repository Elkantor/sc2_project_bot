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
		std::vector<const sc2::Unit*> worker_Idle;
		int count_supply_depot_ = 0;


		// Methods
		virtual void OnGameStart() final {
			std::cout << "[SUCCESS] Game started !" << std::endl;
			Action* action = &actions_available_.find(sc2_bot::ActionName::GAME_START)->second;
			action->do_action(*action, actions_available_, *this);
			
		}

		virtual void OnStep() final {
			sc2_bot::StartNextAction(actions_available_, *this);
		}


		virtual void OnBuildingConstructionComplete(const sc2::Unit* unit) final {
			switch (unit->unit_type.ToType()) {
			case sc2::UNIT_TYPEID::TERRAN_BARRACKS : {
					Action& action_marine = actions_available_.at(sc2_bot::ActionName::BUILD_MARINE);
					action_marine.score += 50;
					break;
				}
			}
			Action& action_reset_worker = actions_available_.at(sc2_bot::ActionName::RESERT_WORKER);
			action_reset_worker.score += action_reset_worker.score_modificator;
		}
		
		virtual void OnGameEnd() final {
			std::cout << "The game is over." << std::endl;
			Action action = actions_available_.find(sc2_bot::ActionName::GAME_END)->second;
			action.do_action(action, actions_available_, *this);
		}
	};
	


} // namespace sc2_bot