// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectiveWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAM312_SCHOBER_API UObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// Blueprint event to update the UI with the current materials collected
	UFUNCTION(BlueprintImplementableEvent)
		void UpdatematOBJ(float matsCollected);

	// Blueprint event to update the UI with the current number of objects built
	UFUNCTION(BlueprintImplementableEvent)
		void UpdatebuildOBJ(float objectsBuilt);
	
};
