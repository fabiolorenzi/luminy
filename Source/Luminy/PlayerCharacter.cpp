#include "PlayerCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 20.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = true;

	IsPaused = false;
	IsPlayerDead = false;
	Life = 100.0f;
	RunningPower = 30.0f;
	IsRunningBlocked = false;
	IsRunning = false;
	CatchedTargets = 0;

	WalkingSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void APlayerCharacter::MoveForward(float Axis)
{
	if (!IsPlayerDead) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Axis);
	};
}

void APlayerCharacter::MoveRight(float Axis)
{
	if (!IsPlayerDead) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	};
}

void APlayerCharacter::PlayerRunning()
{
	if (!IsRunningBlocked) {
		IsRunning = true;
		GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed * 2;
	};
}

void APlayerCharacter::PlayerNotRunning()
{
	IsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsRunning && RunningPower > 0.0f && !IsRunningBlocked) {
		RunningPower -= DeltaTime * 2;
	} else if (RunningPower < 30.0f) {
		RunningPower += DeltaTime * 0.5f;
	};

	if (RunningPower <= 0) {
		IsRunningBlocked = true;
	};

	if (IsRunningBlocked && RunningPower >= 15.0f) {
		IsRunningBlocked = false;
	};
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Running", IE_Pressed, this, &APlayerCharacter::PlayerRunning);
	PlayerInputComponent->BindAction("Running", IE_Released, this, &APlayerCharacter::PlayerNotRunning);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
}