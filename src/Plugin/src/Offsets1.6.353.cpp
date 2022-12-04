#include "Offsets.h"
#include "GameForms.h"

namespace EquipItem
{
    const uintptr_t Offset = RelocationManager::s_baseAddr + 0x65F2A0;
}

namespace EquipSlot
{
    RelocAddr<_GetLeftHandEquipSlot> GetLeftHandEquipSlot(0x346E70);
}

namespace Alchemy
{
    RelocAddr<_IsPoison> IsPoison(0x100790);
}