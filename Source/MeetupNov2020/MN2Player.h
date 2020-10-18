// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MN2GameMode.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Pawn.h"
#include "MN2Player.generated.h"

UCLASS()
class MEETUPNOV2020_API AMN2Player : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MN2|Stats", meta = (DisplayName = "CurrentHealth", AllowPrivateAccess = true))
		float m_Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Stats", meta = (DisplayName = "MaximumHealth", AllowPrivateAccess = true, ClampMin = "0", ClampMax = "200"))
		float m_MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Stats", meta = (DisplayName = "FireRate", AllowPrivateAccess = true, ClampMin = "0.1", ClampMax = "0.8"))
		float m_FireRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Animations", meta = (DisplayName = "NumberOfSpinsAtSpawn", AllowPrivateAccess = true, ClampMin = "0", ClampMax = "4"))
		int m_NumSpinsAtSpawn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Animations", meta = (DisplayName = "MovementOffsetAtSpawn", AllowPrivateAccess = true, ClampMin = "100", ClampMax = "300"))
		float m_MovementOffsetAtSpawn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Animations", meta = (DisplayName = "SpawnAnimationCurve", AllowPrivateAccess = true))
		class UCurveFloat* m_SpawnAnimationCurve;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Movement", meta = (DisplayName = "MaxMovementSpeed", AllowPrivateAccess = true, ClampMin = "300", ClampMax = "1000"))
		float m_MaxMovementSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Audio", meta = (DisplayName = "PrimaryFireSound", AllowPrivateAccess = true))
		class USoundBase* m_PrimaryFireSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Audio", meta = (DisplayName = "SecondaryFireSound", AllowPrivateAccess = true))
		class USoundBase* m_SecondaryFireSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Audio", meta = (DisplayName = "DeathSound", AllowPrivateAccess = true))
		class USoundBase* m_DeathSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Particles", meta = (DisplayName = "PrimaryFireProjectile", AllowPrivateAccess = true))
		class TSubclassOf<AActor> m_PrimaryFireProjectile;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Particles", meta = (DisplayName = "SecondaryFireProjectile", AllowPrivateAccess = true))
		class TSubclassOf<AActor> m_SecondaryFireProjectile;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Particles", meta = (DisplayName = "ParticlesOnDeath", AllowPrivateAccess = true))
		class UParticleSystem* m_DeathEmitter;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|Status", meta = (DisplayName = "CanMove", AllowPrivateAccess = true))
		bool m_CanMove;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|Status", meta = (DisplayName = "CurrentSpeedVector", AllowPrivateAccess = true))
		FVector2D m_CurrentMovementSpeed;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|BeginPlayData", meta = (DisplayName = "OriginalYaw", AllowPrivateAccess = true))
		float m_Yaw;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|BeginPlayData", meta = (DisplayName = "OriginalLocation", AllowPrivateAccess = true))
		FVector m_SpawnLocation;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|Animations", meta = (DisplayName = "AnimationTimeline", AllowPrivateAccess = true))
	    class UTimelineComponent* m_AnimationTimeline;

	float m_DeltaSecs;
	const float m_kBounceFactor=.0f;
	const float m_kYawLeanOnMovement=.0f;
	const float m_kYawLeanInterpSpeed=.0f;
	const float m_kMovementDriftInterpSpeed=.0f;
	AMN2GameMode* m_GameMode;
	UStaticMeshComponent* m_MeshRoot;

	FOnTimelineFloat m_SpawnAnimationUpdateDelegate{};
	FOnTimelineEvent m_SpawnAnimationFinishDelegate{};

	FTimerHandle m_PrimaryActionRepeat;
	FTimerHandle m_SecondaryActionRepeat;
    FTimerDelegate m_OnPrimaryActionDelegate;
	FTimerDelegate m_OnSecondaryActionDelegate;

public:	
	AMN2Player();

protected:

	virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
		void MoveUpDown(float delta);
	UFUNCTION()
		void MoveLeftRight(float delta);
	UFUNCTION()
		void FirePrimary();
	UFUNCTION()
		void ReleasePrimary();
	UFUNCTION()
		void OnFirePrimaryAction();
	UFUNCTION()
		void FireSecondary();
	UFUNCTION()
		void ReleaseSecondary();
	UFUNCTION()
		void OnFireSecondaryAction();
	UFUNCTION()
		void OnSpawnAnimationUpdate(float delta);
	UFUNCTION()
		void OnSpawnAnimationFinish();

	UFUNCTION()
		void OnBoxCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void OnTakeAnyDamageDelegate(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	float ComputeMovementAmount(float direction, float speed) const;
};
