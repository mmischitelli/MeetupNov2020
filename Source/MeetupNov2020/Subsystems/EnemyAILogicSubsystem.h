#pragma once

#include "CoreMinimal.h"
#include "EnemyAILogicSubsystem.generated.h"

UCLASS()
class MEETUPNOV2020_API UEnemyAILogicSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    void Start();
    void Stop();

private:
    void EnemyFireLogic() const;
	
};
 