// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JobSystemComponent.h"
#include "ResourceTypes.h"
#include "ResourceManagerComponent.generated.h"

USTRUCT(BlueprintType)
struct FResourceData
{
	GENERATED_USTRUCT_BODY()
	
	float CurrentAmount = 0.f;
	float MaxAmount = 0.f;
	float AmountPerSecond = 0.f;
	float Upkeep = 0.f;
	float TotalAmountPerSecond = 0.f;
	int32 TotalJobsInResource = 0;
	
	// currently working at a workstation
	int32 CurrentWorkers = 0;
	// tracks workers who are assigned the job but may not be there yet
	int32 AssignedWorkers = 0;
	// player assigned amount of workers
	int32 AllowedWorkers = 0;
	// total amount of workstations
	int32 MaxWorkers = 0;

	// track current controllers in this field
	UPROPERTY()
	TArray<AAIController*> WorkerReferences;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CITYBUILDER_API UResourceManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UResourceManagerComponent();

	UPROPERTY()
	UJobSystemComponent* JobSystemComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	TMap<EResourceType, FResourceData> Resources;

	// default maximums 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultMaximum")
	float MaxStorage = 3000;

	UPROPERTY()
	FTimerHandle ResourceLoopHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	float LoopCalculationInterval = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool ShouldDrawDebugResources = true;

	// Track wisps in world
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wisps")
	int32 TotalWisps = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wisps")
	int32 TotalWorkingWisps = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Wisps")
	int32 GlobalUpkeep = 0;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	void CalculateResources();

	UFUNCTION()
	void UpdateCurrentWorkerForResource(EResourceType Resource, int32 WorkerChange);

	UFUNCTION()
	void UpdateResourcesFromJobData(FJob JobReference);

	UFUNCTION()
	void UpdateMaxWorkerForResource(EResourceType Resource, int32 WorkerChange);

	// PUBLIC BLUEPRINT FUNCTIONS FOR UPDATING UI DATA ---------------------------------------------------
	
	UFUNCTION(BlueprintCallable)
	int32 GetResourceAmount(EResourceType Resource);
	
	UFUNCTION(BlueprintCallable)
	int32 GetAllowedWorkersInResource(EResourceType Resource);

	UFUNCTION(BlueprintCallable)
	int32 GetMaxWorkersInResource(EResourceType Resource);
	
	
};

