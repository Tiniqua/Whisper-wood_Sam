// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableObject.h"

// Sets default values
APlaceableObject::APlaceableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Default cost of 100 wood per building
	PlaceableObjectInfo.ResourceCosts.Add(EResourceType::Skyroot, 100);

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	//ObjectMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SetRootComponent(ObjectMesh);

	// Ignore collision channel of mouse
	ObjectMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);

	BuildingCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Building Collision"));
	BuildingCollision->InitSphereRadius(700.f);
	// Ignore all channels apart from Dynamic - To be used for building collision
	BuildingCollision->SetCollisionObjectType(ECC_WorldDynamic);
	BuildingCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BuildingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
	BuildingCollision->AttachToComponent(ObjectMesh, FAttachmentTransformRules::KeepRelativeTransform);
	
	if (BuildStages.Num() > 0)
	{
		ObjectMesh->SetStaticMesh(BuildStages[BuildStages.Num()]);
	}
}

// Called when the game starts or when spawned
void APlaceableObject::BeginPlay()
{
	Super::BeginPlay();

	HasBeenPlacedInWorld = false;

	BuildingCollision->OnComponentBeginOverlap.AddDynamic(this, &APlaceableObject::OnBeginOverlap);
	BuildingCollision->OnComponentEndOverlap.AddDynamic(this, &APlaceableObject::OnEndOverlap);

	BuildingMaterials = ObjectMesh->GetMaterials();
	
}

// Called every frame
void APlaceableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!HasBeenPlacedInWorld)
	{
		FString value = "Building Colliding";
		if(IsValidPlacement)
		{
			value = "Building OK";
		}
		GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow, FString::Printf(TEXT("%s"), *value));
	}

	UpdatePlaceableMesh(IsValidPlacement, HasBeenPlacedInWorld);
}

void APlaceableObject::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != this)
	{
		IsValidPlacement = false;	
	}
}

void APlaceableObject::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IsValidPlacement = true;
}

void APlaceableObject::UpdatePlaceableMesh(const bool ValidPlacement, bool IsPlaced) const
{
	if(IsValid(ValidBuildMaterial) && IsValid(InvalidBuildMaterial))
	if(IsPlaced)
	{
		for (int i = 0; i < BuildingMaterials.Num(); i++)
		{
			ObjectMesh->SetMaterial(i, BuildingMaterials[i]);
		}
		
		return;
	}

	if(ValidPlacement)
	{
		for (int i = 0; i < BuildingMaterials.Num(); i++)
		{
			ObjectMesh->SetMaterial(i, ValidBuildMaterial);
		}

		return;
	}

	for (int i = 0; i < BuildingMaterials.Num(); i++)
	{
		ObjectMesh->SetMaterial(i, InvalidBuildMaterial);
	}
}

