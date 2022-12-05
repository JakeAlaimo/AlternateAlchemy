Scriptname AlternateAlchemyPoisonMonitor extends ReferenceAlias

Perk Property ProtectBleedout auto
Weapon Property _AA_Syringe auto
FormList Property EmptyList auto

bool Property LeftHandBeneficialPoison auto
bool Property RightHandBeneficialPoison auto

int FriendHits = 0

Function UpdatePoisonStatus()

    Potion LeftHandPoison = AlternateAlchemy.GetEquippedPoison(GetActorRef(), 0)
    Potion RightHandPoison = AlternateAlchemy.GetEquippedPoison(GetActorRef(), 1)

    ; Alternate Alchemy allows equipping potions as poisons without actually mutating the poison flag
    ; The poison flag serves as the ultimate arbiter of whether a potion is predominantly harmful or beneficial
    LeftHandBeneficialPoison = LeftHandPoison && !LeftHandPoison.IsPoison()
    RightHandBeneficialPoison = RightHandPoison && !RightHandPoison.IsPoison()
EndFunction

int Function GetFriendHits()
    return FriendHits
EndFunction

Function IncrementFriendHits()
    FriendHits = FriendHits + 1
    RegisterForSingleUpdate(5.0)
EndFunction

Event OnInit()
    GetActorRef().AddPerk(ProtectBleedout)
    RegisterForMenu("InventoryMenu")
    RegisterForMenu("FavoritesMenu")
EndEvent

Event OnPlayerLoadGame()
    RegisterForMenu("InventoryMenu")
    RegisterForMenu("FavoritesMenu")
EndEvent

Event OnUpdate()
    FriendHits = 0
EndEvent

Event OnMenuClose(string Menu)
    UpdatePoisonStatus()
EndEvent

Event OnObjectEquipped(Form BaseObject, ObjectReference Reference)
    If (BaseObject == _AA_Syringe)
        RemoveAllInventoryEventFilters()
        UpdatePoisonStatus()
    EndIf
EndEvent

Event OnObjectUnequipped(Form BaseObject, ObjectReference Reference)
    If (BaseObject == _AA_Syringe)
        AddInventoryEventFilter(EmptyList) ; Prevents OnItemRemoved calls from queuing
        UpdatePoisonStatus()
    EndIf
EndEvent

Event OnItemRemoved(Form BaseItem, int ItemCount, ObjectReference ItemReference, ObjectReference DestContainer)
    Potion RemovedPotion = BaseItem as Potion

    If (RemovedPotion == none)
        return
    EndIf

    UpdatePoisonStatus()
EndEvent