// Fill out your copyright notice in the Description page of Project Settings.

#include "WispCharacter.h"

#include "CityBuilderGameModeBase.h"

AWispCharacter::AWispCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	WispData.Happiness = 100;
	WispData.Health = 100;
	WispData.Hunger = 100;

	WispData.HappinessChangeRate = 1;
	WispData.HappinessIncreaseValue = 5;
	WispData.HappinessDecreaseValue = 5;

	WispData.HealthChangeRate = 1;
	WispData.HealthIncreaseValue = 5;
	WispData.HealthDecreaseValue = 5;

	WispData.HungerChangeRate = 1;
	WispData.HungerIncreaseValue = 5;
	WispData.HungerDecreaseValue = 5;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));

	HeadMesh->SetupAttachment(RootComponent);
	BodyMesh->SetupAttachment(RootComponent);
}

void AWispCharacter::BeginPlay()
{
	Super::BeginPlay();

	const ACityBuilderGameModeBase* JamGameMode = nullptr;
	UWorld* World = GetWorld();
	if(World)
	{
		if (AGameModeBase* GameMode = World->GetAuthGameMode())
		{
			JamGameMode = Cast<ACityBuilderGameModeBase>(GameMode);
		}
	}

	// initialise and get a reference to the ai controller
	WispAIController = Cast<AWispAIController>(GetController());

	// update amount of wisps in the game when a wisp is spawned
	if(JamGameMode->GetResourceManagerComponent())
	{
		// TODO:: Check if we even need this to display anywhere
		JamGameMode->GetResourceManagerComponent()->TotalWisps += 1;
	}

	// TODO:: Bind event on death to remove total wisps
}

void AWispCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Update Meshes material details 
	UpdateMaterialParams();
	CharacterBobbing();
}

void AWispCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

} 

// TODO :: Move to event on happiness changed rather than Tick
void AWispCharacter::UpdateMaterialParams() const
{
	const float ScaledHappiness = WispData.Happiness * 0.5;
	const FVector ParamValue = FVector(ScaledHappiness,ScaledHappiness,ScaledHappiness);

	// TArray Wizardry with templates
	TArray ParamNames = {ParamName1, ParamName2, ParamName3};
	TArray Meshes = {BodyMesh, HeadMesh};

	// TODO :: Base Colour is the only one setup to work atm really - still needs work doesnt lerp
	for (const auto MeshInstance : Meshes)
	{
		MeshInstance->SetVectorParameterValueOnMaterials(ParamName1,ParamValue);
	}

	// TODO :: Expand material so changing these parameters is a little more graceful before using
	// for (const auto ParamName : ParamNames)
	// {
	// 	for (const auto MeshInstance : Meshes)
	// 	{
	// 		MeshInstance->SetVectorParameterValueOnMaterials(ParamName,ParamValue);
	// 	}
	// }
}

void AWispCharacter::CharacterBobbing() const
{
	TArray Meshes = {BodyMesh, HeadMesh};
		
	for (const auto MeshInstance : Meshes)
	{
		const float Time = GetGameTimeSinceCreation();
		const float ZLocation = BobDistance * FMath::Sin(Time * BobSpeed);
		FVector NewLocation = MeshInstance->GetRelativeLocation();
		NewLocation.Z = ZLocation;
		MeshInstance->SetRelativeLocation(NewLocation);
	}
}

