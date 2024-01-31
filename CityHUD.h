// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CityHUD.generated.h"

UCLASS()
class CITYBUILDER_API ACityHUD : public AHUD
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual void DrawHUD() override;
};
