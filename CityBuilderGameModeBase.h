// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ResourceManagerComponent.h"
#include "JobSystemComponent.h"
#include "TimeManagerComponent.h"
#include "CitizenManager.h"
#include "CityHUD.h"
#include "CityPlayerController.h"
#include "CityBuilderGameModeBase.generated.h"

UCLASS()
class CITYBUILDER_API ACityBuilderGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UJobSystemComponent* JobSystemComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UResourceManagerComponent* ResourceManagerComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UTimeManagerComponent* TimeManagerComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UCitizenManager* CitizenManager = nullptr;
	
	ACityBuilderGameModeBase()
	{
		JobSystemComponent = CreateDefaultSubobject<UJobSystemComponent>(TEXT("JobSystemComponent"));
		ResourceManagerComponent = CreateDefaultSubobject<UResourceManagerComponent>(TEXT("ResourceManagerComponent"));
		TimeManagerComponent = CreateDefaultSubobject<UTimeManagerComponent>(TEXT("TimeManagerComponent"));
		CitizenManager = CreateDefaultSubobject<UCitizenManager>(TEXT("CitizenManagerComponent"));
	}

	UFUNCTION(BlueprintCallable)
	UJobSystemComponent* GetJobSystemComponent() const;

	UFUNCTION(BlueprintCallable)
	UResourceManagerComponent* GetResourceManagerComponent() const;

	UFUNCTION(BlueprintCallable)
	UTimeManagerComponent* GetTimeManagerComponent() const;

	UFUNCTION(BlueprintCallable)
	UCitizenManager* GetCitizenManagerComponent() const;

	virtual void BeginPlay() override;

	UPROPERTY()
	ACityPlayerController* CurrentPlayerController = nullptr;
};

