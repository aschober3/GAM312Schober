// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAM312_SCHOBER_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Blueprint event to update HUD bars for health, hunger, and stamina
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateBars(float Health1, float Hunger1, float Stamina1);
	
};
