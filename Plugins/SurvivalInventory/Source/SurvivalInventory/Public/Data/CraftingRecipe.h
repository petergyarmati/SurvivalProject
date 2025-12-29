// Copyright Peter Gyarmati (Bitrose Studio)

#pragma once

#include "CoreMinimal.h"
#include "CraftingRecipe.generated.h"

class UItemDefinition;

USTRUCT(BlueprintType)
struct FCraftingIngredient
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UItemDefinition> ItemRef;

	UPROPERTY(EditAnywhere)
	int32 Quantity = 1;
};

UCLASS(BlueprintType, Const)
class UCraftingRecipe : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	FText DisplayName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Crafting")
	TArray<FCraftingIngredient> Inputs;

	// In our case might be only one result will be
	UPROPERTY(EditDefaultsOnly, Category = "Crafting")
	TArray<FCraftingIngredient> Outputs;
	
	UPROPERTY(EditDefaultsOnly, Category = "Crafting", meta = (ClampMin = "1.0", ToolTip = "Time to craft in seconds to craft one unit"))
	float CraftingTime = 10.0f;

	// Helper to check if an inventory has the required ingredients
	bool CanCraft(class UInventoryComponent* Inventory) const;
};