#pragma once

#include "CoreMinimal.h"
#include "MN2GameModeBase.h"
#include "Characters/MN2EnemyShip.h"
#include "Characters/MN2Asteroid.h"


#include "MN2StandardGameMode.generated.h"

/**
 *
 */
UCLASS()
class MEETUPNOV2020_API AMN2StandardGameMode : public AMN2GameModeBase
{
    GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Spawn|Classes", meta = (DisplayName = "StandardEnemyClass", AllowPrivateAccess = true))
		TSubclassOf<AMN2EnemyShip> m_StandardEnemyClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Spawn|Classes", meta = (DisplayName = "BossEnemyClass", AllowPrivateAccess = true))
		TSubclassOf<AMN2EnemyShip> m_BossEnemyClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Spawn|Classes", meta = (DisplayName = "AsteroidClass", AllowPrivateAccess = true))
		TSubclassOf<AMN2Asteroid> m_AsteroidClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Spawn|Enemies", meta = (DisplayName = "MaxNumberOfEnemies", ClampMin = "2", ClampMax = "20", AllowPrivateAccess = true))
		int m_MaxNumOfEnemies;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Spawn|Enemies", meta = (DisplayName = "EnemySpawnDelay", ClampMin = "0.1", ClampMax = "0.5", AllowPrivateAccess = true))
		float m_EnemySpawnDelay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Spawn|Enemies", meta = (DisplayName = "MinimumGroupSize", ClampMin = "2", ClampMax = "10", AllowPrivateAccess = true))
		int32 m_MinGroupSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Spawn|Enemies", meta = (DisplayName = "EnemiesGroupSpawnRate", ClampMin = "1.0", ClampMax = "10.0", AllowPrivateAccess = true))
		float m_EnemiesGroupSpawnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Spawn|Others", meta = (DisplayName = "AsteroidsSpawnRate", ClampMin = "0.5", ClampMax = "5.0", AllowPrivateAccess = true))
		float m_AsteroidsSpawnRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MN2|Spawn|Toggles", BlueprintSetter = SetCanSpawnEnemies, meta = (DisplayName = "CanSpawnEnemies", AllowPrivateAccess = true))
		bool m_CanSpawnEnemies;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MN2|Spawn|Toggles", BlueprintSetter = SetCanSpawnAsteroids, meta = (DisplayName = "CanSpawnAsteroids", AllowPrivateAccess = true))
		bool m_CanSpawnAsteroids;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|Spawn|Debug", meta = (DisplayName = "PreparingToSpawnBoss", AllowPrivateAccess = true))
		bool m_PreparingToSpawnBoss;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|Spawn|Debug", meta = (DisplayName = "RepeatedActionManager", AllowPrivateAccess = true))
		class UMN2RepeatedAction* m_RepeatedAction;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|Spawn|Debug", meta = (DisplayName = "EnemySpawnZone", AllowPrivateAccess = true))
		class ATriggerBox* m_EnemySpawnZone;
	
	FTimerHandle m_EnemiesTimerHandle;
	FTimerHandle m_AsteroidsTimerHandle;


public:
	virtual void BeginPlay() override;

private:
	UFUNCTION(BlueprintCallable)
		void SetCanSpawnEnemies(bool newValue);
	UFUNCTION(BlueprintCallable)
		void SetCanSpawnAsteroids(bool newValue);
	
	void SpawnEnemies();
	void SpawnAsteroids();

	void _ResumeEnemyTimer();
	void _StopEnemyTimer();
	void _ResumeAsteroidTimer();
	void _StopAsteroidTimer();

	virtual void OnEnemyKilledAction(FEnemyKilledEventArgs args) override;
};