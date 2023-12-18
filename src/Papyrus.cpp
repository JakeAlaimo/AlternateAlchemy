#include "Papyrus.h"
#include "PCH.h"

RE::AlchemyItem* GetEquippedPoison(RE::StaticFunctionTag*, RE::Actor* Actor, uint32_t WeaponSlot)
{
    RE::InventoryEntryData* EquippedData = Actor->GetEquippedEntryData(WeaponSlot);

    if (!EquippedData || !EquippedData->extraLists)
    {
        return nullptr;
    }

    for (const auto& ExtraList : *EquippedData->extraLists)
    {
        RE::BSExtraData* Data = (ExtraList) ? ExtraList->GetByType(RE::ExtraDataType::kPoison) : nullptr;
        if (RE::ExtraPoison* PoisonData = static_cast<RE::ExtraPoison*>(Data))
        {
            return PoisonData->poison;
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