// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceTypes.generated.h"


UENUM(BlueprintType)
enum class EResourceType : uint8
{
	None,
	Skystone,		// Stone
	Skyroot,		// Wood
	Nectar,			// Refined Food
	ManaCrystal,	// Unique Energy Thing (for unique buildings)
	Ingredients		// Food from Farms etc that can be eaten but is bad for happiness (needs new name)
};