#include "PapyrusAlternateAlchemy.h"
#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"
#include "GameReferences.h"
#include "PapyrusWornObject.h"

AlchemyItem* GetEquippedPoison(StaticFunctionTag*, Actor* Actor, UInt32 WeaponSlot)
{
    EquipData EquipData = referenceUtils::ResolveEquippedObject(Actor, WeaponSlot, 0);

    if (!EquipData.pForm || !EquipData.pExtraData)
    {
        return nullptr;
    }

    // Object must be a weapon to be poisoned
    if (EquipData.pForm && EquipData.pForm->formType == TESObjectWEAP::kTypeID)
    {
        ExtraPoison* PoisonData = static_cast<ExtraPoison*>(EquipData.pExtraData->GetByType(kExtraData_Poison));

        if (PoisonData)
        {
            return PoisonData->poison;
        }
    }

    return nullptr;
}

bool AlternateAlchemy::RegisterFunctions(VMClassRegistry* Registry)
{
    Registry->RegisterFunction(
        new NativeFunction2<StaticFunctionTag, AlchemyItem*, Actor*, UInt32>("GetEquippedPoison", "AlternateAlchemy", GetEquippedPoison, Registry));

    Registry->SetFunctionFlags("AlternateAlchemy", "GetEquippedPoison", VMClassRegistry::kFunctionFlag_NoWait);

    return true;
}
