// Copyright Peter Gyarmati (Bitrose Studio)

#pragma once

#include "CoreMinimal.h"
#include "CraftingRecipe.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "FastArray.generated.h"

class UCraftingComponent;
class UItemDefinition;
class UInventoryComponent;

USTRUCT(BlueprintType)
struct FInventorySlot : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FInventorySlot() : SlotIndex(-1), Quantity(0), DynamicValue(0.f) {}

	// Static Data Reference
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<const UItemDefinition> ItemDef = nullptr;

	// Used for UI mapping
	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Quantity;

	// Use this for Durability (0-1), Ammo Count, or Spoil Time.
	// Replicating a float is cheap
	UPROPERTY(BlueprintReadOnly)
	float DynamicValue;

	// Helper to check if slot is empty
	bool IsEmpty() const { return ItemDef == nullptr || Quantity <= 0; }

	// --- FastArray Callbacks ---
	void PreReplicatedRemove(const struct FInventoryList& InArraySerializer);
	void PostReplicatedAdd(const struct FInventoryList& InArraySerializer);
	void PostReplicatedChange(const struct FInventoryList& InArraySerializer);
};

USTRUCT(BlueprintType)
struct FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FInventorySlot> Items;

	// Pointer to component to fire events
	UPROPERTY(NotReplicated)
	TObjectPtr<UInventoryComponent> OwnerComp;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventorySlot, FInventoryList>(Items, DeltaParms, *this);
	}
};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};

USTRUCT(BlueprintType)
struct FCraftingJob : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FCraftingJob() 
		: Recipe(nullptr), Quantity(0), Progress(0.f), JobID(0) {}
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<const UCraftingRecipe> Recipe;

	// How many times to craft this recipe (e.g., 5 Planks)
	UPROPERTY(BlueprintReadOnly)
	int32 Quantity;

	// Progress of the CURRENT item being crafted (0.0 to Recipe->CraftingTime)
	UPROPERTY(BlueprintReadOnly)
	float Progress;
	
	// Unique ID to handle UI animations/cancellation correctly
	UPROPERTY(BlueprintReadOnly)
	int32 JobID;
	
	// The Server Time (GetWorld()->GetTimeSeconds()) when the CURRENT item started crafting
	UPROPERTY()
	float JobStartTime; 
    
	// Is this job currently processing?
	UPROPERTY()
	bool bIsActive;

	// Helper for UI to get 0.0-1.0 progress
	float GetProgress(float CurrentWorldTime) const
	{
		if (!bIsActive || !Recipe) return 0.0f;
		float Elapsed = CurrentWorldTime - JobStartTime;
		return FMath::Clamp(Elapsed / Recipe->CraftingTime, 0.0f, 1.0f);
	}

	// Fast Array Boilerplate
	void PreReplicatedRemove(const struct FCraftingQueue& InArraySerializer);
	void PostReplicatedAdd(const struct FCraftingQueue& InArraySerializer);
	void PostReplicatedChange(const struct FCraftingQueue& InArraySerializer);
};

USTRUCT(BlueprintType)
struct FCraftingQueue : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FCraftingJob> Items;

	UPROPERTY(NotReplicated)
	TObjectPtr<UCraftingComponent> OwnerComp;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FCraftingJob, FCraftingQueue>(Items, DeltaParms, *this);
	}
};

template<>
struct TStructOpsTypeTraits<FCraftingQueue> : public TStructOpsTypeTraitsBase2<FCraftingQueue>
{
	enum { WithNetDeltaSerializer = true };
};
