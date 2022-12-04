#pragma once

class TESForm;
class TESObjectREFR;
class BGSEquipSlot;

namespace AlternateAlchemy
{
    void PatchPotionEquip();

    extern "C" void TogglePoisonFlagIfAlternateEquipped(TESObjectREFR* Equipper, TESForm* EquippedItem, BGSEquipSlot* EquippedSlot);

    extern "C" UInt32 OverrideFormID;

    extern "C" bool OverridePoisonState;
}

extern "C" void CallPoisonFlagToggleOnAltEquip();

extern "C" bool IsPoisonOverride();