#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PlayerCharacter.h"
#include "EnemyFindPlayer_BTTask.generated.h"

UCLASS()
class LUMINY_API UEnemyFindPlayer_BTTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UEnemyFindPlayer_BTTask();

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector PlayerKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};