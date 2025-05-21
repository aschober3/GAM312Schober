// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
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

};
