
#include "ResourceManagerComponent.h"

#include "BaseBuilding.h"
#include "CityBuilderGameModeBase.h"
#include "JobSystemComponent.h"

UResourceManagerComponent::UResourceManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Resources.Add(EResourceType::Skyroot, FResourceData{0, MaxStorage});
	Resources.Add(EResourceType::Skystone, FResourceData{0, MaxStorage});
	Resources.Add(EResourceType::Nectar, FResourceData{0, MaxStorage});
	Resources.Add(EResourceType::ManaCrystal, FResourceData{0, MaxStorage});
	Resources.Add(EResourceType::Ingredients, FResourceData{0, MaxStorage});
	//Resources.Add(EResourceType::Health, FResourceData{0, MaxStorage}); Should this be a resource?
	Resources.Add(EResourceType::None, FResourceData{0, 0});
}

void UResourceManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if(UWorld* World = GetWorld())
	{
		if (AGameModeBase* GameMode = World->GetAuthGameMode())
		{
			if (const ACityBuilderGameModeBase* JamGameMode = Cast<ACityBuilderGameModeBase>(GameMode))
			{
				if(JamGameMode)
				{
					JobSystemComponent = JamGameMode->GetJobSystemComponent();
				}
			}
		}
	}

	// Start the resource loop
	GetWorld()->GetTimerManager().SetTimer(ResourceLoopHandle, this, &UResourceManagerComponent::CalculateResources, LoopCalculationInterval, true);
}

void UResourceManagerComponent::CalculateResources()
{
	for (auto& ResourcePair : Resources)
	{
		EResourceType Resource = ResourcePair.Key;
		FResourceData& ResourceData = ResourcePair.Value;
		
		if (ResourceData.CurrentAmount < ResourceData.MaxAmount)
		{
			if (ResourceData.CurrentWorkers > 0)
			{
				ResourceData.TotalAmountPerSecond = ResourceData.AmountPerSecond * ResourceData.CurrentWorkers;
			}
			else
			{
				ResourceData.TotalAmountPerSecond = 0;
			}
			
			ResourceData.CurrentAmount += ResourceData.TotalAmountPerSecond;

			if (ResourceData.CurrentAmount > ResourceData.MaxAmount)
			{
				ResourceData.CurrentAmount = ResourceData.MaxAmount;
			}
		}
		else
		{
			// TODO :: This might need to be reset when resources are spent or storage is increased
			ResourceData.TotalAmountPerSecond = 0;
		}

		/*// Upkeep Calculation
		if(Resource == EResourceType::Mana)
		{
			ResourceData.CurrentAmount += GlobalUpkeep;
			if(ResourceData.CurrentAmount < 0)
			{
				//ResourceData.CurrentAmount = 0;
			}
		}*/
	}
}

void UResourceManagerComponent::UpdateCurrentWorkerForResource(EResourceType Resource, int32 WorkerChange)
{
	// updates current workers for resource calculations (based on workers at workstations)
	Resources[Resource].CurrentWorkers += WorkerChange;
}

void UResourceManagerComponent::UpdateMaxWorkerForResource(EResourceType Resource, int32 WorkerChange)
{
	Resources[Resource].MaxWorkers += WorkerChange;
}

void UResourceManagerComponent::UpdateResourcesFromJobData(const FJob JobReference)
{
	//Resources[JobReference.AssociatedResource].Upkeep = JobReference.UpkeepCost;
	Resources[JobReference.AssociatedResource].AmountPerSecond = JobReference.ResourceGeneration;
	
	// Track for later when its deducted
	// TODO :: CALL WHEN BUILDING DELETED
	GlobalUpkeep += JobReference.UpkeepCost;

	// TODO :: CURRENTLY UNUSED WILL BE USED FOR BUILDING UPGRADES
	//Resources[JobReference.AssociatedResource].TotalJobsInResource += JobReference.AvailableSlotsAtBuilding;
}

int32 UResourceManagerComponent::GetResourceAmount(EResourceType Resource)
{
	return Resources[Resource].CurrentAmount;
}

int32 UResourceManagerComponent::GetAllowedWorkersInResource(EResourceType Resource)
{
	return Resources[Resource].AllowedWorkers;
}

int32 UResourceManagerComponent::GetMaxWorkersInResource(EResourceType Resource)
{
	return Resources[Resource].MaxWorkers;
}

// Called every frame
void UResourceManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(ShouldDrawDebugResources)
	{
		for (auto& ResourceData : Resources)
		{
			const FResourceData& CurrentData = ResourceData.Value;
			const EResourceType ResourceType = ResourceData.Key;
			FString ResourceTypeName = UEnum::GetValueAsString(ResourceType);
			
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("Resource Type: %s"), *ResourceTypeName));
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("Total Worker Positions: %d"), CurrentData.MaxWorkers));
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("Current Amount: %f"), CurrentData.CurrentAmount));
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("Max Amount: %f"), CurrentData.MaxAmount));
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("Amount Per Second: %f"), CurrentData.TotalAmountPerSecond));
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("Upkeep: %f"), CurrentData.Upkeep));
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("Workers: %d"), CurrentData.CurrentWorkers));
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("   ")));
		}
	}
}


