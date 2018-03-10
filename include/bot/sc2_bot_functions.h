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
		return 0;
	}
	size_t CountUnitTypeTotal(const sc2::ObservationInterface* observation, sc2::UNIT_TYPEID unit_type, sc2::UNIT_TYPEID production, sc2::ABILITY_ID ability) {
		return 0;
	}

} // namespace functions
} // namespace sc2_bot