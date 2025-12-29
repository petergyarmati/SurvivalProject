// Copyright Peter Gyarmati (Bitrose Studio)

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ItemDefinition.generated.h"

/**
 * @class UItemDefinition
 * @brief Represents the definition of an item in the survival inventory system.
 *
 * This class provides the display properties, behavior tags, and stacking
 * rules for items within the survival inventory system.
 */
UCLASS(BlueprintType, Const)
class SURVIVALINVENTORY_API UItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	FText DisplayName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display", meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	TSoftObjectPtr<UStaticMesh> PickupMesh;

	// Defines item behavior: "Item.Type.Weapon", "Item.Consumable.Food"
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer ItemTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	int32 MaxStackSize = 1;
};
