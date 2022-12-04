#pragma once
#include "Relocation.h"

class BGSEquipSlot;
class AlchemyItem;


namespace EquipItem
{
    extern const uintptr_t Offset;
}

namespace EquipSlot
{
    typedef BGSEquipSlot* (*_GetLeftHandEquipSlot)();
    extern RelocAddr<_GetLeftHandEquipSlot> GetLeftHandEquipSlot;
}

namespace Alchemy
{
    typedef bool (*_IsPoison)(AlchemyItem*);
    extern RelocAddr<_IsPoison> IsPoison;
}