#include "Offsets.h"
#include "GameForms.h"

namespace EquipItem
{
    const uintptr_t Offset = RelocationManager::s_baseAddr + 0x670310;
}

namespace EquipSlot
{
    RelocAddr<_GetLeftHandEquipSlot> GetLeftHandEquipSlot(0x348DC0);
}

namespace Alchemy
{
    RelocAddr<_IsPoison> IsPoison(0x1020E0);
}