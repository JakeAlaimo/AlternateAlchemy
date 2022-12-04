#include "Offsets.h"
#include "GameForms.h"

namespace EquipItem
{
    const uintptr_t Offset = RelocationManager::s_baseAddr + 0x638710;
}

namespace EquipSlot
{
    RelocAddr<_GetLeftHandEquipSlot> GetLeftHandEquipSlot(0x3315F0);
}

namespace Alchemy
{
    RelocAddr<_IsPoison> IsPoison(0xF7130);
}