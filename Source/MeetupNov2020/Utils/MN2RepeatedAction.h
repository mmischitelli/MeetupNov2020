#pragma once

#include "CoreMinimal.h"
#include "MN2RepeatedAction.generated.h"

DECLARE_EVENT_OneParam(UMN2RepeatedAction, FRepeatedActionDelegate, int32)

UCLASS()
class MEETUPNOV2020_API UMN2RepeatedAction : public UObject
{
	GENERATED_BODY()

	class UWorld* m_World;
	int32 NumPerformedActions;
	FTimerHandle m_PerformActionHandle;

public:
	int32 TotalActionCount;
	float TimeBetweenActions;
	
	FRepeatedActionDelegate OnPerformAction;
	FRepeatedActionDelegate OnFinished;

	static UMN2RepeatedAction* RepeatAction(AActor* owner, float timeBetweenActions, int32 totalActionCount);

	void Start();
private:

	void _PerformAction();
	void _EndTask();

};