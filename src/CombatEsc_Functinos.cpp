#include "CombatEsc_Functinos.h"
#include "CombatEsc_DataHandler.h"

namespace MaxsuCombatEscape
{
	bool ShouldExteriorEnemyStopCombat(RE::PlayerCharacter* thisPC, RE::FormID PCEnterCell, RE::Actor* TargetEnemy)
	{
		if (!thisPC || !TargetEnemy)
		{
			logger::error("Not Player or Target Npc Found!");
			return false;
		}

		auto PlayerCell = RE::TESForm::LookupByID<RE::TESObjectCELL>(PCEnterCell);
		auto TargetCell = TargetEnemy->GetParentCell();

		if (!PlayerCell || !TargetCell)
		{
			logger::error("Not Player or Target Npc Parent Cell Found!");
			return false;
		}

		auto datahandler = DataHandler::GetSingleton();

		if (!datahandler)
		{
			logger::error("Not data handler Found!");
			return false;
		}

		if (PlayerCell->IsExteriorCell() && TargetCell->IsExteriorCell())
		{
			auto result = thisPC->GetPosition().GetDistance(TargetEnemy->GetPosition());

			if (result >= 0)
			{
				logger::debug(FMT_STRING("Exterior Player Get Distance of Exterior Enemy: {}"), result);

				if (result >= datahandler->distanceValue.GetLoadedDistance() && TargetEnemy->GetProcessLevel() == RE::PROCESS_TYPE::kHigh)
					return true;
				else if (result >= datahandler->distanceValue.GetUnLoadedDistance() && TargetEnemy->GetProcessLevel() != RE::PROCESS_TYPE::kHigh)
					return true;
			}
			else
				logger::warn("Fail to Get Distance Value of Exterior Player to Exterior Enemy!");
		}
		else if (PlayerCell->IsInteriorCell() && TargetCell->IsExteriorCell())
		{
			auto result = thisPC->exteriorPosition.GetDistance(TargetEnemy->GetPosition());		//Get Distance Bewteen Player's Last Recorded Exterior Position To Exterior Enemy's Position

			auto LastInterDoorPos = GetLastPlayerInterDoorAct(thisPC);

			if (LastInterDoorPos.has_value())
			{
				auto InterDistance = thisPC->GetPosition().GetDistance(LastInterDoorPos.value()) * datahandler->InterDistanceMulti;	 //Add the Distance Between Player's Interior Position to Player's Last Activate Interior Door Position.
				logger::debug(FMT_STRING("Get Player Interior Distance: {}"), InterDistance);
				result += InterDistance;
			}

			logger::debug(FMT_STRING("Interior Player Get Distance of Exterior Enemy: {}"), result);

			if (result >= datahandler->distanceValue.GetLoadedDistance() && TargetEnemy->GetProcessLevel() == RE::PROCESS_TYPE::kHigh)
				return true;
			else if (result >= datahandler->distanceValue.GetUnLoadedDistance() && TargetEnemy->GetProcessLevel() != RE::PROCESS_TYPE::kHigh)
				return true;
		}

		return false;
	}


	bool ShouldInteriorEnemyStopCombat(RE::FormID TargetCellID)
	{
		auto TargetCell = RE::TESForm::LookupByID<RE::TESObjectCELL>(TargetCellID);

		if (!TargetCell)
		{
			logger::error("Not Target Cell Found!");
			return false;
		}

		auto datahandler = DataHandler::GetSingleton();

		if (!datahandler)
		{
			logger::error("Not data handler Found!");
			return false;
		}

		if (TargetCell->IsInteriorCell() && !TargetCell->IsAttached())
		{
			if (datahandler->LastPlayerLeaveCell.has_value() && datahandler->LastPlayerLeaveCell.value() == TargetCell->GetFormID())
				logger::debug("Interior Target is In Player Last Leave Cell!");
			else
				return true;
		}
		else
		{
			logger::debug("Interior Target is In Current Attached Cell!");
		}

		return false;
	}

}