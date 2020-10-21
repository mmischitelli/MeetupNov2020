#include "EnemyAILogicSubsystem.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "MeetupNov2020/Characters/MN2EnemyBase.h"
#include "MeetupNov2020/Characters/MN2Player.h"


void UEnemyAILogicSubsystem::Start()
{
	FTimerHandle enemyFireLogic;
	GetWorld()->GetTimerManager().SetTimer(enemyFireLogic, this, &UEnemyAILogicSubsystem::EnemyFireLogic, 0.1f, true, 0);
}

void UEnemyAILogicSubsystem::Stop()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UEnemyAILogicSubsystem::EnemyFireLogic() const
{
	const auto* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!IsValid(player))
		return;

	for(TActorIterator<AMN2EnemyBase> Itr(GetWorld()); Itr; ++Itr)
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
