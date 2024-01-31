
#include "CitizenManager.h"

#include "CityBuilderGameModeBase.h"
#include "ResourceManagerComponent.h"
#include "WispAIController.h"
#include "GameFramework/GameModeBase.h"

UCitizenManager::UCitizenManager()
{
	// should not need to tick
	PrimaryComponentTick.bCanEverTick = false;
}

void UCitizenManager::SetAllowedToWorkOnResource(const EResourceType Resource, const int32 Change) const
{
	if (ResourceManagerComponent)
	{
		ResourceManagerComponent->Resources[Resource].AllowedWorkers += Change;

		// handle over max cap
		if(ResourceManagerComponent->Resources[Resource].AllowedWorkers > ResourceManagerComponent->Resources[Resource].MaxWorkers)
		{
			ResourceManagerComponent->Resources[Resource].AllowedWorkers = ResourceManagerComponent->Resources[Resource].MaxWorkers;
		}

		// handle under min cap
		if(ResourceManagerComponent->Resources[Resource].AllowedWorkers < 0)
		{
			ResourceManagerComponent->Resources[Resource].AllowedWorkers = 0;
		}

		// Got to fire someone if we reduce workers
		if(ResourceManagerComponent->Resources[Resource].AllowedWorkers < ResourceManagerComponent->Resources[Resource].AssignedWorkers)
		{
			ResourceManagerComponent->Resources[Resource].AssignedWorkers = ResourceManagerComponent->Resources[Resource].AllowedWorkers;
			
			if(ResourceManagerComponent->Resources[Resource].WorkerReferences.Num() > 0)
			{
				const int32 RandomIndex = FMath::RandRange(0, ResourceManagerComponent->Resources[Resource].WorkerReferences.Num() - 1);

				AWispAIController* WorkerToFire = Cast<AWispAIController>(ResourceManagerComponent->Resources[Resource].WorkerReferences[RandomIndex]);
				ResourceManagerComponent->Resources[Resource].WorkerReferences.Remove(WorkerToFire);
				if(JobSystemComponent)
				{
					JobSystemComponent->UpdateWorkerJob(WorkerToFire, true);
				}
			}
		}
	}
}

// Currently unused but will handle reducing resource counts when buying buildings
void UCitizenManager::UpdateResourceAmount(EResourceType Resource, int32 Change) const
{
	if (ResourceManagerComponent)
	{
		ResourceManagerComponent->Resources[Resource].CurrentAmount += Change;
	}
}

void UCitizenManager::BeginPlay()
{
	Super::BeginPlay();


	if (const UWorld* World = GetWorld())
	{
		if (AGameModeBase* GameMode = World->GetAuthGameMode())
		{
			const ACityBuilderGameModeBase* JamGameMode = Cast<ACityBuilderGameModeBase>(GameMode);

			if(JamGameMode)
			{
				ResourceManagerComponent = JamGameMode->GetResourceManagerComponent();
				JobSystemComponent = JamGameMode->GetJobSystemComponent();
			}
		}
	}
}

void UCitizenManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

