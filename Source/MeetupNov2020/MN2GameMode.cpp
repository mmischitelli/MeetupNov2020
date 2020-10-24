#include "MN2GameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/BufferArchive.h"
#include "Subsystems/EnemyAILogicSubsystem.h"

void AMN2GameMode::BeginPlay()
{
    const auto playerCtrl = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    playerCtrl->SetInputMode(FInputModeGameOnly());

    LoadScore();

    const FStringClassReference hudWidgetRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BPW_HUD.BPW_HUD_C'"));
    if (UClass* hudWidget = hudWidgetRef.TryLoadClass<UUserWidget>())
    {
        m_MainHUD = CreateWidget<UUserWidget>(GetWorld(), hudWidget);
        m_MainHUD->AddToViewport();
    }

    if (m_GameSoundtrack != nullptr) {
        UGameplayStatics::PlaySound2D(GetWorld(), m_GameSoundtrack);
    }

    GetWorld()->GetSubsystem<UEnemyAILogicSubsystem>()->Start();
}


void AMN2GameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    GetWorld()->GetSubsystem<UEnemyAILogicSubsystem>()->Stop();	
}

void AMN2GameMode::LoadScore()
{
    FBufferArchive fromBinary;
    FFileHelper::LoadFileToArray(fromBinary, L"history");

    if (fromBinary.TotalSize() <= 0)
        return;

    FSaveGameData saveGameData;
    FMemoryReader reader(fromBinary);
    reader << saveGameData;

    m_HighScore = saveGameData.HighScore;
}

void AMN2GameMode::SaveScore()
{
    FSaveGameData saveGameData { m_HighScore };

    FBufferArchive toBinary;
    toBinary << saveGameData;

    FFileHelper::SaveArrayToFile(toBinary, L"history");
}

void AMN2GameMode::IncrementScoreBy(int amount)
{
    m_CurrentScore += amount;
    if (m_CurrentScore > m_HighScore)
        m_HighScore = m_CurrentScore;
}

void AMN2GameMode::ShowGameOverScreen()
{
    SaveScore();

    const FStringClassReference gameOverWidgetRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BPW_GameOver.BPW_GameOver_C'"));
    if (UClass* gameOverWidget = gameOverWidgetRef.TryLoadClass<UUserWidget>())
    {
        m_GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), gameOverWidget);
        m_GameOverWidget->AddToViewport();

        const auto playerCtrl = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        playerCtrl->bShowMouseCursor = true;

        FInputModeUIOnly uiOnlyMode;
        uiOnlyMode.SetWidgetToFocus(m_GameOverWidget->TakeWidget());
        uiOnlyMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

        playerCtrl->SetInputMode(uiOnlyMode);
    }
}
