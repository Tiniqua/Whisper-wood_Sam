// Fill out your copyright notice in the Description page of Project Settings.


#include "CityPlayerController.h"
#include "CityBuilderGameModeBase.h"

#include "CameraPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void ACityPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ControlledPawn = Cast<ACameraPawn>(GetPawn());

	if(ACityBuilderGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ACityBuilderGameModeBase>())
	{
		GameMode->CurrentPlayerController = this;
	}
	
	// setup controller 
	SetShowMouseCursor(true);

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	
	UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(),EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
	//TODO: Mouse position resets on click (Sam had a fix?)
}

void ACityPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAxis("MoveForward", this, &ACityPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACityPlayerController::MoveRight);
	InputComponent->BindAction("Interact", IE_Pressed, this, &ACityPlayerController::ClickLeftMouse);
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ACityPlayerController::ZoomInCamera);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ACityPlayerController::ZoomOutCamera);
	InputComponent->BindAxis("RotateCamera", this, &ACityPlayerController::RotateCamera);
	InputComponent->BindAxis("RotateObject", this, &ACityPlayerController::RotateObject);
}

void ACityPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (isBuilding && IsValid(CurrentPlaceableObject))
	{
		FHitResult Hit;
		//Uses Landscape Channel
		GetHitResultUnderCursor(ECC_WorldStatic, true, Hit);

		CurrentPlaceableObject->SetActorLocation(Hit.Location);
	}

	UpdateSelection();

	GEngine->AddOnScreenDebugMessage(-1, .0f, FColor::Yellow, FString::Printf(TEXT("Player Transform: %s"), *ControlledPawn->GetActorLocation().ToString()));
}

void ACityPlayerController::UpdateSelection()
{
	if (IsInputKeyDown(EKeys::LeftMouseButton))
	{
		isSelecting = true;
		
		if (startMousePosition.IsZero())
		{
			FVector2D mousePosition;
			if (GetMousePosition(mousePosition.X, mousePosition.Y))
			{
				startMousePosition = mousePosition;
			}
		}

		// Get the current mouse position and calculate the size and position of the drag box
		GetMousePosition(currentMousePosition.X, currentMousePosition.Y);
	}
	else
	{
		// The left mouse button is not being held down, so set the isSelecting variable to false and reset the start mouse position
		isSelecting = false;
		startMousePosition = FVector2D::ZeroVector;
	}
}

void ACityPlayerController::AddSelectedActorsToCurrentSelectedActors(TArray<AActor*>& SelectedActors)
{
	// TODO :: Only add when we have finished the draw not for the entire lifetime of the draw
	CurrentSelectedActors = SelectedActors;
}


void ACityPlayerController::MoveForward(float Value)
{
	if(!UsingFPCamera)
	{
		const float XMovementValue =  (Value * CameraScrollSpeed) ;//* -1.0f;
		UpdateCameraLocation(FVector(XMovementValue, 0.f, 0.f));
	}
	
	//ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(),Value);
}

void ACityPlayerController::MoveRight(float Value)
{
	if(!UsingFPCamera)
	{
		const float YMovementValue =  (Value * CameraScrollSpeed);// * -1.0f;
		UpdateCameraLocation(FVector(0.0f, YMovementValue, 0.f));
	}
	
	//ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(),Value);
}

void ACityPlayerController::ZoomInCamera()
{
	ZoomCamera(CameraZoomSpeed * -1.f);
}

void ACityPlayerController::ZoomOutCamera()
{
	ZoomCamera(CameraZoomSpeed);
}

void ACityPlayerController::ZoomCamera(float ZoomModifier)
{
	if (ControlledPawn)
	{
		ControlledPawn->CameraSpringArm->TargetArmLength = ControlledPawn->CameraSpringArm->TargetArmLength + ZoomModifier;
	}
}

void ACityPlayerController::RotateObject(float Value)
{
	if(isBuilding && IsValid(CurrentPlaceableObject))
	{
		const float RotationSpeed = Value / 10.0f;
		CurrentPlaceableObject->AddActorWorldRotation(FRotator(0.f, RotationSpeed * CameraRotationSpeed, 0.f ));
	}
}

void ACityPlayerController::ClickLeftMouse()
{
	if(isBuilding && IsValid(CurrentPlaceableObject) && CurrentPlaceableObject->IsValidPlacement)
	{
		CurrentPlaceableObject->HasBeenPlacedInWorld = true;
		CurrentPlaceableObject = nullptr;
		isBuilding = false;
	}
}

void ACityPlayerController::RotateCamera(float Value)
{
	const float RotationSpeed = Value / 5.0f;
	ControlledPawn->AddActorWorldRotation(FRotator(0.f, RotationSpeed * CameraRotationSpeed, 0.f));
}

void ACityPlayerController::UpdateCameraLocation(FVector Value) const
{
	if(ControlledPawn)
	{
		const FVector CameraMovement = UKismetMathLibrary::Add_VectorVector(ControlledPawn->GetActorLocation(), UKismetMathLibrary::Quat_RotateVector(ControlledPawn->GetActorRotation().Quaternion(), Value));
		ControlledPawn->SetActorLocation(FVector(CameraMovement.X, CameraMovement.Y, ControlledPawn->GetActorLocation().Z));
	}
}
