#pragma once

#pragma once

struct FEnemySpawnedEventArgs
{
	class UClass* EnemyClass;
	FVector Location;
	bool CountsTowardEnemyLimit;
};
struct FEnemyKilledEventArgs
{
	class UClass* EnemyClass;
	FVector Location;
	bool CountsTowardEnemyLimit;
	int ScoreValue;
	float HealthGlobeProbability;

	FString ToString() const
	{
		return FString::Printf(TEXT("Class: %s - Location %s - CountsTowardsEnemyLimit: %s - Score: %d - HealthGlobeProbability: %0.2f"), 
			*EnemyClass->GetFName().ToString(), *Location.ToCompactString(), *FString(CountsTowardEnemyLimit ? "true" : "false"), ScoreValue, HealthGlobeProbability);
	}
};
