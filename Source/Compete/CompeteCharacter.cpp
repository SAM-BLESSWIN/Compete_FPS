// Copyright Epic Games, Inc. All Rights Reserved.

#include "CompeteCharacter.h"
#include "CompeteProjectile.h"
#include "TP_WeaponComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


//////////////////////////////////////////////////////////////////////////
// ACompeteCharacter

ACompeteCharacter::ACompeteCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	TurnRateGamepad = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
}

void ACompeteCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

}

//////////////////////////////////////////////////////////////////////////// Input

void ACompeteCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &ACompeteCharacter::OnPrimaryAction);


	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ACompeteCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ACompeteCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ACompeteCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ACompeteCharacter::LookUpAtRate);
}

void ACompeteCharacter::OnPrimaryAction()
{
	// Trigger the OnItemUsed Event
	OnUseItem.Broadcast();
}

void ACompeteCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ACompeteCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ACompeteCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ACompeteCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ACompeteCharacter::Fired(UTP_WeaponComponent* Weapon,FVector SpawnLocation, FRotator SpawnRotation)
{
	if (Weapon == nullptr) return;

	if (!GetWorld()->IsServer())
	{//ON THE CLIENT
		UE_LOG(LogTemp, Warning, TEXT("Client call to Server!!"));

		/*projectile spawned locally by WeaponComponent*/
		/*Data are passed to server*/
		/*Projectile spawned on remote, Play fire sound and animation to all : Multicast by server*/

		//RPC Call
		Server_OnFire(Weapon,SpawnLocation,SpawnRotation);
	}
	else
	{//ON THE SERVER

		/*projectile spawned locally by WeaponComponent*/
		/*Projectile spawned on remote, Play fire sound and animation to all : Multicast by server*/
		Multi_OnFire(Weapon, SpawnLocation, SpawnRotation);
	}
}


bool ACompeteCharacter::Server_OnFire_Validate(UTP_WeaponComponent* Weapon,FVector Location, FRotator Rotation)
{
	return true;
}

void ACompeteCharacter::Server_OnFire_Implementation(UTP_WeaponComponent* Weapon,FVector Location, FRotator Rotation)
{
	UE_LOG(LogTemp, Warning, TEXT("Server Call!!"));
	Multi_OnFire(Weapon, Location, Rotation);
}


bool ACompeteCharacter::Multi_OnFire_Validate(UTP_WeaponComponent* Weapon, FVector Location, FRotator Rotation)
{
	return true;
}

void ACompeteCharacter::Multi_OnFire_Implementation(UTP_WeaponComponent* Weapon, FVector Location, FRotator Rotation)
{
	if (!IsLocallyControlled())
	{//ON THE REMOTE SYSTEM

		//Spawn Projectile
		Weapon->SpawnProjectile(Location, Rotation);

		//Play Sound
		Weapon->PlayFireSound();

		//Play Anim
		Weapon->PlayFireAnimation();
	}
}
