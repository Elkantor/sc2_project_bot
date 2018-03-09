#pragma once

#include "bot/common.h"
#include "bot/scoring.h"
#include "bot/agent.h"

int main(int argc, char* argv[]) {
    sc2::Coordinator coordinator;
    coordinator.LoadSettings(argc, argv);

	std::vector<sc2_bot::Action> actions_available;

	auto function_start = [](sc2_bot::Action& current_action) {
		current_action.score += 10;
		return true;
	};
	

	sc2_bot::Bot bot(
		sc2_bot::CreateAction(actions_available, "game_start", 100, function_start)
	);
	//bot._action_game_start = sc2_bot::CreateAction(actions_available, "game_start", 100, function_start);
	{
		if (bot._action_game_start.name != "") {
			std::cout << "[SUCCESS] The action is correctly created." << std::endl;
		}
		else {
			std::cout <<
				"[ERROR] Action is not created, becase a action with the same name already exists in the available actions list."
				<< std::endl;
		}
	}

    coordinator.SetParticipants({
        sc2::CreateParticipant(sc2::Race::Terran, &bot),
        sc2::CreateComputer(sc2::Race::Zerg)
    });

    coordinator.LaunchStarcraft();
    coordinator.StartGame(sc2::kMapBelShirVestigeLE);

    while (coordinator.Update()) {
    }

    return 0;
}