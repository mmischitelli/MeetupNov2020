#pragma once
#include "CoreMinimal.h"
#include "MN2RateLimitedAction.generated.h"

DECLARE_EVENT(UMN2RateLimitedAction, OnFireActionDelegate)

UCLASS()
class MEETUPNOV2020_API UMN2RateLimitedAction : public UObject
{
	GENERATED_BODY()

	float m_Rate = 100.0f;
	bool m_ShouldStop = false;
	FTimerHandle m_ActionHandle;
	UWorld* m_World;

public:

	void SetRate(float rate) { m_Rate = rate; }
	float GetRate() const { return m_Rate; }

	OnFireActionDelegate OnFireAction;

	void Start(UWorld* world);
	void Stop();

private:

	void _ExecuteAction();

};