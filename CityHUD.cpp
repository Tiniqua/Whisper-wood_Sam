// Fill out your copyright notice in the Description page of Project Settings.

#include "CityHUD.h"
#include "CityBuilderGameModeBase.h"

void ACityHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ACityHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw the selection box on the HUD
	const ACityBuilderGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ACityBuilderGameModeBase>();
	if(GameMode != nullptr)
	{
		ACityPlayerController* CurrentController = GameMode->CurrentPlayerController;
	
		if (CurrentController != nullptr)
		{
			// TODO SAM :: can select needs to be set to true when we click "select harvest resources or something"
			if(CurrentController->isSelecting && CurrentController->CanSelect)
			{
				DrawRect(
					FColor::Red,
					CurrentController->startMousePosition.X,
					CurrentController->startMousePosition.Y,
					(CurrentController->currentMousePosition.X - CurrentController->startMousePosition.X),
					(CurrentController->currentMousePosition.Y - CurrentController->startMousePosition.Y)
					);

				// TODO SAM :: Filter to only things we want selectable, maybe use an interface?
				TArray<AActor*> ActorsInSelection;
				GetActorsInSelectionRectangle(CurrentController->startMousePosition,
					CurrentController->currentMousePosition,
					ActorsInSelection,
					false,
					false
					);

				
				CurrentController->AddSelectedActorsToCurrentSelectedActors(ActorsInSelection);
			}
		}
	}
}
