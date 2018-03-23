#pragma once

#include "bot/common.h"
#include "bot/sc2_bot_agent.h"

namespace sc2_bot { 

	/**************/
	/* Structures */
	/**************/

	struct IsTownHall {
		bool operator()(const sc2::Unit& unit) {
			switch (unit.unit_type.ToType()) {
			case sc2::UNIT_TYPEID::ZERG_HATCHERY: return true;
			case sc2::UNIT_TYPEID::ZERG_LAIR: return true;
			case sc2::UNIT_TYPEID::ZERG_HIVE: return true;
			case sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER: return true;
			case sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND: return true;
			case sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMANDFLYING: return true;
			case sc2::UNIT_TYPEID::TERRAN_PLANETARYFORTRESS: return true;
			case sc2::UNIT_TYPEID::PROTOSS_NEXUS: return true;
			default: return false;
			}
		}
	};

	struct IsStructure {
		IsStructure(const sc2::ObservationInterface* obs) : observation_(obs) {};
		bool operator()(const sc2::Unit& unit) {
			auto& attributes = observation_->GetUnitTypeData().at(unit.unit_type).attributes;
			bool is_structure = false;
			for (const auto& attribute : attributes) {
				if (attribute == sc2::Attribute::Structure) {
					is_structure = true;
				}
			}
			return is_structure;
		}
		const sc2::ObservationInterface* observation_;
	};

namespace functions{

	/*******************************************/
	/* Toolbox functions to manage the actions */
	/*******************************************/

	sc2::Point2D GetRandomLocationNextUnit(const sc2_bot::Bot& bot, const sc2::Unit* unit) {
		float rx = sc2::GetRandomScalar();
		float ry = sc2::GetRandomScalar();
		sc2::Point2D location = sc2::Point2D(unit->pos.x + rx * 10, unit->pos.y + ry * 10);
		return location;
	}

	//An estimate of how many workers we should have based on what buildings we have
	int GetExpectedWorkers(sc2::UNIT_TYPEID vespene_building_type, const Bot& bot) {
		const sc2::ObservationInterface* observation = bot.Observation();
		sc2::Units bases = observation->GetUnits(sc2::Unit::Alliance::Self, IsTownHall());
		sc2::Units geysers = observation->GetUnits(sc2::Unit::Alliance::Self, sc2::IsUnit(vespene_building_type));
		int expected_workers = 0;
		for (const auto& base : bases) {
			if (base->build_progress != 1) {
				continue;
			}
			expected_workers += base->ideal_harvesters;
		}
		for (const auto& geyser : geysers) {
			if (geyser->vespene_contents > 0) {
				if (geyser->build_progress != 1) {
					continue;
				}
				expected_workers += geyser->ideal_harvesters;
			}
		}
		return expected_workers;
	}

	bool FindEnemyPosition(sc2::Point2D& target_pos) {
		return false;
	}

	bool GetRandomUnit(const sc2::Unit*& unit_out, const sc2::ObservationInterface* observation, sc2::UnitTypeID unit_type) {
		sc2::Units my_units = observation->GetUnits(sc2::Unit::Alliance::Self);
		std::random_shuffle(my_units.begin(), my_units.end()); // Doesn't work, or doesn't work well.
		for (const auto unit : my_units) {
			if (unit->unit_type == unit_type) {
				unit_out = unit;
				return true;
			}
		}
		return false;
	}

	const sc2::Unit* FindNearestMineralPatch(const sc2::Point2D& start, const sc2::ObservationInterface* observation) {
		sc2::Units units = observation->GetUnits(sc2::Unit::Alliance::Neutral);
		float distance = std::numeric_limits<float>::max();
		const sc2::Unit* target = nullptr;
		for (auto* u : units) {
			if (u->unit_type == sc2::UNIT_TYPEID::NEUTRAL_MINERALFIELD) {
				float d = DistanceSquared2D(u->pos, start);
				if (d < distance) {
					distance = d;
					target = u;
				}
			}
		}
		//If we never found one return nullptr
		if (distance == std::numeric_limits<float>::max()) {
			return target;
		}
		return target;
	}

	bool TryBuildStructure(sc2::AbilityID ability_type_for_structure, sc2::UnitTypeID unit_type, sc2_bot::Bot& bot, bool isExpansion = false) {
		const sc2::ObservationInterface* observation = bot.Observation();
		sc2::Units workers = observation->GetUnits(sc2::Unit::Alliance::Self, sc2::IsUnit(unit_type));
	
		sc2::Units command_centers = observation->GetUnits(sc2::Unit::Alliance::Self, sc2::IsUnit(sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER));
		sc2::Point2D build_location = GetRandomLocationNextUnit(bot, command_centers.at(0));

		//if we have no workers Don't build
		if (workers.empty()) {
			return false;
		}

		// Check to see if there is already a worker heading out to build it
		for (const auto& worker : workers) {
			for (const auto& order : worker->orders) {
				if (order.ability_id == ability_type_for_structure) {
					return false;
				}
			}
		}

		// If no worker is already building one, get a random worker to build one
		const sc2::Unit* unit = GetRandomEntry(workers);
		sc2::AvailableAbilities abilities = bot.Query()->GetAbilitiesForUnit(unit);
		
		float distance = std::numeric_limits<float>::max();
		float d = Distance2D(unit->pos, build_location);
		if (d < distance) {
			distance = d;
		}
		if (!isExpansion) {
			for (const auto& expansion : bot.expansions_) {
				if (Distance2D(build_location, sc2::Point2D(expansion.x, expansion.y)) < 7) {
					return false;
				}
			}
		}
		// Check to see if unit can build there
		bool build_ready = bot.Query()->Placement(ability_type_for_structure, build_location, unit);
		while (!build_ready) {
			build_location = GetRandomLocationNextUnit(bot, command_centers.at(0));
			build_ready = bot.Query()->Placement(ability_type_for_structure, build_location, unit);
		}
		if (build_ready) {
			bot.Actions()->UnitCommand(unit, ability_type_for_structure, build_location);
			//std::thread(sc2_bot::thread_events::BuildingSupplyDepot, bot, unit).detach();
			std::cout << "Adding unit to idle" << std::endl;
			bot.worker_Idle.push_back(unit);

			return true;
		}
		return false;
	}

		bool TryHarvest(sc2_bot::Bot& bot, const sc2::Unit* unit) {
			const sc2::Unit* mineral_patch = FindNearestMineralPatch(unit->pos, bot.Observation());
			if (!mineral_patch) {
				return false;
			}
			else {
				bot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::HARVEST_GATHER, mineral_patch);
				return true;
			}

		}

	bool TryBuildUnit(sc2::AbilityID ability_type_for_unit, sc2::UnitTypeID unit_type, sc2_bot::Bot& bot) {
		/*const sc2::ObservationInterface* observation = bot.Observation();
		sc2::Units bases = observation->GetUnits(sc2::Unit::Alliance::Self, sc2_bot::IsTownHall());
		
		for (const auto& base : bases) {
			if (base->unit_type == sc2::UNIT_TYPEID::TERRAN_BARRACKS && base->energy > 50) {
				bot.Actions()->UnitCommand(base, sc2::ABILITY_ID::TRAIN_MARINE);
			}
		}*/
		return false;
	}

	bool TryBuildSCV(sc2_bot::Bot& bot) {
		const sc2::ObservationInterface* observation = bot.Observation();

		sc2::Units bases = observation->GetUnits(sc2::Unit::Alliance::Self, sc2_bot::IsTownHall());

		for (const auto& base : bases) {
			if (base->unit_type == sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND && base->energy > 50) {
				if (FindNearestMineralPatch(base->pos, bot.Observation())) {
					bot.Actions()->UnitCommand(base, sc2::ABILITY_ID::EFFECT_CALLDOWNMULE);
				}
			}
		}

		if (observation->GetFoodWorkers() >= bot.max_worker_count_) {
			return false;
		}

		if (observation->GetFoodUsed() >= observation->GetFoodCap()) {
			return false;
		}

		if (observation->GetFoodWorkers() > GetExpectedWorkers(sc2::UNIT_TYPEID::TERRAN_REFINERY, bot)) {
			return false;
		}

		for (const auto& base : bases) {
			//if there is a base with less than ideal workers
			if (base->assigned_harvesters < base->ideal_harvesters && base->build_progress == 1) {
				if (observation->GetMinerals() >= 50) {
					return TryBuildUnit(sc2::ABILITY_ID::TRAIN_SCV, base->unit_type,bot);
				}
			}
		}
		return false;;
	}

	bool TryBuildMarine(sc2::AbilityID ability_type_for_structure, sc2::UnitTypeID unit_type,sc2_bot::Bot& bot) {
		sc2::Units barracks = bot.Observation()->GetUnits(sc2::Unit::Alliance::Self, sc2::IsUnit(sc2::UNIT_TYPEID::TERRAN_BARRACKS));
		if (barracks[0]->orders.size() >= 5) {
			return false;
		}
		bot.Actions()->UnitCommand(barracks[0], ability_type_for_structure);

		return true;
	}

	bool TryAttack(sc2::Units army, sc2_bot::Bot& bot) {
		const sc2::GameInfo& game_info = bot.Observation()->GetGameInfo();
		std::vector<sc2::Point2D> enemy_base_locations = game_info.enemy_start_locations;
		for (sc2::Point2D enemy_base : enemy_base_locations) {
			for (const sc2::Unit* unit : army) {
				bot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::ATTACK_ATTACK, enemy_base);
			}
		}
		return true;
	}

	size_t CountUnitType(const sc2::ObservationInterface* observation, sc2::UnitTypeID unit_type) {
		return 0;
	}

	size_t CountUnitTypeBuilding(const sc2::ObservationInterface* observation, sc2::UNIT_TYPEID production_building, sc2::ABILITY_ID ability) {
		int building_count = 0;
		sc2::Units buildings = observation->GetUnits(sc2::Unit::Self, sc2::IsUnit(production_building));

		for (const auto& building : buildings) {
			if (building->orders.empty()) {
				continue;
			}

			for (const auto order : building->orders) {
				if (order.ability_id == ability) {
					building_count++;
				}
			}
		}

		return building_count;
	}

	size_t CountUnitTypeTotal(const sc2::ObservationInterface* observation, sc2::UNIT_TYPEID unit_type, sc2::UNIT_TYPEID production, sc2::ABILITY_ID ability) {
		return 0;
	}

	bool CheckUnitAvailable(const sc2::Unit* unit) {
		if (unit->orders.size() == 0) {
			return true;
		}
		else {
			return false;
		}
	}

} // namespace functions
} // namespace sc2_bot