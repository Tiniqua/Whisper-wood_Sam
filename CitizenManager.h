// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceManagerComponent.h"
#include "Components/ActorComponent.h"
#include "CitizenManager.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CITYBUILDER_API UCitizenManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCitizenManager();

	// people who can work for this resource
	UFUNCTION(BlueprintCallable)
	void SetAllowedToWorkOnResource(EResourceType Resource, int32 Change) const;

	// Will be used for building costs system
	UFUNCTION(BlueprintCallable)
	void UpdateResourceAmount(EResourceType Resource, int32 Change) const;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	UResourceManagerComponent* ResourceManagerComponent = nullptr;

	UPROPERTY()
	UJobSystemComponent* JobSystemComponent = nullptr;
};
