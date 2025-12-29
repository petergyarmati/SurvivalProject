// Copyright Peter Gyarmati (Bitrose Studio)

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Data/FastArray.h"
#include "InventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotUpdate, int32, Index, const FInventorySlot&, Data);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	int32 TryAddItem(const UItemDefinition* Item, int32 Amount);

	// Configuration
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	int32 Capacity = 20;

	// Filter: If empty, accept everything. If set, only accept items matching these tags.
	// Example: Hotbar uses "Item.Type.Equippable", Furnace Fuel slot uses "Item.Type.Fuel"
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FGameplayTagContainer SupportedTags;

	// The Data
	UPROPERTY(Replicated)
	FInventoryList InventoryData;

	// UI Events
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSlotUpdate OnSlotChanged;

	// --- Server API ---
    
	// Tries to add an item. Returns amount remaining (0 if fully added).
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_AddItem(const UItemDefinition* Item, int32 Amount);

	// Moves item from one slot to another (handles splitting/stacking)
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_TransferItem(UInventoryComponent* SourceComp, int32 SourceIndex, int32 DestIndex);
	
	bool HasIngredients(const TArray<FCraftingIngredient> Ingredients, int32 Amount);

	// Used by Crafting System to consume materials
	bool ConsumeIngredients(const TArray<FCraftingIngredient> Ingredients, int32 Amount);

	// --- Internal ---
	virtual void BeginPlay() override;
	void HandleSlotChange(int32 SlotIndex); // Called by FastArray struct
};
