// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"

// Sets default values
APlayerChar::APlayerChar()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initial setup of camera component
	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Cam"));

	// Attaching camera to character mesh and head
	PlayerCamComp->SetupAttachment(GetMesh(), "head");

	// Share rotation with controller
	PlayerCamComp->bUsePawnControlRotation = true;



}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();

	// Declares timer handle for stat updates
	FTimerHandle StatsTimerHandle;

	// Calls DecreaseStats every 2 seconds
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &APlayerChar::DecreaseStats, 2.0f, true);

}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::AddControllerYawInput);
	PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerChar::StartJump);
	PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerChar::StopJump);

}

// Moves player forward or backward based on input
void APlayerChar::MoveForward(float axisValue)
{
	// Get the forward direction relative to the player's current control rotation
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);

	// Move the character in the calculated forward direction
	AddMovementInput(Direction, axisValue);
}

// Moves player left or right based on input
void APlayerChar::MoveRight(float axisValue)
{
	// Get the right direction relative to the player's current control rotation
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);

	// Move the character in the calculated right direction
	AddMovementInput(Direction, axisValue);
}

// Starts the jump action
void APlayerChar::StartJump()
{
	// Set jump flag to true
	bPressedJump = true;
}

// Stops the jump action
void APlayerChar::StopJump()
{
	// Set the jump flag to false
	bPressedJump = false;
}

void APlayerChar::FindObject()
{

}

// Increases health as long as it does not exceed 100
void APlayerChar::SetHealth(float amount)
{
	if (Health + amount < 100)
	{
		Health = Health + amount;
	}
}

// Increases hunger as long as it does not exceed 100
void APlayerChar::SetHunger(float amount)
{
	if (Hunger + amount < 100)
	{
		Hunger = Hunger + amount;
	}
}

// Increases stamina as long as it does not exceed 100
void APlayerChar::SetStamina(float amount)
{
	if (Stamina + amount < 100)
	{
		Stamina = Stamina + amount;
	}
}

// Over time, decreases hunger, restores stamina, and decreases health if hunger is 0
void APlayerChar::DecreaseStats()
{
	// Reduces hunger
	if (Hunger > 0)
	{
		SetHunger(-1.0f);
	}

	// Increases stamina
	SetStamina(10.0f);

	// Reduces health when hunger is 0
	if (Hunger <= 0)
	{
		SetHealth(-3.0f);
	}
}

