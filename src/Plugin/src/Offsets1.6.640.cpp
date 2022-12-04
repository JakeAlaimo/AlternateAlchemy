#include "Offsets.h"
#include "GameForms.h"

namespace EquipItem
{
    const uintptr_t Offset = RelocationManager::s_baseAddr + 0x670810;
}

namespace EquipSlot
{
    RelocAddr<_GetLeftHandEquipSlot> GetLeftHandEquipSlot(0x348E30);
}

namespace Alchemy
{
    RelocAddr<_IsPoison> IsPoison(0x1022E0);
}