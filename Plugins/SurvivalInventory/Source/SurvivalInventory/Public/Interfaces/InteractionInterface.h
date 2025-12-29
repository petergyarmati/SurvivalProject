// Copyright Peter Gyarmati (Bitrose Studio)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SURVIVALINVENTORY_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Return interaction text (e.g., "Press E to Pickup Stone")
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FText GetInteractionText(const APlayerController* Interactor);

	// The main interaction event
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract(APlayerController* Interactor);
};
