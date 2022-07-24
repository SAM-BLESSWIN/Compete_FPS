// Copyright Epic Games, Inc. All Rights Reserved.

#include "CompeteProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Compete/CompeteCharacter.h"
#include "Compete/CompeteGameMode.h"
#include "Compete/CompetePlayerState.h"

ACompeteProjectile::ACompeteProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ACompeteProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ACompeteProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		Destroy();
	}
	else if (ACompeteCharacter* Character = Cast<ACompeteCharacter>(OtherActor))
	{
		//Authority is simply the one who spawned the Actor
		if (HasAuthority())  
		{
			UE_LOG(LogTemp, Warning, TEXT("Authority of Projectile"));

			if (ACompeteGameMode* GM = GetWorld()->GetAuthGameMode<ACompeteGameMode>())
			{//ON THE SERVER : Server only has GameMode
				UE_LOG(LogTemp, Warning, TEXT("Has GameMode [server]"));

				//Update GameState from GameMode
				GM->PlayerHit();

				if (ACompeteCharacter* ShootingPlayer = Cast<ACompeteCharacter>(GetOwner()))
				{
					if (ACompetePlayerState* PS = ShootingPlayer->GetPlayerState<ACompetePlayerState>())
					{
						UE_LOG(LogTemp, Warning, TEXT("ShootingPlayer [Server]"));

						//Update PlayerState
						PS->PlayerHit();
					}
				}
			}
		}
		Destroy();
	}

}