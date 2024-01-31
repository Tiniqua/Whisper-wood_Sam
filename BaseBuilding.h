// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "JobSystemComponent.h"
#include "ResourceManagerComponent.h"
#include "Engine/Texture2D.h"
#include "GameFramework/Actor.h"
#include "BaseBuilding.generated.h"


USTRUCT(BlueprintType)
struct FWorkstation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Workstation")
	bool bIsOccupied = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Workstation")
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Workstation")
	int32 WorkStationKey = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Workstation")
	AAIController* CurrentWorker = nullptr;
};

USTRUCT(BlueprintType)
struct FBuildingData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	int32 ManaCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	UTexture2D* BuildingImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	FString BuildingName = "Building";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	FString BuildingDescription = "This building does a thing!";
};

UCLASS()
class CITYBUILDER_API ABaseBuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBuilding();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool ShouldDrawDebugWorkstations = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Workstation Map")
	TMap<int32, FWorkstation> WorkstationMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	FBuildingData BuildingData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "House")
	int32 WispToSpawn = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job")
	FJob JobReference = FJob();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	UMaterialInterface* ValidBuildMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	UMaterialInterface* InvalidBuildMaterial = nullptr;

	UPROPERTY();
	UResourceManagerComponent* ResourceManagerComponent = nullptr;
	UPROPERTY();
	UJobSystemComponent* JobSystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool HasBeenPlacedInWorld = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool HasSetupWorkstations = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	FWorkstation FindUnoccupiedWorkStation(AWispAIController* CurrentWorker);

	UFUNCTION(BlueprintCallable)
	void UpdateWorkStationStatus(int32 Index, bool IsOccupied, AAIController* AssignedWorker);

	UFUNCTION()
	void OnBuildingPlaced();

	// TODO:: REMOVE BLUEPRINT CALLABLE AS IT SHOULD BE CODE ONLY WHEN BUILDING LOGIC IS MOVED TO CODE
	UFUNCTION(BlueprintCallable)
	void UpdateBuildingMeshMaterials(bool ValidPlacement) const;
};
