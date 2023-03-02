#pragma once


namespace MaxsuCombatEscape
{

	inline std::optional<const RE::NiPoint3> GetLastPlayerInterDoorAct(RE::PlayerCharacter* thisPC)
	{
		std::optional<const RE::NiPoint3> InterDoorPos;

		if (!thisPC)
		{
			logger::error("Not Player Found!");
			return InterDoorPos;
		}

		auto curpro = thisPC->GetActorRuntimeData().currentProcess;

		if (!curpro)
		{
			logger::error("Player Current Process Found!");
			return InterDoorPos;
		}

		if (curpro->high && curpro->high->lastExtDoorActivated.get())
		{
			auto lastExtDoor = curpro->high->lastExtDoorActivated.get();

			auto ExtraTeleport = lastExtDoor->extraList.GetByType<RE::ExtraTeleport>();

			if (ExtraTeleport && ExtraTeleport->teleportData)
			{
				InterDoorPos.emplace(ExtraTeleport->teleportData->position);
				logger::debug("Find the Position of Player Last Activate Interior Door!");
			}
		}

		return InterDoorPos;
	}


	bool ShouldExteriorEnemyStopCombat(RE::PlayerCharacter* thisPC, RE::FormID PCEnterCell, RE::Actor* TargetEnemy);

	bool ShouldInteriorEnemyStopCombat(RE::FormID TargetCellID);

}
