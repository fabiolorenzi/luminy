#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "PlayerCharacter.generated.h"

class USoundCue;
class UAudioComponent;

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
		bool HasWon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerVars)
		bool IsDead;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerVars)
		float Life;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerVars)
		bool IsPlayerDead;
	UPROPERTY(VisibleAnywhere, BlueprintreadOnly, Category = PlayerVars)
		bool IsPlayerUnderAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerVars)
		int CatchedTargets;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerVars)
		float RunningPower;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlayerVars)
		int Seconds;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlayerVars)
		int Minutes;
	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Sweep);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	bool IsRunning;
	bool IsRunningBlocked;

	float WalkingSpeed;
	float TemporaryTime;

	void MoveForward(float Axis);
	void MoveRight(float Axis);
	void PlayerRunning();
	void PlayerNotRunning();
	void PauseGame();
	void RestartGame();
	void QuitGame();
	void Victory();
	void Death();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY(EditAnywhere)
		USoundCue* AttackSound;
	UAudioComponent* AttackSoundComponent;
	bool IsAttackSoundPlaying;
};
