#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeetupNov2020/MN2EventArgs.h"
#include "MN2EnemyBase.generated.h"

DECLARE_EVENT_OneParam(AMN2EnemyShip, FOnEnemyKilledEvent, FEnemyKilledEventArgs)
DECLARE_EVENT_OneParam(AMN2EnemyShip, FOnEnemySpawnedEvent, FEnemySpawnedEventArgs)

UCLASS()
class MEETUPNOV2020_API AMN2EnemyBase : public AActor
{
	GENERATED_BODY()

protected:
	
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Geometry", meta = (DisplayName = "StaticMeshComponent", AllowPrivateAccess = true))
		class UStaticMeshComponent* m_MeshComponent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|Status", meta = (DisplayName = "CurrentMovementSpeed", AllowPrivateAccess = true))
		FVector2D m_CurrentMovementSpeed;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|BeginPlayData", meta = (DisplayName = "OriginalLocation", AllowPrivateAccess = true))
		FVector m_SpawnLocation;

public:
	
	FOnEnemyKilledEvent OnEnemyKilled;
	FOnEnemySpawnedEvent OnEnemySpawned;
	
	AMN2EnemyBase();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual FEnemySpawnedEventArgs GetEnemySpawnedEventArgs() const PURE_VIRTUAL(AMN2EnemyBase::GetEnemySpawnedEventArgs, return {}; );
	virtual FEnemyKilledEventArgs GetEnemyKilledEventArgs() const PURE_VIRTUAL(AMN2EnemyBase::GetEnemyKilledEventArgs, return {}; );
	
private:

	UFUNCTION()
		void OnCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void OnTakeAnyDamageDelegate(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
};
