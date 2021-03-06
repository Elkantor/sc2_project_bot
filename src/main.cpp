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
		int score = 60;
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
		int score = 55;
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

	if (!sc2_bot::CheckActionExists(actions_available, sc2_bot::ActionName::ATTACK)) {
		int score = 10;
		int score_modificator = 100;
		actions_available.insert(std::pair<sc2_bot::ActionName, sc2_bot::Action>(
			sc2_bot::ActionName::ATTACK,
			sc2_bot::Action{
				score,
				score_modificator,
				sc2_bot::actions::function_attack
			}
		));
	}
	
	if (!sc2_bot::CheckActionExists(actions_available, sc2_bot::ActionName::RESERT_WORKER)) {
		int score = 5;
		int score_modificator = 95;
		actions_available.insert(std::pair<sc2_bot::ActionName, sc2_bot::Action>(
			sc2_bot::ActionName::RESERT_WORKER,
			sc2_bot::Action{
				score,
				score_modificator,
				sc2_bot::actions::function_reset_worker
			}
		));
	}

	if (!sc2_bot::CheckActionExists(actions_available, sc2_bot::ActionName::BUILD_MARINE)) {
		int score = 15;
		int score_modificator = 10;
		actions_available.insert(std::pair<sc2_bot::ActionName, sc2_bot::Action>(
			sc2_bot::ActionName::BUILD_MARINE,
			sc2_bot::Action { 
				score, 
				score_modificator, 
				sc2_bot::actions::function_build_marine
			}
		));
	}
	
	

	/************************************************/
	/* Create the AI Bot Agent (the global manager) */
	/************************************************/
	sc2_bot::Bot bot(actions_available);

    coordinator.SetParticipants({
        sc2::CreateParticipant(sc2::Race::Terran, &bot),
        sc2::CreateComputer(sc2::Race::Zerg)
    });

    coordinator.LaunchStarcraft();
    coordinator.StartGame(sc2::kMapBelShirVestigeLE);


    while (coordinator.Update()) {}

    return 0;
}