

#include "BaseBuilding.h"

#include "CityBuilderGameModeBase.h"
#include "WispCharacter.h"
#include "Components/BillboardComponent.h"


ABaseBuilding::ABaseBuilding()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ABaseBuilding::BeginPlay()
{
	Super::BeginPlay();

	if(const UWorld* World = GetWorld())
	{
		if (AGameModeBase* GameMode = World->GetAuthGameMode())
		{
			if (const ACityBuilderGameModeBase* JamGameMode = Cast<ACityBuilderGameModeBase>(GameMode))
			{
				if(JamGameMode)
				{
					ResourceManagerComponent = JamGameMode->GetResourceManagerComponent();
					JobSystemComponent = JamGameMode->GetJobSystemComponent();
				}
			}
		}
	}

	// TODO :: Update to support material change to red when unable to place
	UpdateBuildingMeshMaterials(true);
}

void ABaseBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if(HasBeenPlacedInWorld && !HasSetupWorkstations)
	{
		TArray<UBillboardComponent*> WorkstationBillboards;
		GetComponents<UBillboardComponent>(WorkstationBillboards);

		// wot the fook
		for (const UBillboardComponent* WorkstationBillboard : WorkstationBillboards)
		{
			FWorkstation NewWorkstation;
			NewWorkstation.Location = WorkstationBillboard->GetComponentLocation();
			NewWorkstation.bIsOccupied = false;
		
			int32 NewWorkstationKey = WorkstationMap.Num();
			NewWorkstation.WorkStationKey = NewWorkstationKey;
		
			WorkstationMap.Add(NewWorkstationKey, NewWorkstation);

			// track amount of workstations available - This is basically the setup 
			JobReference.AvailableWorkstationsAtBuilding += 1;
		}

		for (const auto& WorkstationPair : WorkstationMap)
		{
			FWorkstation CurrentWorkstation = WorkstationPair.Value;
				
			if(ShouldDrawDebugWorkstations)
			{
				DrawDebugSphere(GetWorld(), CurrentWorkstation.Location, 50.0f, 8, FColor::Red, true, -1);

				// TODO :: Fix and figure out why the random offset compared to sphere
				//FString Text = FString::Printf(TEXT("Is Occupied: %s"), CurrentWorkstation.bIsOccupied ? TEXT("True") : TEXT("False"));
				//FVector DebugTextLoc = FVector(CurrentWorkstation.Location.X,CurrentWorkstation.Location.Y,CurrentWorkstation.Location.Z);
				//DrawDebugString(GetWorld(),CurrentWorkstation.Location,Text,this,FColor::Red,-1);
			}
		}
		
		// initialise resource stats for job
		if(ResourceManagerComponent)
		{
			// TODO :: COME BACK TO
			ResourceManagerComponent->UpdateResourcesFromJobData(JobReference);
			
			// initial setup when spawned
			ResourceManagerComponent->UpdateMaxWorkerForResource(JobReference.AssociatedResource,WorkstationMap.Num());
		}
		
		// revisit this shite
		OnBuildingPlaced();
		HasSetupWorkstations = true;
	}
}

FWorkstation ABaseBuilding::FindUnoccupiedWorkStation(AWispAIController* CurrentWorker)
{
	// early out for no reference
	if(!CurrentWorker)
	{
		// TODO :: should reset job status on character because they cant find a workstation
		return FWorkstation();
	}

	const AWispCharacter* AIPawn = Cast<AWispCharacter>(CurrentWorker->GetPawn());
	const FVector WorkerLocation = AIPawn->GetActorLocation();

	FWorkstation ClosestWorkstation;
	float ShortestDistance = TNumericLimits<float>::Max();

	for (const auto& WorkstationPair : WorkstationMap)
	{
		if (!WorkstationPair.Value.bIsOccupied)
		{
			const FVector WorkstationLocation = WorkstationPair.Value.Location;
			const float Distance = FVector::Dist(WorkerLocation, WorkstationLocation);
			if (Distance < ShortestDistance)
			{
				ShortestDistance = Distance;
				ClosestWorkstation = WorkstationPair.Value;
			}
		}
	}

	if (ShortestDistance != TNumericLimits<float>::Max())
	{
		WorkstationMap[ClosestWorkstation.WorkStationKey].bIsOccupied = true;
		return ClosestWorkstation;
	}
	
	return FWorkstation();
}

void ABaseBuilding::UpdateWorkStationStatus(int32 Index, bool IsOccupied, AAIController* AssignedWorker)
{
	WorkstationMap.Find(Index)->bIsOccupied = IsOccupied;
	WorkstationMap.Find(Index)->CurrentWorker = AssignedWorker;
	
	// default
	int32 WorkerChange = -1;
	
	// reset
	if(IsOccupied)
	{
		WorkerChange = 1;
	}

	if(ResourceManagerComponent)
	{
		ResourceManagerComponent->UpdateCurrentWorkerForResource(JobReference.AssociatedResource, WorkerChange);
	}
}

void ABaseBuilding::OnBuildingPlaced()
{
	// Job Reference Setup In BP - Except Pointer to Job Building which is assigned when its placed -> here
	JobReference.JobBuilding = this;
	JobSystemComponent->JobInstances.Add(JobReference);
}

void ABaseBuilding::UpdateBuildingMeshMaterials(bool ValidPlacement) const
{
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents<UStaticMeshComponent>(MeshComponents);

	for (int i = 0; i < MeshComponents.Num(); i++)
	{
		UStaticMeshComponent* MeshComponent = MeshComponents[i];
		const int MaterialCount = MeshComponent->GetNumMaterials();
		for (int j = 0; j < MaterialCount; j++)
		{
			if(!HasBeenPlacedInWorld)
			{
				if(ValidPlacement)
				{
					MeshComponent->SetMaterial(j, ValidBuildMaterial);
				}
				else
				{
					MeshComponent->SetMaterial(j, InvalidBuildMaterial);
				}
			}
			else
			{
				// TODO :: SET TO ORIGINAL MATERIALS
			}
		}
	}

	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
	GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);

	for (int i = 0; i < SkeletalMeshComponents.Num(); i++)
	{
		USkeletalMeshComponent* SkeletalMeshComponent = SkeletalMeshComponents[i];
		if(!HasBeenPlacedInWorld)
		{
			SkeletalMeshComponent->SetVisibility(false);
		}
		else
		{
			SkeletalMeshComponent->SetVisibility(true);
		}
	}
}



