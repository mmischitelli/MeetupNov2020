// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MN2Asteroid.generated.h"

UCLASS()
class MEETUPNOV2020_API AMN2Asteroid : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "MN2|Stats", meta = (DisplayName = "CurrentHealth", AllowPrivateAccess = true))
		float m_Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Stats", meta = (DisplayName = "MaximumHealth", AllowPrivateAccess = true, ClampMin = "0", ClampMax = "200"))
		float m_MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Stats", meta = (DisplayName = "EnemyScoreValue", AllowPrivateAccess = true, ClampMin = "10", ClampMax = "1000"))
		int m_ScoreValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Movement", meta = (DisplayName = "MinMovementSpeed", AllowPrivateAccess = true, ClampMin = "300", ClampMax = "1000"))
		float m_MinMovementSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Movement", meta = (DisplayName = "MaxMovementSpeed", AllowPrivateAccess = true, ClampMin = "300", ClampMax = "1000"))
		float m_MaxMovementSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Geometry", meta = (DisplayName = "ObjectScale", AllowPrivateAccess = true))
		FVector m_Scale;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Audio", meta = (DisplayName = "DeathSound", AllowPrivateAccess = true))
		class USoundBase* m_DeathSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Particles", meta = (DisplayName = "ParticlesOnDeath", AllowPrivateAccess = true))
		class UParticleSystem* m_DeathEmitter;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|Status", meta = (DisplayName = "CurrentMovementSpeed", AllowPrivateAccess = true))
		float m_CurrentMovementSpeed;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|BeginPlayData", meta = (DisplayName = "OriginalLocation", AllowPrivateAccess = true))
		FVector m_SpawnLocation;
	
public:	
	// Sets default values for this actor's properties
	AMN2Asteroid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UFUNCTION()
		void OnCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void OnTakeAnyDamageDelegate(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

};
