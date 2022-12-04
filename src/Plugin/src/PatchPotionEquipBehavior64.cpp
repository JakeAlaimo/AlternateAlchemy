#include "PatchPotionEquipBehavior.h"
#include "GameReferences.h"
#include "Offsets.h"
#include "SafeWrite.h"

void ApplyPatch(UInt64 base, UInt8* buf, UInt32 len)
{
	for (UInt32 i = 0; i < len; i++)
	{
		SafeWrite8(base + i, buf[i]);
	}
}

UInt8* GetCodecaveBytes(UInt64 CodecaveFunctionAddress, UInt8 ExtraBytes)
{
	UInt8* Bytes = new UInt8[13 + ExtraBytes];

	//PUSH RAX
	Bytes[0] = 0x50;

	//MOV RAX, (Abs. function addr here)
	Bytes[1] = 0x48;
	Bytes[2] = 0xB8;
	memcpy(Bytes + 3, &CodecaveFunctionAddress, sizeof(UInt64));

	//CALL RAX
	Bytes[11] = 0xFF;
	Bytes[12] = 0xD0;

	if (ExtraBytes > 0)
	{
		memset(Bytes + 13, 0x90, ExtraBytes); // NOP, just to pad the end to be compliant with the ASM we are replacing
	}

	return Bytes;
}

namespace AlternateAlchemy
{
	UInt32 OverrideFormID = 0;
	bool OverridePoisonState = false;

	void TogglePoisonFlagIfAlternateEquipped(TESObjectREFR* Equipper, TESForm* EquippedItem, BGSEquipSlot* EquippedSlot)
	{
		OverrideFormID = 0;

		if (!Equipper || !Equipper->baseForm || Equipper->baseForm->formID != 7)
		{
			return; // ONLY toggle for the player
		}

		if (!EquippedItem)
		{
			return;
		}

		if (EquippedItem->formType != kFormType_Potion)
		{
			return;
		}

		BGSEquipSlot* LeftHandSlot = EquipSlot::GetLeftHandEquipSlot();

		if (EquippedSlot != LeftHandSlot)
		{
			return;
		}

		AlchemyItem* AsAlchemyItem = static_cast<AlchemyItem*>(EquippedItem);

		if (AsAlchemyItem->IsFood())
		{
			return;
		}

		OverrideFormID = AsAlchemyItem->formID;
		OverridePoisonState = !AsAlchemyItem->IsPoison();
	}

	void PatchPotionEquip()
	{
		constexpr UInt8 MinimumCodecaveSize = 13;
		constexpr UInt8 PoisonToggleCodecaveSize = 13;

#ifndef SE_BUILD
		constexpr UInt8 IsPoisonOverrideCodecaveSize = 25; // Replacing the whole function
#else
		constexpr UInt8 IsPoisonOverrideCodecaveSize = 26; // AE builds optimize this function slightly - 1 byte larger in SE
#endif

		UInt8* PoisonFlagToggleCodecaveBytes = GetCodecaveBytes((UInt64)&CallPoisonFlagToggleOnAltEquip, PoisonToggleCodecaveSize - MinimumCodecaveSize);
		ApplyPatch(EquipItem::Offset, PoisonFlagToggleCodecaveBytes, PoisonToggleCodecaveSize * sizeof(UInt8));
		delete[] PoisonFlagToggleCodecaveBytes;

		UInt8* IsPoisonOverrideCodecaveBytes = GetCodecaveBytes((UInt64)&IsPoisonOverride, IsPoisonOverrideCodecaveSize - MinimumCodecaveSize);
		ApplyPatch(Alchemy::IsPoison.GetUIntPtr(), IsPoisonOverrideCodecaveBytes, IsPoisonOverrideCodecaveSize * sizeof(UInt8));
		delete[] IsPoisonOverrideCodecaveBytes;
	}
}