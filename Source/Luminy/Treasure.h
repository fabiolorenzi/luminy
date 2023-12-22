#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Treasure.generated.h"

class USoundCue;

UCLASS()
class LUMINY_API ATreasure : public AActor
{
	GENERATED_BODY()
	
public:	
	ATreasure();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TreasureVars)
		float RotationValue;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void Catch();
	void RotateCrystal(float Rotation);

private:
	UPROPERTY(EditAnywhere)
		USoundCue* CatchSound;
};