// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "ProjectilePlayerBase.generated.h"

UCLASS()
class MEETUPNOV2020_API AProjectilePlayerBase : public AProjectileBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Particles", meta = (DisplayName = "ParticlesOnHit", AllowPrivateAccess = true))
		class UParticleSystem* m_Emitter;

public:
	// Sets default values for this actor's properties
	AProjectilePlayerBase();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void OnProjectileOverlapsDelegate_Impl(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
