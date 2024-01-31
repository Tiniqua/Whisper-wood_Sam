// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraPawn.h"
#include "PlaceableObject.h"
#include "GameFramework/PlayerController.h"
#include "CityPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CITYBUILDER_API ACityPlayerController : public APlayerController
{
	GENERATED_BODY()

	// Overrides
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// Input
	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void ZoomInCamera();

	UFUNCTION()
	void ZoomOutCamera();

	UFUNCTION()
	void ClickLeftMouse();

	UFUNCTION()
	void RotateCamera(float Value);


	UFUNCTION()
	void ZoomCamera(float ZoomModifier);
	void RotateObject(float Value);

	UFUNCTION()
	void UpdateCameraLocation(FVector Value) const;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void UpdateSelection();

	UPROPERTY()
	ACameraPawn* ControlledPawn = nullptr;

public:
	
	UPROPERTY()
	FVector2D startMousePosition;
	
	UPROPERTY()
	FVector2D currentMousePosition;

	UFUNCTION(BlueprintCallable)
	void AddSelectedActorsToCurrentSelectedActors(TArray<AActor*>& SelectedActors);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	bool UsingFPCamera = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float CameraScrollSpeed = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float CameraZoomSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float CameraRotationSpeed = 10.0f;

	// Reference to the current object we're trying to build
	UPROPERTY()
	APlaceableObject* CurrentPlaceableObject;

	UPROPERTY()
	bool isBuilding = false;

	UPROPERTY()
	bool isSelecting = false;

	UPROPERTY()
	bool CanSelect = false;

	UPROPERTY()
	TArray<AActor*> CurrentSelectedActors;
};
