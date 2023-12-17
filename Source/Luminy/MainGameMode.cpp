#include "MainGameMode.h"
#include "GameFramework/Actor.h"

AMainGameMode::AMainGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AMainGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}