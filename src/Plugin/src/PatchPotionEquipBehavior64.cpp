#include "PatchPotionEquipBehavior.h"
#include "GameReferences.h"
#include "GameEvents.h"
#include "GameMenus.h"
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
	AlchemyItem* LastOverriddenAlchemyItem = nullptr;
	UInt32 OverriddenFlags = 0;

	void ClearLastOverriddenAlchemyItem()
	{
		if (LastOverriddenAlchemyItem)
		{
			LastOverriddenAlchemyItem->itemData.flags = OverriddenFlags;
			LastOverriddenAlchemyItem = nullptr;
		}
	}

	class ClearOverrideOnMenuChanged : public BSTEventSink<MenuOpenCloseEvent>
	{
		virtual	EventResult	ReceiveEvent(MenuOpenCloseEvent* Event, EventDispatcher<MenuOpenCloseEvent>* Dispatcher)
		{
			std::string MenuName = Event->menuName.data; 
			if (MenuName == "InventoryMenu" || MenuName == "FavoritesMenu")
			{
				ClearLastOverriddenAlchemyItem();
			}

			return kEvent_Continue;
		}
	};

	ClearOverrideOnMenuChanged ClearOverride;
	bool ClearOverrideRegistered = false;

	void TogglePoisonFlagIfAlternateEquipped(TESObjectREFR* Equipper, TESForm* EquippedItem, BGSEquipSlot* EquippedSlot)
	{
		ClearLastOverriddenAlchemyItem();

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

		LastOverriddenAlchemyItem = AsAlchemyItem;
		OverriddenFlags = AsAlchemyItem->itemData.flags;

		if (AsAlchemyItem->IsPoison())
		{
			AsAlchemyItem->itemData.flags &= ~AlchemyItem::kFlag_Poison;
		}
		else
		{
			AsAlchemyItem->itemData.flags |= AlchemyItem::kFlag_Poison;
		}

		if (!ClearOverrideRegistered)
		{
			MenuManager::GetSingleton()->MenuOpenCloseEventDispatcher()->AddEventSink(&ClearOverride);
		}
	}

	void PatchPotionEquip()
	{
		constexpr UInt8 MinimumCodecaveSize = 13;
		constexpr UInt8 PoisonToggleCodecaveSize = 13;

		UInt8* PoisonFlagToggleCodecaveBytes = GetCodecaveBytes((UInt64)&CallPoisonFlagToggleOnAltEquip, PoisonToggleCodecaveSize - MinimumCodecaveSize);
		ApplyPatch(EquipItem::Offset, PoisonFlagToggleCodecaveBytes, PoisonToggleCodecaveSize * sizeof(UInt8));
		delete[] PoisonFlagToggleCodecaveBytes;
	}
}