// Copyright Epic Games, Inc. All Rights Reserved.

#include "CityBuilderGameModeBase.h"

UJobSystemComponent* ACityBuilderGameModeBase::GetJobSystemComponent() const
{
	return JobSystemComponent;
}

UResourceManagerComponent* ACityBuilderGameModeBase::GetResourceManagerComponent() const
{
	return ResourceManagerComponent;
}

UTimeManagerComponent* ACityBuilderGameModeBase::GetTimeManagerComponent() const
{
	return TimeManagerComponent;
}

UCitizenManager* ACityBuilderGameModeBase::GetCitizenManagerComponent() const
{
	return CitizenManager;
}

void ACityBuilderGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}
