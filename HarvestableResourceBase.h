// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceTypes.h"
#include "GameFramework/Actor.h"
#include "HarvestableResourceBase.generated.h"

class UResourceManagerComponent;

USTRUCT(BlueprintType)
struct FHarvestableResourceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Harvestable Resource")
	EResourceType AssociatedResource = EResourceType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Harvestable Resource")
	float ResourceAmount = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Harvestable Resource")
	float HarvestDuration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Harvestable Resource")
	bool bCanRegrow = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Harvestable Resource")
	USoundBase* HarvestSound = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Harvestable Resource")
	//UNiagaraSystem* HarvestParticles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Harvestable Resource")
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Harvestable Resource")
	FText TooltipText = FText();
};

UCLASS()
class CITYBUILDER_API AHarvestableResourceBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHarvestableResourceBase();

	UPROPERTY()
	UStaticMeshComponent* ResourceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHarvestableResourceData ResourceData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBeingHarvested = false;

	UFUNCTION(BlueprintCallable)
	void StartHarvesting();

	UFUNCTION(BlueprintCallable)
	void HarvestTick();

	UFUNCTION(BlueprintCallable)
	void StopHarvesting();


protected:


private:
	FTimerHandle HarvestHandle;

	UResourceManagerComponent* ResourceManagerComponent = nullptr;
	
};
