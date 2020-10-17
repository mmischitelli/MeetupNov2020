#include "MN2GameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

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
}


void AMN2GameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    SaveScore();
}

void AMN2GameMode::LoadScore()
{
}

void AMN2GameMode::SaveScore()
{
}

void AMN2GameMode::IncrementScoreBy(int amount)
{
    m_CurrentScore += amount;
    if (m_CurrentScore > m_HighScore)
        m_HighScore = m_CurrentScore;
}

void AMN2GameMode::ShowGameOverScreen()
{
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
