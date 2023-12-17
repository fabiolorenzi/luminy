#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class LUMINY_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerVars)
		bool IsPaused;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerVars)
		float Life;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerVars)
		bool IsPlayerDead;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerVars)
		int CatchedTargets;

	void MoveForward(float Axis);
	void MoveRight(float Axis);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};