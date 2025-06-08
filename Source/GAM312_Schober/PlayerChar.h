// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Resource_M.h"
#include "Kismet/GameplayStatics.h"
#include "BuildingPart.h"
#include "PlayerWidget.h"
#include "ObjectiveWidget.h"
#include "PlayerChar.generated.h"

UCLASS()
class GAM312_SCHOBER_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Declares the MoveForward function
	UFUNCTION()
		void MoveForward(float axisValue);

	// Declares the MoveRight function
	UFUNCTION()
		void MoveRight(float axisValue);

	// Declares the StartJump function
	UFUNCTION()
		void StartJump();

	// Declares the StopJump function
	UFUNCTION()
		void StopJump();

	// Declares the FindObject function
	UFUNCTION()
		void FindObject();

	// Declares a visible camera component attached to the player
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* PlayerCamComp;

	// Health value of the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Health = 100.0f;

	// Hunger value of the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Hunger = 100.0f;

	// Stamina value of the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Stamina = 100.0f;

	// Amount of wood the player has
	UPROPERTY(EditAnywhere, Category = "Resources")
		int Wood;

	// Amount of stone the player has
	UPROPERTY(EditAnywhere, Category = "Resources")
		int Stone;

	// Amount of berries the player has
	UPROPERTY(EditAnywhere, Category = "Resources")
		int Berry;

	// Array storing resource quantities
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
		TArray<int> ResourcesArray;

	// Array storing resource names
	UPROPERTY(EditAnywhere, Category = "Resources")
		TArray<FString> ResourcesNameArray;

	// Material used for hit marker decal
	UPROPERTY(EditAnywhere, Category = "HitMarker")
		UMaterialInterface* hitDecal;

	// Stores number of each type of building part the player has
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Supplies")
		TArray<int> BuildingArray;

	// Indicates if the player is currently placing a part
	UPROPERTY()
		bool isBuilding;

	// References which building part to spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<ABuildingPart> BuildPartClass;

	// Pointer to currently spawned building part being placed
	UPROPERTY()
		ABuildingPart* spawnedPart;

	// Reference to the player's HUD widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPlayerWidget* playerUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UObjectiveWidget* objWidget;

	UPROPERTY()
		float objectsBuilt;

	UPROPERTY()
		float matsCollected;

	// Sets player health
	UFUNCTION(BlueprintCallable)
		void SetHealth(float amount);

	// Sets player hunger
	UFUNCTION(BlueprintCallable)
		void SetHunger(float amount);

	// Sets player stamina
	UFUNCTION(BlueprintCallable)
		void SetStamina(float amount);

	// Decreases stats of player
	UFUNCTION()
		void DecreaseStats();

	// Gives the player a specified amount of a resource
	UFUNCTION()
		void GiveResource(float amount, FString resourceType);

	// Reduces specified wood and stone amount, increments building part count
	UFUNCTION(BlueprintCallable)
		void UpdateResources(float woodAmount, float stoneAmount, FString buildingObject);

	// Attempts to spawn building part
	UFUNCTION(BlueprintCallable)
		void SpawnBuilding(int buildingID, bool& isSuccess);

	// Rotates the currently spawned building part
	UFUNCTION()
		void RotateBuilding();


};
