#pragma once

#include "bot/common.h"
#include "bot/sc2_bot_agent.h"

namespace sc2_bot { 

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
	
namespace functions{

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

	const sc2::Unit* FindNearestMineralPatch(const sc2::Point2D& start, const Bot& bot) {
		sc2::Units units = bot.Observation()->GetUnits(sc2::Unit::Alliance::Neutral);
		float distance = std::numeric_limits<float>::max();
		const sc2::Unit* target = nullptr;
		for (const auto& u : units) {
			if (u->unit_type == sc2::UNIT_TYPEID::NEUTRAL_MINERALFIELD) {
				float d = DistanceSquared2D(u->pos, start);
				if (d < distance) {
					distance = d;
					target = u;
				}
			}
		}
		//If we never found one return false;
		if (distance == std::numeric_limits<float>::max()) {
			return target;
		}
		return target;
	}

	bool FindEnemyPosition(sc2::Point2D& target_pos) {
		return false;
	}

	bool TryBuildStructure(sc2::AbilityID ability_type_for_structure, sc2::UnitTypeID unit_type = sc2::UNIT_TYPEID::TERRAN_SCV) {
		return false;
	}

	bool TryBuildSupplyDepot() {
		return false;
	}

	bool TryBuildBarracks() {
		return false;
	}

	bool TryBuildUnit(sc2::AbilityID ability_type_for_unit, sc2::UnitTypeID unit_type) {
		return false;
	}

	bool TryBuildSCV(sc2_bot::Bot& bot) {
		const sc2::ObservationInterface* observation = bot.Observation();

		sc2::Units bases = observation->GetUnits(sc2::Unit::Alliance::Self, sc2_bot::IsTownHall());

		for (const auto& base : bases) {
			if (base->unit_type == sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND && base->energy > 50) {
				if (FindNearestMineralPatch(base->pos, bot)) {
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
					return TryBuildUnit(sc2::ABILITY_ID::TRAIN_SCV, base->unit_type);
				}
			}
		}
		return false;;
	}

	bool TryBuildMarine() {
		return false;
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

	const sc2::Unit* FindNearestMineralPatch(const sc2::Point2D& start, const sc2::ObservationInterface* observation) {
		sc2::Units units = observation->GetUnits(sc2::Unit::Alliance::Neutral);
		float distance = std::numeric_limits<float>::max();
		const sc2::Unit* target = nullptr;
		for (const auto& u : units) {
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

} // namespace functions
} // namespace sc2_bot