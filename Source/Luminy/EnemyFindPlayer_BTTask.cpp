#include "EnemyFindPlayer_BTTask.h"
#include "Kismet/GameplayStatics.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"

UEnemyFindPlayer_BTTask::UEnemyFindPlayer_BTTask()
{
    NodeName = "Find Player";
}

EBTNodeResult::Type UEnemyFindPlayer_BTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0) || APlayerCharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) {
        auto Blackboard = OwnerComp.GetBlackboardComponent();
        Blackboard->SetValueAsObject(PlayerKey.SelectedKeyName, Player);

        return EBTNodeResult::Succeeded;
    };

    return EBTNodeResult::Failed;
}