#pragma once

namespace MaxsuCombatEscape
{
	class DataHandler;

	using EventResult = RE::BSEventNotifyControl;

	class CellEventHandler : public RE::BSTEventSink<RE::BGSActorCellEvent>
	{

	public:
		 
		virtual EventResult ProcessEvent(const RE::BGSActorCellEvent* a_event, RE::BSTEventSource<RE::BGSActorCellEvent>* a_eventSource);

		static void RegisterSink();


	private:

		CellEventHandler() = default;
		~CellEventHandler() = default;
	};


}