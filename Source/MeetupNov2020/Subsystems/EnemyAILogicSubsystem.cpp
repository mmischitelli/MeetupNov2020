#include "EnemyAILogicSubsystem.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "MeetupNov2020/Characters/MN2EnemyShip.h"
#include "MeetupNov2020/Characters/MN2Player.h"


void UEnemyAILogicSubsystem::Start()
{
	FTimerHandle enemyFireLogic;
	GetWorld()->GetTimerManager().SetTimer(enemyFireLogic, this, &UEnemyAILogicSubsystem::EnemyFireLogic, 0.1f, true, 0);
}

void UEnemyAILogicSubsystem::Stop()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	// Make all enemies stop firing when the AI logic is stopped
	for (TActorIterator<AMN2EnemyShip> Itr(GetWorld()); Itr; ++Itr)
	{
		auto* enemy = *Itr;
		if (IsValid(enemy)) {
			enemy->StopFiring();
		}
	}
}

void UEnemyAILogicSubsystem::EnemyFireLogic() const
{
	const auto* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!IsValid(player))
		return;

	for(TActorIterator<AMN2EnemyShip> Itr(GetWorld()); Itr; ++Itr)
	{
		auto* enemy = *Itr;
		if (!IsValid(enemy))
			continue;

		if (abs(player->GetActorLocation().Y - enemy->GetActorLocation().Y) <= enemy->GetEngagementDistance()) {
			enemy->StartFiring();
		}
		else {
			enemy->StopFiring();
		}

	}
}
