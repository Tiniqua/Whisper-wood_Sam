// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WispAIController.h"
#include "GameFramework/Character.h"
#include "WispCharacter.generated.h"

USTRUCT(BlueprintType)
struct FWispData
{
	GENERATED_USTRUCT_BODY()
	
	// Happiness Data ------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Happiness")
	float Happiness;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Happiness")
	float HappinessChangeRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Happiness")
	float HappinessIncreaseValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Happiness")
	float HappinessDecreaseValue;

	// Health Data ------------------------------------------------------
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthChangeRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthIncreaseValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthDecreaseValue;

	// Hunger Data ------------------------------------------------------
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger")
	float Hunger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HungerChangeRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HungerIncreaseValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HungerDecreaseValue;
};

UCLASS()
class CITYBUILDER_API AWispCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWispCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FWispData WispData = FWispData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AWispAIController* WispAIController = nullptr;

	// Mesh data ----------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* BodyMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* HeadMesh;

	// Material Param Data -----------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wisp Material Parameters")
	FName ParamName1 = "Wisp_Base_Colour";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wisp Material Parameters")
	FName ParamName2 = "Wisp_Edge_Colour";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wisp Material Parameters")
	FName ParamName3 = "Wisp_Inner_Colour";

protected:
	virtual void BeginPlay() override;

	// Character Bob Data Data -----------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Bobbing")
	float BobSpeed = 150.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Bobbing")
	float BobDistance = 10.0f;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void UpdateMaterialParams() const;

	UFUNCTION()
	void CharacterBobbing() const;
};
