#include "CombatEsc_Events.h"
#include "CombatEsc_DataHandler.h"
#include "CombatEsc_Functinos.h"

namespace MaxsuCombatEscape
{

	void CellEventHandler::RegisterSink()
	{
		auto PC = RE::PlayerCharacter::GetSingleton();

		if (!PC)
		{
			logger::error("Not Player Character handler Found!");
			return;
		}

		static CellEventHandler Cell_handler;

		PC->AsBGSActorCellEventSource()->AddEventSink(&Cell_handler);

		logger::info("Register Cell Event Sink!");
	}


	EventResult CellEventHandler::ProcessEvent(const RE::BGSActorCellEvent* a_event, RE::BSTEventSource<RE::BGSActorCellEvent>* a_eventSource)
	{
		if (!a_event || !a_eventSource)
		{
			logger::error("Event source NOT Found!");
			return EventResult::kContinue;
		}

		//logger::debug("Actor Cell Event Trigger!");

		auto PC = RE::PlayerCharacter::GetSingleton();

		if (!PC)
		{
			logger::error("Not Player Character handler Found!");
			return EventResult::kContinue;
		}

		auto playercell = RE::TESForm::LookupByID<RE::TESObjectCELL>(a_event->cellID);

		if (!playercell)
		{
			logger::error("Player Cell Not Found!");
			return EventResult::kContinue;
		}

		auto datahandler = DataHandler::GetSingleton();

		if (!datahandler)
		{
			logger::error("Not data handler Found!");
			return EventResult::kContinue;
		}

		if (a_event->flags == RE::BGSActorCellEvent::CellFlag::kEnter)
		{
			logger::debug(FMT_STRING("Enter a Cell Name {}, ID {:x}"), playercell->GetFormEditorID(), playercell->GetFormID());

			if (!PC->IsInCombat())
			{
				logger::debug("Player Character Not in Combat!");
				return EventResult::kContinue;
			}

			for (auto ActorHadnler : PC->GetPlayerRuntimeData().actorsToDisplayOnTheHUDArray)
			{
				auto target = ActorHadnler.get();

				if (!target || target->IsDisabled() || target->IsDeleted())
				{
					logger::debug("Hud Target Actor is Deleted or Disabled!");
					continue;
				}

				if (!target->IsInCombat() || !target->IsHostileToActor(PC))
				{
					logger::debug("Hud Target Actor is Not Combat Enemy of Player's!");
					continue;
				}

				logger::debug(FMT_STRING("Find a Enemy in Combat State! Name: {}, ID : {:x}"), target->GetName(), target->GetFormID());

				auto TargetCell = target->GetParentCell();

				if (!TargetCell)
				{
					logger::debug("Not Target Enemy Cell Found!");
					target->StopCombat();
					logger::debug(FMT_STRING("Stop a Enemy Combat State! Name: {}, ID : {:x}"), target->GetName(), target->GetFormID());
					continue;
				}

				bool StopCombat = false;

				if (TargetCell->IsExteriorCell() && ShouldExteriorEnemyStopCombat(PC, playercell->GetFormID(), target.get()))
				{
					logger::debug("Exterior Target Enemy Should Stop Combat!");
					StopCombat = true;
				}
				else if (TargetCell->IsInteriorCell() && ShouldInteriorEnemyStopCombat(TargetCell->GetFormID()))
				{
					logger::debug("Interior Target Enemy Should Stop Combat!");
					StopCombat = true;
				}

				if (StopCombat)
				{
					target->StopCombat();
					logger::debug(FMT_STRING("Stop a Enemy Combat State! Name: {}, ID : {:x}"), target->GetName(), target->GetFormID());
				}
			}
		}
		else if(a_event->flags == RE::BGSActorCellEvent::CellFlag::kLeave)
		{
			logger::debug(FMT_STRING("Leave a Cell Name {}, ID {:x}"), playercell->GetFormEditorID(), playercell->GetFormID());

			datahandler->LastPlayerLeaveCell.emplace(playercell->GetFormID());
			logger::debug("Record a Last Leave Cell!");
		}

		return EventResult::kContinue;
	}





};
