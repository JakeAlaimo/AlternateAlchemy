#include "Papyrus.h"
#include "PCH.h"

RE::AlchemyItem* GetEquippedPoison(RE::StaticFunctionTag*, RE::Actor* Actor, uint32_t WeaponSlot)
{
    if (!Actor)
    {
        return nullptr;
    }

    RE::InventoryEntryData* EquippedData = Actor->GetEquippedEntryData(WeaponSlot);
    if (!EquippedData || EquippedData->extraLists == nullptr)
    {
        return nullptr;
    }

    for (const RE::ExtraDataList* ExtraList : *EquippedData->extraLists)
    {
        const RE::BSExtraData* Data = (ExtraList) ? ExtraList->GetByType(RE::ExtraDataType::kPoison) : nullptr;
        if (const RE::ExtraPoison* PoisonData = static_cast<const RE::ExtraPoison*>(Data))
        {
            return (PoisonData && PoisonData->GetType() == RE::ExtraDataType::kPoison) ? PoisonData->poison : nullptr;
        }
    }

    return nullptr;
}

bool AlternateAlchemy::RegisterFunctions(RE::BSScript::IVirtualMachine* VM)
{
    if (!VM)
    {
        return false;
    }

    VM->RegisterFunction("GetEquippedPoison", "AlternateAlchemy", GetEquippedPoison);
    return true;
}