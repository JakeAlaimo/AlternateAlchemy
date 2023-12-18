#pragma once
#include "PCH.h"
#include "REL/Relocation.h"

namespace AlternateAlchemyHooks
{
    typedef void(*EquipItem)(uint64_t Unk1, RE::Actor* Equipper, RE::TESForm** EquippedItem, RE::BGSEquipSlot* Slot, uint8_t Unk2);
    typedef RE::BGSEquipSlot*(*GetEquipSlot)();

    REL::Relocation<GetEquipSlot> GetLeftHandEquipSlotHook() noexcept
    {
        if (REL::Module::IsVR()) // No relocation support for these addresses in VR yet, need to RE further down the line
        {
            return {};
        }

        return REL::Relocation<GetEquipSlot>(RELOCATION_ID(23150, 23607));
    }

    REL::Relocation<EquipItem> GetInventoryMenuEquipHook() noexcept
    {
        if (REL::Module::IsVR()) // No relocation support for these addresses in VR yet, need to RE further down the line
        {
            return {};
        }

        return REL::Relocation<EquipItem>(RELOCATION_ID(50991, 51870), (REL::Module::IsAE()) ? 0x85 : 0x8F);
    }

    REL::Relocation<EquipItem> GetFavoritesMenuEquipHook() noexcept
    {
        if (REL::Module::IsVR()) // No relocation support for these addresses in VR yet, need to RE further down the line
        {
            return {};
        }

        return REL::Relocation<EquipItem>(RELOCATION_ID(50654, 51548), (REL::Module::IsAE()) ? 0xC2 : 0xC4);
    }
}