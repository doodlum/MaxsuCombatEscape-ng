#include "CombatEsc_LoadGameFunc.h"

void Init()
{
	auto g_message = SKSE::GetMessagingInterface();

	if (!g_message) {
		logger::error("Messaging Interface Not Found!");
	}

	if (g_message->RegisterListener(MaxsuCombatEscape::EventCallback)) {
		logger::info("Register Event Call Back!");
	}
}

void InitializeLog()
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		util::report_and_fail("Failed to find standard logging directory"sv);
	}

	*path /= fmt::format("{}.log"sv, Plugin::NAME);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

#ifndef NDEBUG
	const auto level = spdlog::level::trace;
#else
	const auto level = spdlog::level::info;
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
	log->set_level(level);
	log->flush_on(level);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%l] %v"s);
}

#ifdef MCE_NG  // alandtse CommonLibSSE-NG: SKSEPluginLoad() exports SKSEPlugin_Load
SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
#else
EXTERN_C [[maybe_unused]] __declspec(dllexport) bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
#endif
{
#ifndef NDEBUG
	while (!IsDebuggerPresent()) {};
#endif

	InitializeLog();

	logger::info("Loaded plugin");

	SKSE::Init(a_skse);

	Init();

	return true;
}

#ifndef MCE_NG  // the ng build (alandtse CommonLibSSE-NG) generates the SKSEPluginInfo block, see ng.cmake
EXTERN_C [[maybe_unused]] __declspec(dllexport) constinit auto SKSEPlugin_Version = []() noexcept {
	SKSE::PluginVersionData v;
	v.PluginName("PluginName");
	v.PluginVersion({ 1, 0, 0, 0 });
	v.UsesAddressLibrary();
	v.UsesUpdatedStructs();
	return v;
}();

EXTERN_C [[maybe_unused]] __declspec(dllexport) bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo* pluginInfo)
{
	pluginInfo->name = SKSEPlugin_Version.pluginName;
	pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
	pluginInfo->version = SKSEPlugin_Version.pluginVersion;
	return true;
}
#endif
