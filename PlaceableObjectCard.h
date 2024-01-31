// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaceableObject.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "PlaceableObjectCard.generated.h"

/**
 * 
 */
UCLASS()
class CITYBUILDER_API UPlaceableObjectCard : public UUserWidget
{
	GENERATED_BODY()

public:

	// UI THINGS
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* PlaceableObjectButton;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* PlaceableObjectName;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* PlaceableObjectImage;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UVerticalBox* ResourceCostsVerticalBox;

	// Placeable Object that will be spawned when this card is clicked
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category="Building")
	TSubclassOf<APlaceableObject> PlaceableObjectClass;

protected:
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonClicked();

	UFUNCTION(BlueprintImplementableEvent)
	void SetupButtonInfo();
public:
	
};
