// Fill out your copyright notice in the Description page of Project Settings.


#include "HarvestableResourceBase.h"

#include "CityBuilderGameModeBase.h"
#include "ResourceManagerComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHarvestableResourceBase::AHarvestableResourceBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	ResourceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ResourceMesh"));
	RootComponent = ResourceMesh;

	// Initialize the mesh component to the mesh stored in the resource data struct
	if(IsValid(ResourceData.Mesh))
	{
		ResourceMesh->SetStaticMesh(ResourceData.Mesh);
	}

	if(!GetWorld())
	{
		return;
	}
		
	if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode())
	{
		if(const ACityBuilderGameModeBase* JamGameMode = Cast<ACityBuilderGameModeBase>(GameMode))
		{
			ResourceManagerComponent = JamGameMode->GetResourceManagerComponent();
		}
	}
}

void AHarvestableResourceBase::StartHarvesting()
{
	bIsBeingHarvested = true;
	
	GetWorldTimerManager().SetTimer(HarvestHandle, this, &AHarvestableResourceBase::HarvestTick, 1.0f, true);

	// Spawn the harvesting particle effect
	// if (ResourceData.HarvestParticles != nullptr)
	// {
	// 	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ResourceData.HarvestParticles, GetActorLocation());
	// 	if (NiagaraComponent != nullptr)
	// 	{
	// 		NiagaraComponent->SetAutoDestroy(true);
	// 	}
	// }
}

void AHarvestableResourceBase::HarvestTick()
{
	// Calculate the resource increment based on the harvest duration
	const float ResourceIncrement = ResourceData.ResourceAmount / ResourceData.HarvestDuration;
	
	ResourceManagerComponent->Resources[ResourceData.AssociatedResource].CurrentAmount += ResourceIncrement;
	
	ResourceData.ResourceAmount -= ResourceIncrement;

	if (ResourceData.ResourceAmount <= 0)
	{
		StopHarvesting();
	}
	else
	{
		// Play the harvesting sound
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ResourceData.HarvestSound, GetActorLocation());
	
		// Spawn the harvesting particle effect
		// if (ResourceData.HarvestParticles != nullptr)
		// {
		// 	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ResourceData.HarvestParticles, GetActorLocation());
		// 	if (NiagaraComponent != nullptr)
		// 	{
		// 		NiagaraComponent->SetAutoDestroy(true);
		// 	}
		// }
	}
}

void AHarvestableResourceBase::StopHarvesting()
{
	// Clear the timer handle
	GetWorldTimerManager().ClearTimer(HarvestHandle);

	// Set the harvesting state to false
	bIsBeingHarvested = false;

	Destroy();
}

