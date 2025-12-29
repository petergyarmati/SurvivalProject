// Copyright Peter Gyarmati (Bitrose Studio)

#pragma once

#include "CoreMinimal.h"
#include "Data/CraftingRecipe.h"
#include "GameFramework/PlayerController.h"
#include "InvPlayerController.generated.h"

class UCraftingComponent;
/**
 * 
 */
UCLASS()
class SURVIVALINVENTORY_API AInvPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AInvPlayerController();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Call this from Input Action (Key 1 = Index 0, Key 2 = Index 1...)
	UFUNCTION(BlueprintCallable)
	void OnHotbarKeyPressed(int32 SlotIndex);

	// The currently held item index
	UPROPERTY(BlueprintReadOnly, Replicated)
	int32 ActiveHotbarIndex;

	UFUNCTION(Server, Reliable)
	void Server_SetActiveHotbarSlot(int32 NewIndex);
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TObjectPtr<UInventoryComponent> InventoryComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory|Crafting")
	TObjectPtr<UCraftingComponent> CraftingComp;
};
