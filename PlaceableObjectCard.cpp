// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableObjectCard.h"
#include "CityPlayerController.h"

void UPlaceableObjectCard::NativeConstruct()
{
	Super::NativeConstruct();
	SetupButtonInfo();

	PlaceableObjectButton->OnClicked.AddDynamic(this, &UPlaceableObjectCard::OnButtonClicked);

}

void UPlaceableObjectCard::OnButtonClicked()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		ACityPlayerController* PC = Cast<ACityPlayerController>(World->GetFirstPlayerController());
		if (IsValid(PC))
		{
			if(PC->isBuilding)
			{
				return;
			}
			
			if (IsValid(PlaceableObjectClass))
			{
				//SPAWN ACTOR
				FActorSpawnParameters SpawnParams;
				PC->CurrentPlaceableObject = World->SpawnActor<APlaceableObject>(PlaceableObjectClass, SpawnParams);
				PC->isBuilding = true;
			}
		}
	}
}
