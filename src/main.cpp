#include "bot/common.h"
#include "bot/sc2_bot_agent.h"
#include "bot/sc2_bot_scoring.h"
#include "bot/sc2_bot_actions.h"


int main(int argc, char* argv[]) {
    sc2::Coordinator coordinator;
    coordinator.LoadSettings(argc, argv);

	// Create the map of the available actions
	std::map<sc2_bot::ActionName, sc2_bot::Action> actions_available;
	
	if(!sc2_bot::CheckActionExists(actions_available, sc2_bot::ActionName::GAME_START)) {
		int score = 100;
		int score_modificator = -100;
		actions_available.insert(std::pair<sc2_bot::ActionName, sc2_bot::Action>(
			sc2_bot::ActionName::GAME_START, sc2_bot::Action{ score, score_modificator, sc2_bot::actions::function_start, std::map<sc2_bot::ActionName, int>() }
		));
	}

	if (!sc2_bot::CheckActionExists(actions_available, sc2_bot::ActionName::BUILD_SCV)) {
		int score = 90;
		int score_modificator = -20;
		actions_available.insert(std::pair<sc2_bot::ActionName, sc2_bot::Action>(
			sc2_bot::ActionName::BUILD_SCV, sc2_bot::Action{ score, score_modificator, sc2_bot::actions::function_buildsvc, std::map<sc2_bot::ActionName, int>() }
		));
	}

	// Insert a map of actions to be modified when the actions are called.
	// The first parameter is the ActionName of the action, to find it, and the second is the score modificator.
	actions_available.begin()->second.actions_impacted.insert(
		std::pair<sc2_bot::ActionName, int>(sc2_bot::ActionName::BUILD_SCV, -10)
	);

	// Create the AI Agent (the global manager)
	sc2_bot::Bot bot(actions_available);

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