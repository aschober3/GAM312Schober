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

	// Set the size of the ResourcesArray and BuilidngArray to hold 3 resource values
	ResourcesArray.SetNum(3);
	BuildingArray.SetNum(3);

	// Add resource names to the ResourcesNameArray
	ResourcesNameArray.Add(TEXT("Wood"));  
	ResourcesNameArray.Add(TEXT("Stone"));  
	ResourcesNameArray.Add(TEXT("Berry")); 

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

	// Checks if the player is building
	if (isBuilding)
	{
		if (spawnedPart) // Checks for existing building part
		{
			// Calculates point 400 units from the player camera and moves build part to the location
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() *400.0f;
			FVector EndLocation = StartLocation + Direction;
			spawnedPart->SetActorLocation(EndLocation);
		}
	}

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
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerChar::FindObject);
	PlayerInputComponent->BindAction("RotPart", IE_Pressed, this, &APlayerChar::RotateBuilding);


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

// Performs a forward line trace to detect and interact with a resource object
void APlayerChar::FindObject()
{
	// Set up hit result and trace start/end locations based on the camera
	FHitResult HitResult;
	FVector StartLocation = PlayerCamComp->GetComponentLocation();
	FVector Direction = PlayerCamComp->GetForwardVector() * 800.0f;
	FVector EndLocation = StartLocation + Direction;

	// Set up collision parameters
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnFaceIndex = true;

	if (!isBuilding)
	{
		// Perform the line trace along the camera's forward direction
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
		{
			// Attempt to cast the hit actor to a resource type
			AResource_M* HitResource = Cast<AResource_M>(HitResult.GetActor());

			// Only proceed if the player has enough stamina
			if (Stamina > 5.0f)
			{
				if (HitResource)
				{
					// Get the resource type and value from the hit object
					FString hitName = HitResource->resourceName;
					int resourceValue = HitResource->resourceAmount;

					// Subtract collected amount from the total remaining in the resource
					HitResource->totalResouce = HitResource->totalResouce - resourceValue;

					// If resource still has enough left, give it to the player
					if (HitResource->totalResouce > resourceValue)
					{
						GiveResource(resourceValue, hitName);

						// Show on-screen message for feedback
						check(GEngine != nullptr)
							GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Collected"));

						// Spawn a decal at the hit location for visual feedback
						UGameplayStatics::SpawnDecalAtLocation(GetWorld(), hitDecal, FVector(10.0f, 10.0f, 10.0f), HitResult.Location, FRotator(-90, 0, 0), 2.0f);

						// Reduce player's stamina for interacting with the object
						SetStamina(-5.0f);
					}
					else
					{
						// If the resource is depleted, destroy it
						HitResource->Destroy();
						check(GEngine != nullptr);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Depleted"));
					}
				}
			}
		}
	}	

	else
	{
		isBuilding = false;
	
	}

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

// Adds the specified amount to the appropriate resource based on the resource type string
void APlayerChar::GiveResource(float amount, FString resourceType)
{
	// Add to wood if the resource type is "Wood"
	if (resourceType == "Wood")
	{
		ResourcesArray[0] = ResourcesArray[0] + amount;
	}

	// Add to stone if the resource type is "Stone"
	if (resourceType == "Stone")
	{
		ResourcesArray[1] = ResourcesArray[1] + amount;
	}

	// Add to berries if the resource type is "Berry"
	if (resourceType == "Berry")
	{
		ResourcesArray[2] = ResourcesArray[2] + amount;
	}
}

// Reduces resources and increases building count if conditions are met
void APlayerChar::UpdateResources(float woodAmount, float stoneAmount, FString buildingObject)
{
	if (woodAmount <= ResourcesArray[0]) // Check is player has enough wood
	{
		if (stoneAmount <= ResourcesArray[1]) // Check if player has enough stone
		{
			// Reduces resources
			ResourcesArray[0] = ResourcesArray[0] - woodAmount;
			ResourcesArray[1] = ResourcesArray[1] - stoneAmount;

			// Increment count of built objects
			if (buildingObject == "Wall")
			{
				BuildingArray[0] = BuildingArray[0] + 1;
			}

			if (buildingObject == "Floor")
			{
				BuildingArray[1] = BuildingArray[1] + 1;
			}

			if (buildingObject == "Ceiling")
			{
				BuildingArray[2] = BuildingArray[2] + 1;
			}
		}
	
	}
}

// Spawns building part if available and player is not currently building
void APlayerChar::SpawnBuilding(int buildingID, bool& isSuccess)
{
	if (!isBuilding) // Onlu allow one build at once
	{
		if (BuildingArray[buildingID] >= 1) // Check if player has building part
		{
			isBuilding = true; // Set building to true

			//Set spawn location for building part in front of the player
			FActorSpawnParameters SpawnParams;
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction;
			FRotator myRot(0, 0, 0);

			// Decrease available count and spawn building part
			BuildingArray[buildingID] = BuildingArray[buildingID] - 1;
			spawnedPart = GetWorld()->SpawnActor<ABuildingPart>(BuildPartClass, EndLocation, myRot, SpawnParams);

			isSuccess = true;
		}

		isSuccess = false; // Building failed
	}
}

// Rotates building part 90 degrees
void APlayerChar::RotateBuilding()
{
	if (isBuilding) // Only rotate if a part is being placed
	{
		spawnedPart->AddActorWorldRotation(FRotator(0, 90, 0));
	}
}

