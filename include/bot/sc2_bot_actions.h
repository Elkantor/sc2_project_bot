#pragma once

#include "bot/sc2_bot_scoring.h"
#include "bot/sc2_bot_agent.h"
#include "bot/sc2_bot_functions.h"

namespace sc2_bot { namespace actions {

	// Functions associated to actions
	auto function_start = [](
		Action& current_action,
		std::map<sc2_bot::ActionName, sc2_bot::Action>& actions_available,
		class Bot& bot
		) {
		current_action.score -= current_action.score_modificator;
		return true;
	};

	auto function_end = [](
		Action& current_action,
		std::map<sc2_bot::ActionName, sc2_bot::Action>& actions_available,
		class Bot& bot
		) {
		current_action.score -= current_action.score_modificator;
		return true;
	};

	auto function_buildsvc = [](
		Action& current_action,
		std::map<sc2_bot::ActionName, sc2_bot::Action>& actions_available,
		class Bot& bot
		) {
		//current_action.score += current_action.score_modificator;

		/*if (sc2_bot::functions::TryBuildSCV(bot)) {
			std::cout << "[SUCCESS] A SCV is created !" << std::endl;
		};*/

		return true;
	};

	auto function_build_marine = [](
		Action& current_action,
		std::map<sc2_bot::ActionName, sc2_bot::Action>& actions_available,
		class Bot& bot
		) {
			// Prerequisite

		Action& action_supply = actions_available.at(sc2_bot::ActionName::BUILD_SUPPLY_DEPOT);
	

		if (bot.Observation()->GetFoodUsed() + 1 >= bot.Observation()->GetFoodCap()) {
			action_supply.score += action_supply.score_modificator;
			current_action.score -= current_action.score_modificator;
			return false;
		}
		if (bot.Observation()->GetUnits(sc2::Unit::Alliance::Self, sc2::IsUnit(sc2::UNIT_TYPEID::TERRAN_BARRACKS))[0]->orders.size() >= 4) {
			current_action.score -= current_action.score_modificator;
			action_supply.score += action_supply.score_modificator;
			return false;
		}


		if (sc2_bot::functions::TryBuildMarine(sc2::ABILITY_ID::TRAIN_MARINE, sc2::UNIT_TYPEID::TERRAN_BARRACKS, bot)) {
				current_action.score -= current_action.score_modificator;
				action_supply.score += action_supply.score_modificator;
				std::cout << "Building a Marine" << std::endl;
				return true;
		}

		return true;
	};

	auto function_reset_worker = [](
		Action& current_action,
		std::map<sc2_bot::ActionName, sc2_bot::Action>& actions_available,
		class Bot& bot
		) {
		// Prerequisite


		if (!bot.worker_idle_.empty()) {
			for (const sc2::Unit* unit : bot.worker_idle_) {
				functions::TryHarvest(bot, unit);
			}
			bot.worker_idle_.clear();
		}

		std::cout << "Reseting workers" << std::endl;
		current_action.score -= current_action.score_modificator;
		return true;
	};

	auto function_mine = [](
		Action& current_action,
		std::map<sc2_bot::ActionName, sc2_bot::Action>& actions_available,
		class Bot& bot
		) {
		// Action
		current_action.score--;
		return true;
	};

	auto function_build_barracks = [](
		Action& current_action,
		std::map<sc2_bot::ActionName, sc2_bot::Action>& actions_available,
		class Bot& bot
			) {
			if (bot.Observation()->GetUnits(sc2::Unit::Alliance::Self, sc2::IsUnit(sc2::UNIT_TYPEID::TERRAN_BARRACKS)).size()<1) {
				Action& action_marine = actions_available.at(sc2_bot::ActionName::BUILD_MARINE);
				Action& action_mine = actions_available.at(sc2_bot::ActionName::MINE);
				// Prerequisite 
				int number_supply_depot = bot.Observation()->GetUnits(sc2::Unit::Alliance::Self, sc2::IsUnit(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT)).size();
				if (number_supply_depot == 0) {
					action_mine.score += action_mine.score_modificator;
					return true;
				}
				// Action
				if (sc2_bot::functions::TryBuildStructure(sc2::ABILITY_ID::BUILD_BARRACKS, sc2::UNIT_TYPEID::TERRAN_SCV, bot)) {
					action_mine.score += action_mine.score_modificator;
					current_action.score -= current_action.score_modificator;
					std::cout << "Building a barracks..." << std::endl;
					return true;
				}
			}
			else {
				current_action.score = 0;
			}
		};

	auto function_build_supply_depot = [](
		Action& current_action,
		std::map<sc2_bot::ActionName, sc2_bot::Action>& actions_available,
		class Bot& bot
		) {
		Action& action_mine = actions_available.at(sc2_bot::ActionName::MINE);
		Action& action_train_marine = actions_available.at(sc2_bot::ActionName::BUILD_MARINE);
		// Prerequisite
		if (bot.Observation()->GetMinerals() <= 120) {
			action_mine.score += action_mine.score_modificator;
			return true;
		}
		if (bot.Observation()->GetUnits(sc2::Unit::Alliance::Self, sc2::IsUnit(sc2::UNIT_TYPEID::TERRAN_BARRACKS)).size()>=1) {
			if (bot.Observation()->GetUnits(sc2::Unit::Alliance::Self, sc2::IsUnit(sc2::UNIT_TYPEID::TERRAN_BARRACKS))[0]->orders.size() < 4) {
				action_train_marine.score += action_train_marine.score_modificator;
				current_action.score -= current_action.score_modificator;
			}
		}
		// Action
		if (sc2_bot::functions::TryBuildStructure(sc2::ABILITY_ID::BUILD_SUPPLYDEPOT, sc2::UNIT_TYPEID::TERRAN_SCV, bot)) {
			action_mine.score += action_mine.score_modificator;
			current_action.score -= current_action.score_modificator;
			std::cout << "Building a supply depot..." << std::endl;
			return true;
		}
		
	};

	auto function_attack = [](
		Action& current_action,
		std::map<sc2_bot::ActionName, sc2_bot::Action>& actions_available,
		class Bot& bot
		) {
		
		// Prerequisite 
		sc2::Units army = bot.Observation()->GetUnits(sc2::Unit::Alliance::Self, sc2::IsUnit(sc2::UNIT_TYPEID::TERRAN_MARINE));
		if (army.size() < 10) {
			return true;
		}
		// Action
		if (sc2_bot::functions::TryAttack(&army, bot)) {
			current_action.score -= current_action.score_modificator;
		}
		std::cout << "Attack !" << std::endl;
		return true;
	};


} // namespace actions
} // namespace sc2_bot