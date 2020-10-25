#include "MN2GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Characters/MN2EnemyBase.h"
#include "Components/BoxComponent.h"
#include "Engine/TriggerBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Serialization/BufferArchive.h"
#include "Subsystems/EnemyAILogicSubsystem.h"


void AMN2GameModeBase::BeginPlay()
{
    LoadScore();
	
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());

    const FStringClassReference hudWidgetRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BPW_HUD.BPW_HUD_C'"));
    if (UClass* hudWidget = hudWidgetRef.TryLoadClass<UUserWidget>())
    {
        m_MainHUD = CreateWidget<UUserWidget>(GetWorld(), hudWidget);
        m_MainHUD->AddToViewport();
    }

    if (m_GameSoundtrack) {
        UGameplayStatics::PlaySound2D(GetWorld(), m_GameSoundtrack);
    }

    GetWorld()->GetSubsystem<UEnemyAILogicSubsystem>()->Start();
}

void AMN2GameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    GetWorld()->GetSubsystem<UEnemyAILogicSubsystem>()->Stop();
}

void AMN2GameModeBase::RegisterEnemyEvents(AMN2EnemyBase* enemy)
{
	enemy->OnEnemySpawned.AddUObject(this, &AMN2GameModeBase::OnEnemySpawnedAction);
	enemy->OnEnemyKilled.AddUObject(this, &AMN2GameModeBase::OnEnemyKilledAction);
}

void AMN2GameModeBase::LoadScore()
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

void AMN2GameModeBase::SaveScore()
{
    FSaveGameData saveGameData{ m_HighScore };

    FBufferArchive toBinary;
    toBinary << saveGameData;

    FFileHelper::SaveArrayToFile(toBinary, L"history");
}

void AMN2GameModeBase::OnEnemySpawnedAction(FEnemySpawnedEventArgs args)
{
    if (args.CountsTowardEnemyLimit) {
        ++m_EnemyCount;
    }
}

void AMN2GameModeBase::OnEnemyKilledAction(FEnemyKilledEventArgs args)
{
    if (args.CountsTowardEnemyLimit) {
        --m_EnemyCount;
    }

	// if the score is 0, it means it didn't get killed by the player
	if (args.ScoreValue > 0)
	{
        ++m_NumKilledEnemies;
        m_CurrentScore += args.ScoreValue;
        if (m_CurrentScore > m_HighScore)
            m_HighScore = m_CurrentScore;

        if (UKismetMathLibrary::RandomFloatInRange(.0f, 1.0f) <= args.HealthGlobeProbability) {
            GetWorld()->SpawnActor<AActor>(m_HealthGlobe, args.Location, FRotator::ZeroRotator, FActorSpawnParameters());
        }
	}
}

void AMN2GameModeBase::ShowGameOverScreen()
{
    GetWorld()->GetSubsystem<UEnemyAILogicSubsystem>()->Stop();

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

FVector AMN2GameModeBase::GetRandomPointInSpawnZone(ATriggerBox* spawnZone)
{
    if (!IsValid(spawnZone))
        return FVector::ZeroVector;
	
    const auto actorComp = spawnZone->GetComponentByClass(UBoxComponent::StaticClass());
    if (actorComp)
    {
        const auto collisionComp = static_cast<UBoxComponent*>(actorComp);
        const auto location = UKismetMathLibrary::RandomPointInBoundingBox(collisionComp->Bounds.Origin, collisionComp->Bounds.BoxExtent);
        return FVector{ spawnZone->GetActorLocation().X, location.Y, location.Z };
    }

    return FVector::ZeroVector;
}