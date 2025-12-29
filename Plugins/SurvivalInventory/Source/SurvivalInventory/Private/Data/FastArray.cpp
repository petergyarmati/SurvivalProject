// Copyright Peter Gyarmati (Bitrose Studio)


#include "Data/FastArray.h"

#include "Components/InventoryComponent.h"

void FInventorySlot::PreReplicatedRemove(const FInventoryList& InArraySerializer)
{
}

void FInventorySlot::PostReplicatedAdd(const FInventoryList& InArraySerializer)
{
}

void FInventorySlot::PostReplicatedChange(const FInventoryList& InArraySerializer)
{
	if (InArraySerializer.OwnerComp)
	{
		InArraySerializer.OwnerComp->HandleSlotChange(SlotIndex);
	}
}

void FCraftingJob::PreReplicatedRemove(const struct FCraftingQueue& InArraySerializer)
{
}

void FCraftingJob::PostReplicatedAdd(const struct FCraftingQueue& InArraySerializer)
{
}

void FCraftingJob::PostReplicatedChange(const struct FCraftingQueue& InArraySerializer)
{
}
