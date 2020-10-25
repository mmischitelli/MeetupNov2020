// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MN2GameModeBase.generated.h"

class ATriggerBox;
struct FEnemySpawnedEventArgs;
struct FEnemyKilledEventArgs;

struct FSaveGameData
{
    int HighScore;

    friend FArchive& operator<<(FArchive& Ar, FSaveGameData& data)
    {
        Ar << data.HighScore;
        return Ar;
    }
};

/**
 * 
 */
UCLASS()
class MEETUPNOV2020_API AMN2GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Spawn|Classes", meta = (DisplayName = "HealthGlobe", AllowPrivateAccess = true))
        class TSubclassOf<AActor> m_HealthGlobe;
	
protected:
    int m_EnemyCount = 0;
    int m_NumKilledEnemies = 0;
    int m_CurrentScore = 0;
    int m_HighScore = 0;

public:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    void RegisterEnemyEvents(class AMN2EnemyBase* enemy);

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|UI", meta = (DisplayName = "MainHUD"))
        class UUserWidget* m_MainHUD;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|UI", meta = (DisplayName = "GameOverWidget"))
        class UUserWidget* m_GameOverWidget;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Audio", meta = (DisplayName = "GameSoundtrack"))
        class USoundBase* m_GameSoundtrack;

    UFUNCTION(BlueprintCallable, Category = "MN2|Tracking")
        int GetCurrentEnemyCount() const { return m_EnemyCount; }
    UFUNCTION(BlueprintCallable, Category = "MN2|Tracking")
        int GetNumKilledEnemies() const { return m_NumKilledEnemies; }
    UFUNCTION(BlueprintCallable, Category = "MN2|Score")
        int GetCurrentScore() const { return m_CurrentScore; }
    UFUNCTION(BlueprintCallable, Category = "MN2|Score")
        int GetHighScore() const { return m_HighScore; }
    UFUNCTION(BlueprintCallable, Category = "MN2|GUI")
        UUserWidget* GetHUD() const { return m_MainHUD; }

    UFUNCTION(BlueprintCallable, Category = "MN2|Score")
        void LoadScore();
    UFUNCTION(BlueprintCallable, Category = "MN2|Score")
        void SaveScore();
    UFUNCTION(BlueprintCallable, Category = "MN2|UI")
        void ShowGameOverScreen();

    virtual void OnEnemySpawnedAction(FEnemySpawnedEventArgs args);
    virtual void OnEnemyKilledAction(FEnemyKilledEventArgs args);

protected:
    static FVector GetRandomPointInSpawnZone(ATriggerBox* spawnZone);
	
};
