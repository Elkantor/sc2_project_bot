#pragma once

#include "bot/common.h"

namespace sc2_bot { namespace functions{

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
	bool TryBuildSCV() {
		return false;
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