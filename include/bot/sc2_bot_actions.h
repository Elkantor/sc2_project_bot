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

	auto function_mine = [](
		Action& current_action,
		std::map<sc2_bot::ActionName, sc2_bot::Action>& actions_available,
		class Bot& bot
		) {
		current_action.score--;
		return true;
	};

	auto function_build_barracks = [](
		Action& current_action,
		std::map<sc2_bot::ActionName, sc2_bot::Action>& actions_available,
		class Bot& bot
		) {
		Action& action_mine = actions_available.at(sc2_bot::ActionName::MINE);
		// Prerequisite 
		if (bot.count_supply_depot < 1) {
			action_mine.score += action_mine.score_modificator;
			return true;
		}
		// Action
		if (sc2_bot::functions::TryBuildStructure(sc2::ABILITY_ID::BUILD_BARRACKS, sc2::UNIT_TYPEID::TERRAN_SCV, bot)) {
			action_mine.score += action_mine.score_modificator;
			current_action.score -= current_action.score_modificator;
		}
		std::cout << "Building a barracks..." << std::endl;

		return true;
	};

	auto function_build_supply_depot = [](
		Action& current_action,
		std::map<sc2_bot::ActionName, sc2_bot::Action>& actions_available,
		class Bot& bot
		) {
		Action& action_mine = actions_available.at(sc2_bot::ActionName::MINE);
		// Prerequisite
		if (bot.Observation()->GetMinerals() <= 120) {
			action_mine.score += action_mine.score_modificator;
			return true;
		}
		// Action
		if (sc2_bot::functions::TryBuildStructure(sc2::ABILITY_ID::BUILD_SUPPLYDEPOT, sc2::UNIT_TYPEID::TERRAN_SCV, bot)) {
			action_mine.score += action_mine.score_modificator;
			current_action.score -= current_action.score_modificator;
		}
		std::cout << "Building a supply depot..." << std::endl;

		return true;
	};

} // namespace actions
} // namespace sc2_bot