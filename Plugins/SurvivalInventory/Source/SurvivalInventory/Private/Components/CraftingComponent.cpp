// Copyright Peter Gyarmati (Bitrose Studio)


#include "Components/CraftingComponent.h"

#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"


UCraftingComponent::UCraftingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCraftingComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, CraftingQueue);
}

void UCraftingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Only Server processes crafting
	if (!GetOwner()->HasAuthority() || CraftingQueue.Items.Num() == 0) return;

	// Always process the HEAD of the queue (Index 0)
	ProcessCurrentJob(DeltaTime);
}

void UCraftingComponent::Server_RequestCraft_Implementation(const UCraftingRecipe* Recipe, int32 Amount)
{
	if (!Recipe || Amount <= 0 || !LinkedInventory) return;

	// 1. Verify Resources (Can we afford 5 Planks?)
	// Note: You need a helper 'HasIngredients' in Inventory logic
	if (!LinkedInventory->HasIngredients(Recipe->Inputs, Amount)) return;

	// 2. Consume Resources IMMEDIATELY (Upfront cost)
	LinkedInventory->ConsumeIngredients(Recipe->Inputs, Amount);

	// 3. Add to Queue
	FCraftingJob NewJob;
	NewJob.Recipe = Recipe;
	NewJob.Quantity = Amount;
	NewJob.Progress = 0.0f;
	NewJob.JobID = NextJobID++; // Unique ID ensures Planks(5) -> Iron(1) -> Planks(3) are distinct

	CraftingQueue.Items.Add(NewJob);
	CraftingQueue.MarkItemDirty(NewJob);
}

void UCraftingComponent::Server_CancelJob_Implementation(int32 JobID)
{
	// Find the job by ID
	for (int32 i = 0; i < CraftingQueue.Items.Num(); i++)
	{
		if (CraftingQueue.Items[i].JobID == JobID)
		{
			FCraftingJob& Job = CraftingQueue.Items[i];

			// 1. Refund the REMAINING quantity
			// If we crafted 2 of 5, we refund materials for 3.
			for (const auto& Input : Job.Recipe->Inputs)
			{
				LinkedInventory->TryAddItem(Input.ItemRef, Input.Quantity * Job.Quantity);
			}

			// 2. Remove from queue
			CraftingQueue.Items.RemoveAt(i);
			CraftingQueue.MarkArrayDirty();
			return;
		}
	}
}

void UCraftingComponent::ProcessCurrentJob(float DeltaTime)
{
	// Get mutable reference to the first job
	FCraftingJob& CurrentJob = CraftingQueue.Items[0];
    
	// Increment Progress
	CurrentJob.Progress += DeltaTime;

	// Check Completion
	if (CurrentJob.Progress >= CurrentJob.Recipe->CraftingTime)
	{
		FinishCraftingItem(CurrentJob);
	}
    
	// We only replicate progress if it changes significantly or finishes
	// Optimization: Don't mark dirty every single frame, maybe every 0.1s
	CraftingQueue.MarkItemDirty(CurrentJob);
}

void UCraftingComponent::FinishCraftingItem(FCraftingJob& Job)
{
	// 1. Give Output Items
	for (const auto& Output : Job.Recipe->Outputs)
	{
		LinkedInventory->TryAddItem(Output.ItemRef, Output.Quantity);
	}

	// 2. Reset Progress for next item in the stack
	Job.Progress = 0.0f;
	Job.Quantity--;

	// 3. If Job is finished, remove it
	if (Job.Quantity <= 0)
	{
		CraftingQueue.Items.RemoveAt(0);
		CraftingQueue.MarkArrayDirty();
	}
}

