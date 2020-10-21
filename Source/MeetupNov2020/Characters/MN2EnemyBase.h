// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "MN2EnemyBase.generated.h"

UENUM()
enum class EMovementDirection : uint8
{
    LeftToRight,
	RightToLeft
};

UCLASS()
class MEETUPNOV2020_API AMN2EnemyBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "MN2|Stats", meta = (DisplayName = "CurrentHealth", AllowPrivateAccess = true))
		float m_Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Stats", meta = (DisplayName = "MaximumHealth", AllowPrivateAccess = true, ClampMin = "0", ClampMax = "200"))
		float m_MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Stats", meta = (DisplayName = "FireRate", AllowPrivateAccess = true, ClampMin = "0.1", ClampMax = "0.8"))
		float m_FireRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Stats", meta = (DisplayName = "HealthGlobeProbability", AllowPrivateAccess = true, ClampMin = "0.1", ClampMax = "0.8"))
		float m_HealthGlobeProbability;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Stats", meta = (DisplayName = "EnemyScoreValue", AllowPrivateAccess = true, ClampMin = "10", ClampMax = "1000"))
		int m_ScoreValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Stats", meta = (DisplayName = "EngagementDistance", AllowPrivateAccess = true, ClampMin = "10", ClampMax = "1000"))
		int m_EngagementDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Animations", meta = (DisplayName = "MovementOffsetAtSpawn", AllowPrivateAccess = true, ClampMin = "100", ClampMax = "300"))
		float m_MovementOffsetAtSpawn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Animations", meta = (DisplayName = "SpawnAnimationCurve", AllowPrivateAccess = true))
		class UCurveFloat* m_SpawnAnimationCurve;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Movement", meta = (DisplayName = "MinMovementSpeed", AllowPrivateAccess = true, ClampMin = "300", ClampMax = "1000"))
		float m_MinMovementSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Movement", meta = (DisplayName = "MaxMovementSpeed", AllowPrivateAccess = true, ClampMin = "300", ClampMax = "1000"))
		float m_MaxMovementSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Audio", meta = (DisplayName = "GunFireSound", AllowPrivateAccess = true))
		class USoundBase* m_GunFireSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Audio", meta = (DisplayName = "GunFireDelay", AllowPrivateAccess = true, ClampMin = "-2.0", ClampMax = "2.0"))
		float m_GunFireDelay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Audio", meta = (DisplayName = "DeathSound", AllowPrivateAccess = true))
		class USoundBase* m_DeathSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Spawns", meta = (DisplayName = "GunFireProjectile", AllowPrivateAccess = true))
		class TSubclassOf<AActor> m_GunFireProjectile;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Spawns", meta = (DisplayName = "HealthGlobe", AllowPrivateAccess = true))
		class TSubclassOf<AActor> m_HealthGlobe;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Particles", meta = (DisplayName = "ParticlesOnDeath", AllowPrivateAccess = true))
		class UParticleSystem* m_DeathEmitter;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|Status", meta = (DisplayName = "CanMove", AllowPrivateAccess = true))
		bool m_CanMove;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|Status", meta = (DisplayName = "MovementDirection", AllowPrivateAccess = true))
		EMovementDirection m_MovementDirection;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|Status", meta = (DisplayName = "CurrentMovementSpeed", AllowPrivateAccess = true))
		float m_CurrentMovementSpeed;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|BeginPlayData", meta = (DisplayName = "OriginalLocation", AllowPrivateAccess = true))
		FVector m_SpawnLocation;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|Animations", meta = (DisplayName = "AnimationTimeline", AllowPrivateAccess = true))
		class UTimelineComponent* m_AnimationTimeline;

	class AMN2GameMode* m_GameMode;

	FOnTimelineFloat m_SpawnAnimationUpdateDelegate{};
	FOnTimelineEvent m_SpawnAnimationFinishDelegate{};

	FTimerHandle m_FireActionRepeat;

public:	
	AMN2EnemyBase();
	virtual void Tick(float DeltaTime) override;

	void StartFiring();
	void StopFiring();

	float GetEngagementDistance() const { return m_EngagementDistance; }

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnFireAction();
	UFUNCTION()
		void OnSpawnAnimationUpdate(float delta);
	UFUNCTION()
		void OnSpawnAnimationFinish();

	UFUNCTION()
		void OnBoxCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void OnTakeAnyDamageDelegate(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void _FireProjectile() const;
	void _PlayGunfire() const;

};
