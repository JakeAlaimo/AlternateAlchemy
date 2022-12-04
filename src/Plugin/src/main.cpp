#include "PluginAPI.h"
#include "skse_version.h"
#include "GameAPI.h"
#include "PatchPotionEquipBehavior.h"
#include "PapyrusAlternateAlchemy.h"

PluginHandle	g_PluginHandle = kPluginHandle_Invalid;

extern "C"
{
#if defined AE_BUILD
	__declspec(dllexport) SKSEPluginVersionData SKSEPlugin_Version =
	{
		SKSEPluginVersionData::kVersion,
		1,
		"AlternateAlchemy",
		"Jake Alaimo",
		"",
#if !defined V16353_BUILD
		0,
#endif
		0,
#if defined GOG_BUILD
		{ RUNTIME_VERSION_1_6_659_GOG, 0 },
#elif defined V16640_BUILD
		{ RUNTIME_VERSION_1_6_640, 0 },
#elif defined V16353_BUILD
		{ RUNTIME_VERSION_1_6_353, 0 },
#endif 
		0,
	};

#else
	__declspec(dllexport) bool SKSEPlugin_Query(const SKSEInterface* skse, PluginInfo* info)
	{
		// populate info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "AlternateAlchemy";
		info->version = 1;

		// store plugin handle so we can identify ourselves later
		g_PluginHandle = skse->GetPluginHandle();

		UINT32 TargetRuntime = 0;
#ifdef LE_BUILD
		TargetRuntime = RUNTIME_VERSION_1_9_32_0;
#else
		TargetRuntime = RUNTIME_VERSION_1_5_97;
		info->name = "AlternateAlchemy";
#endif

		if (skse->isEditor)
		{
			return false;
		}
		else if (skse->runtimeVersion != TargetRuntime)
		{
			return false;
		}

		// ### do not do anything else in this callback
		// ### only fill out PluginInfo and return true/false

		// supported runtime version
		return true;
	}
#endif

	__declspec(dllexport) bool SKSEPlugin_Load(const SKSEInterface* skse)
	{
#ifdef AE_BUILD
		// in the case of AE, plugin handle and interfaces are not grabbed yet as SKSEPlugin_Query never fires
		g_PluginHandle = skse->GetPluginHandle();
#endif // AE_BUILD

		AlternateAlchemy::PatchPotionEquip();

		SKSEPapyrusInterface* PapyrusInterface = (SKSEPapyrusInterface*)skse->QueryInterface(kInterface_Papyrus);
		PapyrusInterface->Register(AlternateAlchemy::RegisterFunctions);

		return true;
	}
};
