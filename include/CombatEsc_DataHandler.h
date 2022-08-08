#pragma once
#include "CombatEsc_Events.h"
#include "CombatEsc_Functinos.h"
#include "CombatEsc_LoadGameFunc.h"

namespace MaxsuCombatEscape
{
	class DataHandler;
	class DistanceData;

	class DistanceData
	{

	private:

		DistanceData();
		
		float		MaximumLoadedExteriorDistance;
		float		MaximumUnLoadedExteriorDistance;

	public:

		friend class DataHandler;

		float GetLoadedDistance() { return MaximumLoadedExteriorDistance; }
		float GetUnLoadedDistance() { return MaximumUnLoadedExteriorDistance; }
	};


	class DataHandler
	{

	public:

		static DataHandler* GetSingleton()
		{
			static DataHandler singleton;

			return &singleton;
		}

		friend void EventCallback(SKSE::MessagingInterface::Message* msg);

		friend bool ShouldInteriorEnemyStopCombat(RE::FormID TargetCellID);

		friend EventResult CellEventHandler::ProcessEvent(const RE::BGSActorCellEvent* a_event, RE::BSTEventSource<RE::BGSActorCellEvent>* a_eventSource);

	private:

		DataHandler();

		~DataHandler() = default;

		DataHandler(const DataHandler&) = delete;

		DataHandler(DataHandler&&) = delete;

		DataHandler& operator= (const DataHandler&) = delete;

		DataHandler& operator= (DataHandler&&) = delete;


	public:

		const float		InterDistanceMulti = 2.0f;
		DistanceData    distanceValue;


	private:

		std::optional<RE::FormID> LastPlayerLeaveCell;
	};

}