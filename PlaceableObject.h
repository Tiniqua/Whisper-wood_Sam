// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceTypes.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "PlaceableObject.generated.h"

UENUM(BlueprintType)
enum class EPlaceableType : uint8
{
	House,
	ResourceGen,
	Decoration,
	None
};

USTRUCT(BlueprintType)
// Used for the build system and menu
struct FPlaceableObjectInfo
{
	GENERATED_BODY()

	// Placeable Building Category
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building")
	EPlaceableType PlaceableType = EPlaceableType::None;

	// Resource Cost to place building
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	TMap<EResourceType, int32> ResourceCosts;

	// Building Icon for Build Menu
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	UTexture2D* BuildingImage = nullptr;

	// Name of Building
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	FString BuildingName = "Building";

	// Building Description
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	FString BuildingDescription = "This building does a thing!";
};

UCLASS()
class CITYBUILDER_API APlaceableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaceableObject();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ObjectMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building")
	USphereComponent* BuildingCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building")
	FPlaceableObjectInfo PlaceableObjectInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Building")
	UMaterialInterface* ValidBuildMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Building")
	UMaterialInterface* InvalidBuildMaterial = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool HasBeenPlacedInWorld = false;

	UPROPERTY()
	bool IsValidPlacement = true;

	// Different Build Stages (Un-built/Part-Built/Built)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building")
	TArray<UStaticMesh*> BuildStages;

	UPROPERTY()
	TArray<UMaterialInterface*> BuildingMaterials;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable)
	void UpdatePlaceableMesh(bool ValidPlacement, bool IsPlaced) const;

};
