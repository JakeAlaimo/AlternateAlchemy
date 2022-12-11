#pragma once

class TESForm;
class TESObjectREFR;
class BGSEquipSlot;

namespace AlternateAlchemy
{
    void PatchPotionEquip();

    extern "C" void TogglePoisonFlagIfAlternateEquipped(TESObjectREFR* Equipper, TESForm* EquippedItem, BGSEquipSlot* EquippedSlot);
}

extern "C" void CallPoisonFlagToggleOnAltEquip();