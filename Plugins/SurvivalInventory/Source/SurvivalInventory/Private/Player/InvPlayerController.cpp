// Copyright Peter Gyarmati (Bitrose Studio)


#include "Player/InvPlayerController.h"

#include "Components/CraftingComponent.h"
#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"

AInvPlayerController::AInvPlayerController()
{
	PrimaryActorTick.bCanEverTick = false;
	
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	CraftingComp = CreateDefaultSubobject<UCraftingComponent>("CraftingComponent");
}

void AInvPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(ThisClass, ActiveHotbarIndex, COND_OwnerOnly);
}

void AInvPlayerController::OnHotbarKeyPressed(int32 SlotIndex)
{
	// Client-side prediction (update UI instantly)
	ActiveHotbarIndex = SlotIndex;
	// We have not HotbarSlotChanged event yet. 
	// OnHotbarSlotChanged.Broadcast(SlotIndex); 

	// Tell Server
	Server_SetActiveHotbarSlot(SlotIndex);
}

void AInvPlayerController::Server_SetActiveHotbarSlot_Implementation(int32 NewIndex)
{
	ActiveHotbarIndex = NewIndex;
    
	// --- Future GAS Implementation ---
	// 1. Unequip old item (Remove Abilities)
	// 2. Equip new item (Grant Abilities from ItemDef)
	// 3. Attach Mesh to Character Hand Socket
}
