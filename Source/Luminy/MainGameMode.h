#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainGameMode.generated.h"

UCLASS()
class LUMINY_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()
	
	AMainGameMode();

	virtual void Tick(float DeltaTime) override;
};