#include "CombatEsc_LoadGameFunc.h"
#include "CombatEsc_DataHandler.h"

namespace MaxsuCombatEscape
{

	void EventCallback(SKSE::MessagingInterface::Message* msg)
	{
		if (msg->type == SKSE::MessagingInterface::kDataLoaded)
		{
			logger::info("Data Load CallBack Trigger!");

			auto IsEnableDebugMessage = GetPrivateProfileIntA("Main", "EnableDebugMessage", 0, SETTINGFILE_PATH);

			if (IsEnableDebugMessage == 1)
			{
				spdlog::set_level(spdlog::level::debug);
				logger::info("Enable Debug Message!");
			}

			DataHandler::GetSingleton();
		}
	}

}