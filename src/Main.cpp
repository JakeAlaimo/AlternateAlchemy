#include <SKSE/Interfaces.h>
#include <SKSE/API.h>
#include "Papyrus.h"
#include "Patches.h"

using namespace SKSE;

namespace 
{
    SKSEPluginLoad(const LoadInterface* SKSE)
    {
        Init(SKSE);

        if (!AlternateAlchemy::PatchItemEquipBehavior())
        {
            return false;
        }

        if (auto* PapyrusInterface = GetPapyrusInterface())
        {
            return PapyrusInterface->Register(AlternateAlchemy::RegisterFunctions);
        } 

        return false;
    }
}