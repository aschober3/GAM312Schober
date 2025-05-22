// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Resource_M.generated.h"

UCLASS()
class GAM312_SCHOBER_API AResource_M : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResource_M();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Name of the resource
	UPROPERTY(EditAnywhere)
	FString resourceName = "Wood";

	// Amount of resource given per interaction
	UPROPERTY(EditAnywhere)
	int resourceAmount = 5;

	// Total amount of the resource available in the object
	UPROPERTY(EditAnywhere)
	int totalResouce = 100;

	// Temporary text holder
	UPROPERTY()
	FText tempText;

	// Text render component to display the resource name in the world
	UPROPERTY(EditAnywhere)
	UTextRenderComponent* ResourceNameTxt;

	// Static mesh representing the resource in the game world
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

};
