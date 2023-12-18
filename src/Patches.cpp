#include "Patches.h"
#include "PCH.h"
#include "Hooks.h"
#include "SKSE/API.h"

using namespace REL;
using namespace SKSE::stl;
using namespace AlternateAlchemyHooks;

namespace AlternateAlchemy
{
	RE::AlchemyItem* LastOverriddenAlchemyItem = nullptr;
	stl::enumeration<RE::AlchemyItem::AlchemyFlag, std::uint32_t> OverriddenFlags;

	void ClearLastOverriddenAlchemyItem()
	{
		if (LastOverriddenAlchemyItem)
		{
			LastOverriddenAlchemyItem->data.flags = OverriddenFlags;
			LastOverriddenAlchemyItem = nullptr;
		}
	}

	class ClearOverrideOnMenuChanged : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{
		virtual RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* Event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* Source)
		{
			if (Event->menuName == "InventoryMenu" || Event->menuName == "FavoritesMenu")
			{
				ClearLastOverriddenAlchemyItem();
			}

			return RE::BSEventNotifyControl::kContinue;
		}
	};

	ClearOverrideOnMenuChanged ClearAlchemyOverrideOnInventoryOpenClose;

	void TogglePoisonFlagIfAlternateEquipped(RE::Actor* Equipper, RE::TESForm* EquippedItem, RE::BGSEquipSlot* EquippedSlot)
	{
		ClearLastOverriddenAlchemyItem();

		if (!Equipper || (!Equipper->IsPlayer() && !Equipper->IsPlayerRef()))
		{
			return; // ONLY toggle for the player
		}

		if (!EquippedItem)
		{
			return;
		}

		if (EquippedItem->formType != RE::FormType::AlchemyItem)
		{
			return;
		}

		if (!AlternateAlchemyHooks::GetLeftHandEquipSlotHook().address() || EquippedSlot != AlternateAlchemyHooks::GetLeftHandEquipSlotHook()())
		{
			return;
		}

		RE::AlchemyItem* AsAlchemyItem = static_cast<RE::AlchemyItem*>(EquippedItem);

		if (!AsAlchemyItem)
		{
			return;
		}

		LastOverriddenAlchemyItem = AsAlchemyItem;
		OverriddenFlags = AsAlchemyItem->data.flags;

		if (AsAlchemyItem->IsPoison())
		{
			AsAlchemyItem->data.flags.reset(RE::AlchemyItem::AlchemyFlag::kPoison);
		}
		else
		{
			AsAlchemyItem->data.flags.set(RE::AlchemyItem::AlchemyFlag::kPoison);
		}
	}

	REL::Relocation<AlternateAlchemyHooks::EquipItem> OriginalInventoryMenuEquip;
	REL::Relocation<AlternateAlchemyHooks::EquipItem> OriginalFavoritesMenuEquip;

	void InventoryMenuEquipOverride(uint64_t Unk1, RE::Actor* Equipper, RE::TESForm** EquippedItem, RE::BGSEquipSlot* Slot, uint8_t Unk2)
	{
		if (EquippedItem)
		{
			TogglePoisonFlagIfAlternateEquipped(Equipper, *EquippedItem, Slot);
		}

		OriginalInventoryMenuEquip(Unk1, Equipper,EquippedItem, Slot, Unk2);
	}

	void FavoritesMenuEquipOverride(uint64_t Unk1, RE::Actor* Equipper, RE::TESForm** EquippedItem, RE::BGSEquipSlot* Slot, uint8_t Unk2)
	{
		if (EquippedItem)
		{
			TogglePoisonFlagIfAlternateEquipped(Equipper, *EquippedItem, Slot);
		}

		OriginalFavoritesMenuEquip(Unk1, Equipper, EquippedItem, Slot, Unk2);
	}

	bool PatchItemEquipBehavior()
	{
		if (auto* UI = RE::UI::GetSingleton())
		{
			UI->AddEventSink<RE::MenuOpenCloseEvent>(&ClearAlchemyOverrideOnInventoryOpenClose);
		}
		else
		{
			return false;
		}

		SKSE::Trampoline& Trampoline = SKSE::GetTrampoline();
		Trampoline.create(30);

		if (uintptr_t InventoryMenuEquipItemAddress = AlternateAlchemyHooks::GetInventoryMenuEquipHook().address())
		{
			OriginalInventoryMenuEquip = Trampoline.write_call<5>(InventoryMenuEquipItemAddress, InventoryMenuEquipOverride);
		}
		else
		{
			return false;
		}

		if (uintptr_t FavoritesMenuEquipItemAddress = AlternateAlchemyHooks::GetFavoritesMenuEquipHook().address())
		{
			OriginalFavoritesMenuEquip = Trampoline.write_call<5>(FavoritesMenuEquipItemAddress, FavoritesMenuEquipOverride);
		}
		else
		{
			return false;
		}

		return true;
	}
}