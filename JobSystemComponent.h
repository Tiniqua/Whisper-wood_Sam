
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ResourceTypes.h"
#include "Components/ActorComponent.h"
#include "JobSystemComponent.generated.h"

class AWispAIController;
class ABaseBuilding;

UENUM(BlueprintType)
enum class EJobType : uint8
{
	Farmer,
	Miner,
	Healer,
	Scout,
	Gatherer,
	Hunter,
	Fisher,
	Blacksmith,
	Cook,
	None
};

USTRUCT(BlueprintType)
struct
FJob
{
	GENERATED_USTRUCT_BODY()

	// Job Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job")
	FString JobName= "";

	// Job Type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job")
	EJobType JobType = EJobType::None;

	// TODO :: Expand system for upgrading buildings
	UPROPERTY(BlueprintReadOnly, Category = "Job")
	int AvailableWorkstationsAtBuilding = 0;

	// Reference to building Job is at
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job")
	AActor* JobBuilding = nullptr;

	// Amount of resources the job instance generates
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job")
	int32 ResourceGeneration = 0;

	// Upkeep cost for running this building
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job")
	int32 UpkeepCost = 0;

	// Resource this Job manufactures
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job")
	EResourceType AssociatedResource = EResourceType::None;

	// ------------------------------------------------------- //
	
	// TODO :: not currently tracked
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job")
	TArray<AAIController*> CurrentWorkers = TArray<AAIController*>();;

	// TODO :: not currently tracked
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job")
	AActor* CurrentWorkStation = nullptr;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CITYBUILDER_API UJobSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UJobSystemComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job System")
	TArray<FJob> JobInstances;

	UFUNCTION(BlueprintCallable, Category = "Job System")
	void UpdateWorkerJob(AWispAIController* WispAIController, bool ShouldFire);

	UFUNCTION(BlueprintCallable, Category = "Job System")
	TArray<FJob> ReturnAllJobData();
};
