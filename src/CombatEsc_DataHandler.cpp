#include "CombatEsc_DataHandler.h"
#include "CombatEsc_LoadGameFunc.h"

namespace MaxsuCombatEscape
{
	const float DefaultLoadedDistance = 9500.f;
	const float DefaultUnLoadedDistance = 5000.f;

	DataHandler::DataHandler()
	{
		CellEventHandler::RegisterSink();
	}



	DistanceData::DistanceData()
	{
		float LoadedDistance = GetFloatFromINI("Distance", "MaximumLoadedExteriorDistance", DefaultLoadedDistance);

		if (LoadedDistance < 3500.f || LoadedDistance > 20000.f)
		{
			logger::error("MaximumLoadedExteriorDistance Readed Value Wrong! Set to Default");
			LoadedDistance = DefaultLoadedDistance;
		}

		float UnLoadedDistance = GetFloatFromINI("Distance", "MaximumUnLoadedExteriorDistance", DefaultUnLoadedDistance);

		if (UnLoadedDistance < 1500.f || UnLoadedDistance > 10000.f)
		{
			logger::error("MaximumUnLoadedExteriorDistance Readed Value Wrong! Set to Default");
			UnLoadedDistance = DefaultUnLoadedDistance;
		}

		MaximumLoadedExteriorDistance = LoadedDistance;
		MaximumUnLoadedExteriorDistance = UnLoadedDistance;

		logger::info(FMT_STRING("Get MaximumLoadedExteriorDistance Value is {}"), MaximumLoadedExteriorDistance);
		logger::info(FMT_STRING("Get MaximumUnLoadedExteriorDistance Value is {}"), MaximumUnLoadedExteriorDistance);
	}


}