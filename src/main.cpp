#include "bot/common.h"
#include "bot/sc2_bot_agent.h"
#include "bot/sc2_bot_scoring.h"
#include "bot/sc2_bot_actions.h"


int main(int argc, char* argv[]) {
    sc2::Coordinator coordinator;
    coordinator.LoadSettings(argc, argv);

	/*******************************************/
	/* Create the map of the available actions */
	/*******************************************/
	std::map<sc2_bot::ActionName, sc2_bot::Action> actions_available;

	
	if(!sc2_bot::CheckActionExists(actions_available, sc2_bot::ActionName::GAME_START)) {
		int score = 100;
		int score_modificator = 100;
		actions_available.insert(std::pair<sc2_bot::ActionName, sc2_bot::Action>(
			sc2_bot::ActionName::GAME_START, 
			sc2_bot::Action { 
				score, 
				score_modificator, 
				sc2_bot::actions::function_start
			}
		));
	}

	if (!sc2_bot::CheckActionExists(actions_available, sc2_bot::ActionName::MINE)) {
		int score = 90;
		int score_modificator = 5;
		actions_available.insert(std::pair<sc2_bot::ActionName, sc2_bot::Action>(
			sc2_bot::ActionName::MINE, 
			sc2_bot::Action { 
				score, 
				score_modificator, 
				sc2_bot::actions::function_mine
			}
		));
	}

	if (!sc2_bot::CheckActionExists(actions_available, sc2_bot::ActionName::BUILD_SUPPLY_DEPOT)) {
		int score = 51;
		int score_modificator = 5;
		actions_available.insert(std::pair<sc2_bot::ActionName, sc2_bot::Action>(
			sc2_bot::ActionName::BUILD_SUPPLY_DEPOT,
			sc2_bot::Action{
				score,
				score_modificator,
				sc2_bot::actions::function_build_supply_depot
			}
		));
	}

	if (!sc2_bot::CheckActionExists(actions_available, sc2_bot::ActionName::BUILD_BARRACKS)) {
		int score = 50;
		int score_modificator = 5;
		actions_available.insert(std::pair<sc2_bot::ActionName, sc2_bot::Action>(
			sc2_bot::ActionName::BUILD_BARRACKS, 
			sc2_bot::Action { 
				score, 
				score_modificator, 
				sc2_bot::actions::function_build_barracks
			}
		));
	}
	

	/********************************************/
	/* Create the AI Bot Agent (the global manager) */
	/********************************************/
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