// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "JobSystemComponent.h"
#include "BaseBuilding.h"
#include "WispAIController.generated.h"

UCLASS()
class CITYBUILDER_API AWispAIController : public AAIController
{
	GENERATED_BODY()

public:
	AWispAIController();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job System")
	UJobSystemComponent* JobSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job System")
	FJob CurrentJob = FJob();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job")
	FWorkstation WorkstationData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job")
	ABaseBuilding* CurrentWorkBuilding = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job")
	ABaseBuilding* CurrentHomeBuilding = nullptr;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWorkBuilding(AActor* WorkBuilding);
};
