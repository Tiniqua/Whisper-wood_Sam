
#include "JobSystemComponent.h"

#include "CityBuilderGameModeBase.h"
#include "WispAIController.h"
#include "Kismet/GameplayStatics.h"
#include "ResourceTypes.h"
#include "Kismet/KismetMathLibrary.h"

UJobSystemComponent::UJobSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UJobSystemComponent::UpdateWorkerJob(AWispAIController* WispAIController, bool ShouldFire)
{
	// For tracking current closest job
	FJob ClosestJob = FJob();
	
	if(const UWorld* World = GetWorld())
	{
		UResourceManagerComponent* ResourceManagerComponent = nullptr;
		
		if (AGameModeBase* GameMode = World->GetAuthGameMode())
		{
			if(const ACityBuilderGameModeBase* JamGameMode = Cast<ACityBuilderGameModeBase>(GameMode))
			{
				ResourceManagerComponent = JamGameMode->GetResourceManagerComponent();
			}
		}

		if(ResourceManagerComponent)
		{
			// Initialise 
			float ShortestJobDistance = FLT_MAX;
			bool JobFound = false;

			// Early out for firing worker
			if(ShouldFire)
			{
				// predicate bullshittery to access something without a pointer
				FJob currentJob = WispAIController->CurrentJob;
				FJob* FoundJob = JobInstances.FindByPredicate([currentJob](const FJob& Job)
				{
					return Job.JobBuilding == currentJob.JobBuilding && Job.AssociatedResource == currentJob.AssociatedResource;
				});

				if(FoundJob)
				{
					FoundJob->AvailableWorkstationsAtBuilding ++;
				}
				
				// tracks workers at this building
				WispAIController->WorkstationData.WorkStationKey = 0;
				WispAIController->CurrentJob = FJob();
				// tracks workers at all buildings of this resource
				ResourceManagerComponent->Resources[ClosestJob.AssociatedResource].AssignedWorkers -= 1;
				WispAIController->UpdateWorkBuilding(nullptr);
				WispAIController->CurrentWorkBuilding = nullptr;
				ResourceManagerComponent->TotalWorkingWisps -= 1;
				return;
			}
	
			// check all jobs
			for (const FJob& Job : JobInstances)
			{
				if(IsValid(Job.JobBuilding) && IsValid(WispAIController->CurrentHomeBuilding))
				{
					// may be unnecessary removed temporarily
					// should prevent looking at jobs with no slots (e.g. houses)
					// should be used for upgrading buildings
					if(Job.AvailableWorkstationsAtBuilding > 0)
					{
						// track shortest distance
						const float TempDistance = UKismetMathLibrary::Vector_Distance(Job.JobBuilding->GetActorLocation(),WispAIController->CurrentHomeBuilding->GetActorLocation());
			
						// find closest job available
						if(TempDistance < ShortestJobDistance)
						{
							// Check Job Instance is not null
							if(!(Job.AssociatedResource == EResourceType::None))
							{
								// Check resource isn't already at the cap for allowed workers
								if(ResourceManagerComponent->Resources[Job.AssociatedResource].AssignedWorkers < ResourceManagerComponent->Resources[Job.AssociatedResource].AllowedWorkers)
								{
									// Check player assigned workers is less than max workers available for resource 
									if(ResourceManagerComponent->Resources[Job.AssociatedResource].AllowedWorkers <= ResourceManagerComponent->Resources[Job.AssociatedResource].MaxWorkers)
									{
										// shortest job distance doesnt actually seem to drive anything investigate
										ShortestJobDistance = TempDistance;
										ClosestJob = Job;
										JobFound = true;
									}
								}	
							}
						}
					}
				}
			}
			if(JobFound)
			{
				WispAIController->CurrentJob = ClosestJob;				
				ResourceManagerComponent->Resources[ClosestJob.AssociatedResource].AssignedWorkers += 1;
				WispAIController->UpdateWorkBuilding(ClosestJob.JobBuilding);
				WispAIController->CurrentWorkBuilding = Cast<ABaseBuilding>(ClosestJob.JobBuilding);
				ResourceManagerComponent->Resources[ClosestJob.AssociatedResource].WorkerReferences.Add(WispAIController);
				ResourceManagerComponent->TotalWorkingWisps += 1;

				
				// predicate bullshittery to access something without a pointer
				FJob* FoundJob = JobInstances.FindByPredicate([ClosestJob](const FJob& Job)
				{
					return Job.JobBuilding == ClosestJob.JobBuilding && Job.AssociatedResource == ClosestJob.AssociatedResource;
				});
				
				if (FoundJob)
				{
					FoundJob->AvailableWorkstationsAtBuilding--;
				}
			}
		}
	}
}

TArray<FJob> UJobSystemComponent::ReturnAllJobData()
{
	return JobInstances;
}


