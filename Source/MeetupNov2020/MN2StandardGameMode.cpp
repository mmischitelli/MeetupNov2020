#include "MN2StandardGameMode.h"
#include "EngineUtils.h"
#include "MN2EventArgs.h"
#include "Characters/MN2Asteroid.h"
#include "Engine/TriggerBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/MN2RepeatedAction.h"


void AMN2StandardGameMode::BeginPlay()
{
	Super::BeginPlay();

	m_PreparingToSpawnBoss = false;

	for(TActorIterator<ATriggerBox> Itr(GetWorld()); Itr; ++Itr)
	{
		const auto actor = *Itr;
		if (actor->ActorHasTag("EnemySpawnZone"))
		{
			m_EnemySpawnZone = actor;
			break;
		}
	}

	m_RepeatedAction = UMN2RepeatedAction::RepeatAction(this, m_EnemySpawnDelay, 0);
	m_RepeatedAction->OnFinished.AddWeakLambda(this, [&](int32 iterationIndex)
	{
		_ResumeEnemyTimer();
	});
	m_RepeatedAction->OnPerformAction.AddWeakLambda(this, [&](int32 iterationIndex)
	{
		if (m_EnemyCount < m_MaxNumOfEnemies)
		{
			const FTransform transform{ FQuat::Identity, GetRandomPointInSpawnZone(m_EnemySpawnZone) };
			GetWorld()->SpawnActor<AMN2EnemyShip>(m_StandardEnemyClass, transform);
		}
	});

	_ResumeEnemyTimer();
	_ResumeAsteroidTimer();
}

void AMN2StandardGameMode::SpawnEnemies()
{
	if (!m_CanSpawnEnemies)
		return;

	const FTransform transform{ FQuat::Identity, GetRandomPointInSpawnZone(m_EnemySpawnZone) };

	if (m_PreparingToSpawnBoss)
	{
		if (m_EnemyCount == 0)
		{
			m_PreparingToSpawnBoss = false;
			_StopEnemyTimer();
			GetWorld()->SpawnActor<AMN2EnemyShip>(m_BossEnemyClass, transform);
		}
	}
	else if (m_EnemyCount < m_MaxNumOfEnemies)
	{
		_StopEnemyTimer();
		m_RepeatedAction->TotalActionCount = UKismetMathLibrary::RandomIntegerInRange(m_MinGroupSize, m_MaxNumOfEnemies - m_EnemyCount);
		m_RepeatedAction->Start();
	}
}

void AMN2StandardGameMode::SpawnAsteroids()
{
	if (!m_CanSpawnAsteroids)
		return;
	
	const FTransform transform{ FQuat::Identity, GetRandomPointInSpawnZone(m_EnemySpawnZone) };
	GetWorld()->SpawnActor<AMN2Asteroid>(m_AsteroidClass, transform);
}

void AMN2StandardGameMode::OnEnemyKilledAction(FEnemyKilledEventArgs args)
{
	Super::OnEnemyKilledAction(args);

	if (m_NumKilledEnemies > 0 && m_NumKilledEnemies % (m_MaxNumOfEnemies * 2) == 0) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Preparing to spawn BOSS!!!");
		m_PreparingToSpawnBoss = true;
	}

	if (args.EnemyClass == m_BossEnemyClass) {
		_ResumeEnemyTimer();
	}
}

void AMN2StandardGameMode::SetCanSpawnEnemies(bool newValue)
{
	m_CanSpawnEnemies = newValue;
	if (!m_CanSpawnEnemies) {
		_StopEnemyTimer();
	}
	else if (!m_EnemiesTimerHandle.IsValid()) {
		_ResumeEnemyTimer();
	}
}

void AMN2StandardGameMode::SetCanSpawnAsteroids(bool newValue)
{
	m_CanSpawnAsteroids = newValue;
	if (!m_CanSpawnAsteroids) {
		_StopAsteroidTimer();
	}
	else if (!m_AsteroidsTimerHandle.IsValid()) {
		_ResumeAsteroidTimer();
	}
}

void AMN2StandardGameMode::_ResumeEnemyTimer()
{
	GetWorldTimerManager().SetTimer(m_EnemiesTimerHandle, this, &AMN2StandardGameMode::SpawnEnemies, m_EnemiesGroupSpawnRate, true);
}

void AMN2StandardGameMode::_StopEnemyTimer()
{
	GetWorldTimerManager().ClearTimer(m_EnemiesTimerHandle);
}

void AMN2StandardGameMode::_ResumeAsteroidTimer()
{
	GetWorldTimerManager().SetTimer(m_AsteroidsTimerHandle, this, &AMN2StandardGameMode::SpawnAsteroids, m_AsteroidsSpawnRate, true);
}

void AMN2StandardGameMode::_StopAsteroidTimer()
{
	GetWorldTimerManager().ClearTimer(m_AsteroidsTimerHandle);
}


