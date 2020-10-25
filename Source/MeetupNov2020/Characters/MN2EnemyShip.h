#pragma once

#include "CoreMinimal.h"
#include "MN2EnemyBase.h"
#include "Components/TimelineComponent.h"
#include "MN2EnemyShip.generated.h"

UCLASS()
class MEETUPNOV2020_API AMN2EnemyShip : public AMN2EnemyBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Stats", meta = (DisplayName = "FireRate", AllowPrivateAccess = true, ClampMin = "0.1", ClampMax = "0.8"))
		float m_FireRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Stats", meta = (DisplayName = "HealthGlobeProbability", AllowPrivateAccess = true, ClampMin = "0.1", ClampMax = "0.8"))
		float m_HealthGlobeProbability;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Stats", meta = (DisplayName = "EngagementDistance", AllowPrivateAccess = true, ClampMin = "10", ClampMax = "1000"))
		int m_EngagementDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Animations", meta = (DisplayName = "MovementOffsetAtSpawn", AllowPrivateAccess = true, ClampMin = "100", ClampMax = "300"))
		float m_MovementOffsetAtSpawn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Animations", meta = (DisplayName = "SpawnAnimationCurve", AllowPrivateAccess = true))
		class UCurveFloat* m_SpawnAnimationCurve;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Audio", meta = (DisplayName = "GunFireDelay", AllowPrivateAccess = true, ClampMin = "-2.0", ClampMax = "2.0"))
		float m_GunFireDelay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Audio", meta = (DisplayName = "GunFireSound", AllowPrivateAccess = true))
		class USoundBase* m_GunFireSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Spawns", meta = (DisplayName = "GunFireProjectile", AllowPrivateAccess = true))
		class TSubclassOf<AActor> m_GunFireProjectile;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|Status", meta = (DisplayName = "CanMove", AllowPrivateAccess = true))
		bool m_CanMove;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|Animations", meta = (DisplayName = "AnimationTimeline", AllowPrivateAccess = true))
		class UTimelineComponent* m_AnimationTimeline;

	FOnTimelineFloat m_SpawnAnimationUpdateDelegate{};
	FOnTimelineEvent m_SpawnAnimationFinishDelegate{};

	FTimerHandle m_FireActionRepeat;

public:
	AMN2EnemyShip();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void StartFiring();
	void StopFiring();
	float GetEngagementDistance() const { return m_EngagementDistance; }

protected:
	virtual FEnemySpawnedEventArgs GetEnemySpawnedEventArgs() const override {
		return { GetClass(), GetActorLocation(), true };
	}
	virtual FEnemyKilledEventArgs GetEnemyKilledEventArgs() const override {
		return { GetClass(), GetActorLocation(), true, m_ScoreValue, m_HealthGlobeProbability };
	}
	
private:
	UFUNCTION()
		void OnFireAction();
	UFUNCTION()
		void OnSpawnAnimationUpdate(float delta);
	UFUNCTION()
		void OnSpawnAnimationFinish();

	void _FireProjectile() const;
	void _PlayGunfire() const;

};
