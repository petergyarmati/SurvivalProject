// Copyright Peter Gyarmati (Bitrose Studio)


#include "Components/InventoryComponent.h"

#include "Data/ItemDefinition.h"
#include "Net/UnrealNetwork.h"


UInventoryComponent::UInventoryComponent()
{
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, InventoryData);
}

int32 UInventoryComponent::TryAddItem(const UItemDefinition* Item, int32 Amount)
{
	return 0;
}

void UInventoryComponent::Server_AddItem_Implementation(const UItemDefinition* Item, int32 Amount)
{
}

void UInventoryComponent::Server_TransferItem_Implementation(UInventoryComponent* SourceComp, int32 SourceIndex,
	int32 DestIndex)
{
	// 1. Validation: Ensure components and indices exist
    if (!SourceComp || !SourceComp->InventoryData.Items.IsValidIndex(SourceIndex)) return;
    if (!this->InventoryData.Items.IsValidIndex(DestIndex)) return;

    FInventorySlot& SourceSlot = SourceComp->InventoryData.Items[SourceIndex];
    FInventorySlot& DestSlot = this->InventoryData.Items[DestIndex];

    // 2. Logic: Empty Destination (Simple Move)
    if (DestSlot.IsEmpty())
    {
        // Copy Data
        DestSlot.ItemDef = SourceSlot.ItemDef;
        DestSlot.Quantity = SourceSlot.Quantity;
        DestSlot.DynamicValue = SourceSlot.DynamicValue;
        
        // Clear Source
        SourceSlot = FInventorySlot(); // Reset to default
        SourceSlot.SlotIndex = SourceIndex; // Restore index
    }
    // 3. Logic: Same Item (Stacking)
    else if (DestSlot.ItemDef == SourceSlot.ItemDef)
    {
        int32 MaxStack = DestSlot.ItemDef->MaxStackSize;
        int32 SpaceRemaining = MaxStack - DestSlot.Quantity;

        if (SpaceRemaining >= SourceSlot.Quantity)
        {
            // Fully stack into destination
            DestSlot.Quantity += SourceSlot.Quantity;
            SourceSlot = FInventorySlot(); // Clear Source
            SourceSlot.SlotIndex = SourceIndex;
        }
        else
        {
            // Partial stack (fill destination, leave rest in source)
            DestSlot.Quantity = MaxStack;
            SourceSlot.Quantity -= SpaceRemaining;
        }
    }
    // 4. Logic: Different Item (Swap)
    else
    {
        // We use a temp copy to swap
        FInventorySlot Temp = DestSlot;
        
        DestSlot.ItemDef = SourceSlot.ItemDef;
        DestSlot.Quantity = SourceSlot.Quantity;
        DestSlot.DynamicValue = SourceSlot.DynamicValue;

        SourceSlot.ItemDef = Temp.ItemDef;
        SourceSlot.Quantity = Temp.Quantity;
        SourceSlot.DynamicValue = Temp.DynamicValue;
    }

    // 5. Critical: Mark Dirty for Replication
    SourceComp->InventoryData.MarkItemDirty(SourceSlot);
    this->InventoryData.MarkItemDirty(DestSlot);
}

bool UInventoryComponent::HasIngredients(const TArray<FCraftingIngredient> Ingredients, int32 Amount)
{
	return false;
}

bool UInventoryComponent::ConsumeIngredients(const TArray<FCraftingIngredient> Ingredients, int32 Amount)
{
	return false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner()->HasAuthority())
	{
		// Server: Initialize Empty Slots
		InventoryData.Items.SetNum(Capacity);
		for (int32 i = 0; i < Capacity; i++)
		{
			InventoryData.Items[i].SlotIndex = i;
			InventoryData.MarkItemDirty(InventoryData.Items[i]);
		}
	}
    
	// Assign Owner for callbacks
	InventoryData.OwnerComp = this;
}

void UInventoryComponent::HandleSlotChange(int32 SlotIndex)
{
	if (InventoryData.Items.IsValidIndex(SlotIndex))
	{
		// Broadcast to UI (e.g., WBP_InventorySlot)
		OnSlotChanged.Broadcast(SlotIndex, InventoryData.Items[SlotIndex]);
	}
}
