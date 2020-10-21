#pragma once

#include "CoreMinimal.h"
#include "AILogicSubsystem.generated.h"

UCLASS()
class MEETUPNOV2020_API UAILogicSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
        float m_EngagementDistanceFromPlayer;

};
