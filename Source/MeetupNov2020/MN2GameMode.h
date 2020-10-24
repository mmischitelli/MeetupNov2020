#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MN2GameMode.generated.h"

class UUserWidget;
class USoundBase;

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
class MEETUPNOV2020_API AMN2GameMode : public AGameMode
{
    GENERATED_BODY()

    int m_EnemyCount = 0;
    int m_NumKilledEnemies = 0;
    int m_CurrentScore = 0;
    int m_HighScore = 0;
public:
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|UI", meta = (DisplayName = "MainHUD"))
    UUserWidget* m_MainHUD;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|UI", meta = (DisplayName = "GameOverWidget"))
    UUserWidget* m_GameOverWidget;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Audio", meta = (DisplayName = "GameSoundtrack"))
    USoundBase* m_GameSoundtrack;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Tracking", meta = (ClampMin = "2", ClampMax = "20"))
    int MaxNumOfEnemies;

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

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION(BlueprintCallable, Category="MN2|Score")
        void LoadScore();
    UFUNCTION(BlueprintCallable, Category = "MN2|Score")
        void SaveScore();
    UFUNCTION(BlueprintCallable, Category = "MN2|Score")
        void IncrementScoreBy(int amount);
    UFUNCTION(BlueprintCallable, Category = "MN2|Tracking")
        void IncrementNumOfEnemies() { ++m_EnemyCount; }
    UFUNCTION(BlueprintCallable, Category = "MN2|Tracking")
        void DecrementNumOfEnemies() { --m_EnemyCount; }
    UFUNCTION(BlueprintCallable, Category = "MN2|Tracking")
        void IncrementNumOfEnemiesKilled() { ++m_NumKilledEnemies; DecrementNumOfEnemies(); }
    UFUNCTION(BlueprintCallable, Category = "MN2|UI")
        void ShowGameOverScreen();
};